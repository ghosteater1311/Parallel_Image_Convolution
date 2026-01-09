#include <cuda_runtime.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "convolution.hpp"
#include "utils.hpp"
#include "timer.hpp"

#define BLOCK_SIZE 16
#define TILE_SIZE 20

__global__ void convolveKernel(const float* input, const float* kernel, float* output, int W, int H, int C, int kernelSize)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if(x >= W || y >= H) return;

    int half = kernelSize / 2;

    // Iterate each color channel
    for(int c = 0; c < C; c++)
    {
        float sum = 0.0f;

        // Apply Kernel
        for(int ky = -half; ky <= half; ky++)
        {
            for(int kx = -half; kx <= half; kx++)
            {
                int ix = x + kx;
                int iy = y + ky;

                // Zero padding (skip if outside bound)
                if(ix >= 0 && ix < W && iy >= 0 && iy < H)
                {
                    // Read pixel value
                    float pixelVal = input[(iy * W + ix) * C + c];

                    // read kernel value
                    float kernelVal = kernel[(ky + half) * kernelSize + (kx + half)];

                    sum += pixelVal * kernelVal;
                }
            }
        }

        output[(y * W + x) * C + c] = sum;
    }
}

double convolveCUDA(const cv::Mat& input, const std::vector<float>& kernel, int kernelSize, cv::Mat& output)
{
    // Validate Kernel
    if(!utils::isValidKernel(kernel, kernelSize)) 
    {
        throw std::invalid_argument("Invalid kernel: size must be positive odd and match kernel vector length!");
    }

    // Convert input to float
    cv::Mat in = utils::convertToFloat(input);
    if(in.empty()) 
    {
        throw std::invalid_argument("Input image is empty or could not be converted to float!");
    }

    // Ensure output is allocated 
    utils::ensureOutputLike(in, output, CV_32F);

    // Extract dimensions
    const int H = in.rows;
    const int W = in.cols;
    const int C = in.channels();
    
    size_t imageSize = W * H * C * sizeof(float);
    size_t kernelSizeBytes = kernelSize * kernelSize * sizeof(float);

    // Timing the whole convolution operation
    timer::ScopedTimer st;

    // Allocate device (GPU) memory
    float *d_input, *d_kernel, *d_output;
    
    cudaMalloc(&d_input, imageSize);
    cudaMalloc(&d_kernel, kernelSizeBytes);
    cudaMalloc(&d_output, imageSize);

    // Copy data from host (CPU) to device (GPU)
    cudaMemcpy(d_input, in.data, imageSize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_kernel, kernel.data(), kernelSizeBytes, cudaMemcpyHostToDevice);

    // Config kernel launch parameter for GPU device
    dim3 blockSize(BLOCK_SIZE, BLOCK_SIZE);
    dim3 gridSize((W + BLOCK_SIZE - 1) / BLOCK_SIZE, (H + BLOCK_SIZE - 1) / BLOCK_SIZE);

    // Launch kernel
    convolveKernel <<< gridSize, blockSize >>> (d_input, d_kernel, d_output, W, H, C, kernelSize);

    // Wait for GPU finishing
    cudaDeviceSynchronize();

    // Copy result from GPU back to CPU
    cudaMemcpy(output.data, d_output, imageSize, cudaMemcpyDeviceToHost);

    // Free device memory (Avoid Memory leak)
    cudaFree(d_input);
    cudaFree(d_kernel);
    cudaFree(d_output);

    return st.elapsedMs();
}