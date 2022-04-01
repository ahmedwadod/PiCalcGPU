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

    // Get the first device
    auto device = devices.front();

    // Get the Work Item size (LENGTH)
    const int LENGTH = wiSize(device);

    // Allocate memory and generate the data
    unsigned long long *input_data = (unsigned long long *)malloc(sizeof(unsigned long long) * LENGTH);
    double *output_data = (double *)malloc(sizeof(double) * LENGTH);
    writeStartingPoints(input_data, LENGTH);

    // Load the program
    auto program = getProgram(N / LENGTH);

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

void writeStartingPoints(unsigned long long *mem, size_t length)
{
    auto iterations_per_wi = N / length;
    for (size_t i = 0; i < length; i++)
    {
        mem[i] = i * iterations_per_wi;
    }
}

bool _replace(cl::string &str, const cl::string &from, const cl::string &to)
{
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

cl::Program getProgram(unsigned long long diff)
{
    std::ifstream input_file("./calcPi.cl");
    if (!input_file.is_open())
    {
        std::cerr << "Could not open the file - 'calcPi.cl'" << std::endl;
        exit(EXIT_FAILURE);
    }
    cl::string src((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());

    _replace(src, "{N / CORES}", std::to_string(diff));
    return cl::Program(src);
}