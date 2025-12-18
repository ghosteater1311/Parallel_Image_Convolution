// Define convolution.hpp
// - CPU: Single-threaded naive implementation
// - OMP: OpenMP multi-threaded parallel implementation  
// - CUDA: GPU-accelerated implementation
#ifndef CONVOLUTION_HPP
#define CONVOLUTION_HPP

// Include library for dev
#include <vector>
#include <opencv2/opencv.hpp>

// CPU Single-Threaded Convolution
// Performs 2D convolution using naive nested loops.
//
// Algorithm:
//   For each pixel (x, y) in output:
//     sum = 0
//     For each kernel weight at (i, j):
//       sum += input[x+i, y+j] * kernel[i, j]
//     output[x, y] = sum
//
// Time Complexity: O(W × H × K²)
//   W = image width
//   H = image height  
//   K = kernel size
//
// Parameters:
//   input      - Source image (grayscale or color)
//   kernel     - 1D vector containing K×K kernel weights (row-major)
//   kernelSize - Dimension of kernel (e.g., 3 for 3×3)
//   output     - Destination image (will be allocated if empty)
//
// Returns:
//   Execution time in milliseconds (for benchmarking)
double convolveCPU(
    const cv::Mat& input,
    const std::vector<float>& kernel,
    int kernelSize,
    cv::Mat& output
);

// OpenMP Multi-Threaded Convolution 
// Performs 2D convolution using OpenMP parallel for loops.
// Distributes rows across multiple CPU cores.
//
// Parallelization Strategy:
//   - Outer loop (rows) is parallelized with #pragma omp parallel for
//   - Each thread processes a chunk of rows independently
//   - No data races: each output pixel is written by exactly one thread
//
// Speedup Potential:
//   - Ideal: Linear with core count (8 cores = 8× faster)
//   - Real: Sub-linear due to memory bandwidth limits
//
// Time Complexity: O(W × H × K² / P)
//   P = number of threads/cores
//
// Parameters:
//   input      - Source image (grayscale or color)
//   kernel     - 1D vector containing K×K kernel weights (row-major)
//   kernelSize - Dimension of kernel (e.g., 3 for 3×3)
//   output     - Destination image (will be allocated if empty)
//   numThreads - Number of threads to use (0 = auto-detect all cores)
//
// Returns:
//   Execution time in milliseconds (for benchmarking)
double convolveOMP(
    const cv::Mat& input,
    const std::vector<float>& kernel,
    int kernelSize,
    cv::Mat& output,
    int numThreads = 0
);

// CUDA GPU-Accelerated Convolution
// Performs 2D convolution on NVIDIA GPU using CUDA.
// Massive parallelism: thousands of threads process pixels simultaneously.
//
// GPU Execution Model:
//   - Image divided into blocks (e.g., 16×16 pixels each)
//   - Each block runs on a Streaming Multiprocessor (SM)
//   - Each pixel = 1 CUDA thread
//   - GTX 1650: 896 CUDA cores running in parallel
//
// Memory Hierarchy:
//   - Global Memory: Large but slow (~400 GB/s)
//   - Shared Memory: Small but fast (per-block cache for kernel)
//   - Registers: Fastest (per-thread local variables)
//
// Data Transfer Overhead:
//   - CPU → GPU: Copy input image (PCIe bandwidth ~8 GB/s)
//   - GPU → CPU: Copy output image back
//   - For small images, transfer time may exceed compute time!
//
// Time Complexity: O(W × H × K² / C)
//   C = CUDA cores (896 for GTX 1650)
//
// Parameters:
//   input      - Source image (grayscale or color)
//   kernel     - 1D vector containing K×K kernel weights (row-major)
//   kernelSize - Dimension of kernel (e.g., 3 for 3×3)
//   output     - Destination image (will be allocated if empty)
//
// Returns:
//   Execution time in milliseconds (includes memory transfer)
double convolveCUDA(
    const cv::Mat& input,
    const std::vector<float>& kernel,
    int kernelSize,
    cv::Mat& output
);

#endif