#include <iostream>

#define CL_HPP_TARGET_OPENCL_VERSION 200
#include <opencl.hpp>

#ifndef __PI_CALC_GPU_HPP__
#define __PI_CALC_GPU_HPP__

// The number of iterations
const unsigned long long N = 100000000; // 100 Million

// Get the approperiate Work Item size a.k.a LENGTH
size_t wi_size(const cl::Device &d);

// Get the OpenCL C program from source
cl::Program get_program();

#endif