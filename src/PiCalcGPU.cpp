#include <PiCalcGPU.hpp>

int main()
{
    // Get the platforms
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    // Make sure there is at least one usable platform
    if (platforms.size() < 1)
    {
        std::cout << "No OpenCL Platforms\n";
        return 1;
    }

    // Get the GPU devices from the first platform
    auto platform = platforms.front();
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

    // Make sure there is at least one usable device
    if (devices.size() < 1)
    {
        std::cout << "No Supported GPUs\n";
        return 1;
    }

    // Get the first device, context and queue
    auto device = devices.front();
    cl::Context context(device);
    cl::CommandQueue queue(context, device);
    queue.flush();

    // Get the Work Item size (LENGTH)
    const int LENGTH = wiSize(device);

    // Allocate memory and generate the data
    auto size_input = sizeof(unsigned long) * LENGTH;
    unsigned long input_data[LENGTH];
    auto size_output = sizeof(double) * LENGTH;
    double output_data[LENGTH];
    unsigned long *iter_per_wi = (unsigned long *)malloc(sizeof(unsigned long));
    *iter_per_wi = N / LENGTH;
    writeStartingPoints(input_data, LENGTH);

    // Load the program, build and make the kernel
    auto src = loadProgramSource();
    cl::Program program(context, src);
    auto err = program.build();
    if (err != 0)
    {
        auto log = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>();
        std::cerr << "Program build error: " << log[0].second << std::endl;
        return 1;
    }
    cl::Kernel kernel(program, "calcPi", &err);
    assert(err == CL_SUCCESS);

    // Create the buffers
    cl::Buffer input_buffer(context, CL_MEM_READ_WRITE, size_input);
    cl::Buffer output_buffer(context, CL_MEM_READ_WRITE, size_output);
    cl::Buffer iter_buffer(context, CL_MEM_READ_WRITE, sizeof(unsigned long));

    // Enqueue writing
    queue.enqueueWriteBuffer(input_buffer, CL_FALSE, 0, size_input, input_data);
    queue.enqueueWriteBuffer(iter_buffer, CL_FALSE, 0, sizeof(unsigned long), iter_per_wi);

    // Set kernel arguments
    kernel.setArg(0, sizeof(input_buffer), &input_buffer);
    kernel.setArg(1, sizeof(output_buffer), &output_buffer);
    kernel.setArg(2, sizeof(iter_buffer), &iter_buffer);

    // Execute the kernel
    cl::NDRange dims(LENGTH);
    queue.enqueueNDRangeKernel(kernel, 1, dims);

    // Enqueue reading
    queue.enqueueReadBuffer(output_buffer, CL_FALSE, 0, size_output, output_data);

    // Wait to finish
    queue.finish();

    double pi = 0;
    for (size_t i = 0; i < LENGTH; i++)
    {
        pi += output_data[i];
    }

    std::cout << pi << std::endl;

    return 0;
}

size_t wiSize(const cl::Device &d)
{
    auto wi_sizes = d.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
    size_t max_size_in_first_dim = wi_sizes[0];
    if ((N / max_size_in_first_dim) % 2 == 0)
    {
        return max_size_in_first_dim;
    }
    else if ((N / (max_size_in_first_dim / 2)) % 2 == 0)
    {
        return (max_size_in_first_dim / 2);
    }
    else
    {
        return (max_size_in_first_dim / 2 + 1);
    }
}

void writeStartingPoints(unsigned long *mem, size_t length)
{
    auto iterations_per_wi = N / length;
    for (size_t i = 0; i < length; i++)
    {
        mem[i] = i * iterations_per_wi;
    }
}

cl::string loadProgramSource()
{
    std::ifstream input_file("./calcPi.cl");
    if (!input_file.is_open())
    {
        std::cerr << "Could not open the file - 'calcPi.cl'" << std::endl;
        exit(EXIT_FAILURE);
    }
    cl::string src((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    return src;
}