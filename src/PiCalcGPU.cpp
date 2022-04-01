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

    return 0;
}

size_t wi_size(const cl::Device &d)
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