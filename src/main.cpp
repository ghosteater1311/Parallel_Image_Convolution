#include <iostream>
#include <opencv2/opencv.hpp>
#include "convolution.hpp"
#include "kernels.hpp"
#include "utils.hpp"

int main()
{
    // Standard Image
    // Load Image 
    cv::Mat imgStandard = utils::loadImage("/image/standard/baboon_512.png", cv::IMREAD_COLOR, CV_8U);
    if(imgStandard.empty())
    {
        std::cout << "Cannot load image!\n";
        return 1;
    }
    std::cout << "Image loaded (baboon_512): " << imgStandard.cols << "x" << imgStandard.rows << "\n";

    // Get all benchmark kernels
    auto standardBenchmarkKernels = kernels::getAllBenchmarkKernels();
    std::cout << "Testing " << standardBenchmarkKernels.size() << " standard kernels:\n\n";

    // Test each kernel
    for(const auto& kernel : standardBenchmarkKernels) 
    {
        std::cout << "Kernel: " << kernel.name << " (" << kernel.size << "x" << kernel.size << ")\n";
        
        // CPU (Single Thread)
        cv::Mat outCPU;
        double cpuTime = convolveCPU(imgStandard, kernel.data, kernel.size, outCPU);
        std::cout << "CPU:         " << cpuTime << " ms\n";
        
        // OMP (Multi-Threads) - test with different thread counts
        int threads[] = {1, 2, 4, 8, 16};
        for(int thread : threads) 
        {
            cv::Mat outOMP;
            double ompTime = convolveOMP(imgStandard, kernel.data, kernel.size, outOMP, thread);
            double speedup = cpuTime / ompTime;
            
            std::cout << "OMP(" << thread << "):      " << ompTime << " ms  (Speedup: " << speedup << "x)\n";
        }
        
        // CUDA (GPU)
        cv::Mat outCUDA;
        double cudaTime = convolveCUDA(imgStandard, kernel.data, kernel.size, outCUDA);
        double cudaSpeedup = cpuTime / cudaTime;
        std::cout << "CUDA:        " << cudaTime << " ms  (Speedup: " << cudaSpeedup << "x)\n";
        
        // Verify correctness
        bool cudaCorrect = utils::imagesNearEqual(outCPU, outCUDA, 1e-3);
        std::cout << "CUDA correctness: " << (cudaCorrect ? "PASS" : "FAIL") << "\n";
        std::cout << "\n";
    }

    // Synthetic Image
    // Load Image 
    cv::Mat imgSynthetic = utils::loadImage("/image/synthetic/noise_4096.png", cv::IMREAD_COLOR, CV_8U);
    if(imgSynthetic.empty())
    {
        std::cout << "Cannot load image!\n";
        return 1;
    }
    std::cout << "Image loaded (noise_4096): " << imgSynthetic.cols << "x" << imgSynthetic.rows << "\n";

    // Get all benchmark kernels
    auto syntheticBenchmarkKernels = kernels::getAllBenchmarkKernels();
    std::cout << "Testing " << syntheticBenchmarkKernels.size() << " standard kernels:\n\n";

    // Test each kernel
    for(const auto& kernel : syntheticBenchmarkKernels) 
    {
        std::cout << "Kernel: " << kernel.name << " (" << kernel.size << "x" << kernel.size << ")\n";
        
        // CPU (Single Thread)
        cv::Mat outCPU;
        double cpuTime = convolveCPU(imgSynthetic, kernel.data, kernel.size, outCPU);
        std::cout << "CPU:         " << cpuTime << " ms\n";
        
        // OMP (Multi-Threads) - test with different thread counts
        int threads[] = {1, 2, 4, 8, 16};
        for(int thread : threads) 
        {
            cv::Mat outOMP;
            double ompTime = convolveOMP(imgSynthetic, kernel.data, kernel.size, outOMP, thread);
            double speedup = cpuTime / ompTime;
            
            std::cout << "OMP(" << thread << "):      " << ompTime << " ms  (Speedup: " << speedup << "x)\n";
        }
        
        // CUDA (GPU)
        cv::Mat outCUDA;
        double cudaTime = convolveCUDA(imgSynthetic, kernel.data, kernel.size, outCUDA);
        double cudaSpeedup = cpuTime / cudaTime;
        std::cout << "CUDA:        " << cudaTime << " ms  (Speedup: " << cudaSpeedup << "x)\n";
        
        // Verify correctness
        bool cudaCorrect = utils::imagesNearEqual(outCPU, outCUDA, 1e-3);
        std::cout << "CUDA correctness: " << (cudaCorrect ? "PASS" : "FAIL") << "\n";
        std::cout << "\n";
    }

    // Large Resolution Image
    // Load Image 
    cv::Mat imgLarge = utils::loadImage("/image/large/city_4k.jpg", cv::IMREAD_COLOR, CV_8U);
    if(imgLarge.empty())
    {
        std::cout << "Cannot load image!\n";
        return 1;
    }
    std::cout << "Image loaded (city_4k): " << imgLarge.cols << "x" << imgLarge.rows << "\n";

    // Test large kernels for GPU stress testing
    std::cout << "GPU Stress Test - Large Kernels:\n\n";
    
    auto largeKernels = kernels::getLargeKernelsForCUDA();
    
    for(const auto& kernel : largeKernels) 
    {
        std::cout << "Kernel: " << kernel.name << " (" << kernel.size << "x" << kernel.size << ")\n";
        
        // OMP with maximum thread (16)
        int threads = 16;;
        double ompBestTime = 0.0;
        cv::Mat outOMP;
        cv::Mat outOMP_temp;
        double ompTime = convolveOMP(imgLarge, kernel.data, kernel.size, outOMP_temp, threads);
        std::cout << "OMP(" << threads << "):      " << ompTime << " ms\n";
        ompBestTime = ompTime;
        outOMP = outOMP_temp;
      
        // CUDA (GPU)
        cv::Mat outCUDA;
        double cudaTime = convolveCUDA(imgLarge, kernel.data, kernel.size, outCUDA);
        double cudaVsOmpSpeedup = ompBestTime / cudaTime;
        std::cout << "CUDA:        " << cudaTime << " ms  (vs OMP(16): " << cudaVsOmpSpeedup << "x)\n";
        
        // Verify correctness (compare CUDA against OMP)
        bool cudaCorrect = utils::imagesNearEqual(outOMP, outCUDA, 1e-3);
        std::cout << "CUDA correctness: " << (cudaCorrect ? "PASS" : "FAIL") << "\n";
        std::cout << "\n";
    }

    std::cout << "Benchmark completed!\n";

    return 0;
}