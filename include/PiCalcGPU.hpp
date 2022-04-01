#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

#define CL_HPP_TARGET_OPENCL_VERSION 200
#include <opencl.hpp>

#ifndef __PI_CALC_GPU_HPP__
#define __PI_CALC_GPU_HPP__

// The number of iterations
const unsigned long N = 100000000; // 100 Million

// Get the approperiate Work Item size a.k.a LENGTH
size_t wiSize(const cl::Device &d);

// Write the iterations data.
// Write to a memory location of length = wi_size()
// the starting point of each Work Item.
void writeStartingPoints(unsigned long *mem, size_t length);

// Get the OpenCL C program from source
cl::string loadProgramSource(unsigned long diff);

#endif