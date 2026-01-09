#include <iostream>
#include <opencv2/opencv.hpp>
#include "convolution.hpp"
#include "kernels.hpp"
#include "utils.hpp"

int main()
{
    // Load Image (absolute path for now)
    cv::Mat img = utils::loadImage("d:/HUST/GHWorkspace/Parallel_Image_Convolution_Benchmark/image/standard/lena_512.png", cv::IMREAD_COLOR, CV_8U);
    if(img.empty())
    {
        std::cout << "Cannot load image!\n";
        return 1;
    }
    std::cout << "Image loaded:" << img.cols << "x" << img.rows << "\n";

    // Create Kernels for test
    // Box Filter
    // auto BoxFilter = kernels::createBoxFilter(3);
    // std::cout << "Kernel: " << BoxFilter.name << " (" << BoxFilter.size << "x" << BoxFilter.size << ")\n";

    // Gaussian Filter
    auto GaussianFilter = kernels::createGaussianFilter(5);
    std::cout << "Kernel: " << GaussianFilter.name << " (" << GaussianFilter.size << "x" << GaussianFilter.size << ")\n";

    // Sobel X&Y
    auto SobelX = kernels::createSobelX();
    auto SobelY = kernels::createSobelY();
    std::cout << "Kernel: " << "Sobel X&Y" << " (" << SobelX.size << "x" << SobelY.size << ")\n";

    // Laplace 4-connect
    auto Laplace4ConFilter = kernels::createLaplacian4();
    std::cout << "Kernel: " << Laplace4ConFilter.name << " (" << Laplace4ConFilter.size << "x" << Laplace4ConFilter.size << ")\n";


    // Laplace 8-connect
    auto Laplace8ConFilter = kernels::createLaplacian8();
    std::cout << "Kernel: " << Laplace8ConFilter.name << " (" << Laplace8ConFilter.size << "x" << Laplace8ConFilter.size << ")\n";

    // LoG Filter
    auto LoGFilter = kernels::createLoG(9, -1.0F);
    std::cout << "Kernel: " << LoGFilter.name << " (" << LoGFilter.size << "x" << LoGFilter.size << ")\n";

    // Sharpending Filter
    auto SharpendingFilter = kernels::createSharpen();
    std::cout << "Kernel: " << SharpendingFilter.name << " (" << SharpendingFilter.size << "x" << SharpendingFilter.size << ")\n";

    // Convolve
    int sizes[] = {3, 5, 9, 15};
    for(int ksize : sizes) 
    {   
        auto BoxFilter = kernels::createBoxFilter(ksize);
        std::cout << "Kernel: " << BoxFilter.name << " (" << BoxFilter.size << "x" << BoxFilter.size << ")\n";
        std::cout << "Kernel: " << ksize << "x" << ksize << "\n";
        
        // CPU (Single Thread)
        cv::Mat outCPU;
        double cpuTime = convolveCPU(img, BoxFilter.data, BoxFilter.size, outCPU);
        std::cout << "CPU:     " << cpuTime << " ms\n";
        
        // OMP (Multi-Threads)
        int threads[] = {1, 2, 4, 8, 16};
        for(int thread : threads) 
        {
            cv::Mat outOMP;
            double ompTime = convolveOMP(img, BoxFilter.data, BoxFilter.size, outOMP, thread);
            double speedup = cpuTime / ompTime;
            
            std::cout << "OMP(" << thread << "):     " << ompTime << " ms  (Speedup: " << speedup << "x)\n";
        }
        
        // CUDA (GPU)
        cv::Mat outCUDA;
        double cudaTime = convolveCUDA(img, BoxFilter.data, BoxFilter.size, outCUDA);
        double cudaSpeedup = cpuTime / cudaTime;
        std::cout << "CUDA:    " << cudaTime << " ms  (Speedup: " << cudaSpeedup << "x)\n";
        
        // Verify correctness
        bool cudaCorrect = utils::imagesNearEqual(outCPU, outCUDA, 1e-3);
        std::cout << "CUDA correctness: " << (cudaCorrect ? "PASS" : "FAIL") << "\n";
        std::cout << "\n";
    }

    return 0;
}