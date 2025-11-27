// Define Convolution Kernels
#ifndef KERNELS_HPP
#define KERNELS_HPP

// Stadard library for calculate & store Kernels
#include <vector>
#include <cmath>
#include <string>

// Define Pi constant
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Define namespace for each Kernels
namespace kernels
{
    struct Kernel
    {
        std::string name;
        std:: vector<float> data;
        int size;
        bool separable;
        std::vector<float> separableX;
        std::vector<float> separableY;
        
        Kernel() : size(0) , separable(false) {}
        Kernel(const std::string& n, const std::vector<float>& d, int s, bool sep = false)
            : name(n), data(d), size(s), separable(sep) {}
    };

    // Define Box Filter (Mean Filter) (can be act like Low-pass filter)
    inline Kernel createBoxFilter(int size = 3)
    {
        std::vector<float> data(size * size);
        float value = 1.0f / (size * size);

        for(int i = 0; i < size * size; i++)
        {
            data[i] = value;
        }

        Kernel k("Box " + std::to_string(size) + "x" + std::to_string(size), data, size, true);
        k.separableX.resize(size, 1.0f / size);
        k.separableY.resize(size, 1.0f);

        return k;
    }

    // Define Gaussian Filter (can be act like Low-pass filter)
    inline Kernel createGaussianFilter(int size = 3, float sigma = -1.0f) 
    {
        if(sigma < 0) 
        {
            sigma = 0.3f * ((size - 1) * 0.5f - 1) + 0.8f;
        }

        std::vector<float> data(size * size);
        int half = size / 2;
        float sum = 0.0f;

        for(int y = -half; y <= half; y++) 
        {
            for(int x = -half; x <= half; x++) 
            {
                float value = std::exp(-(x * x + y * y) / (2.0f * sigma * sigma));
                data[(y + half) * size + (x + half)] = value;
                sum += value;
            }
        }

        for(int i = 0; i < size * size; i++) 
        {
            data[i] /= sum;
        }

        Kernel k("Gaussian " + std::to_string(size) + "x" + std::to_string(size), data, size, true);

        k.separableX.resize(size);
        k.separableY.resize(size);
        float sum1d = 0.0f;
        for(int i = -half; i <= half; i++) 
        {
            float val = std::exp(-(i*i) / (2.0f * sigma * sigma));
            k.separableX[i + half] = val;
            sum1d += val;
        }

        for(int i = 0; i < size; i++) 
        {
            k.separableX[i] /= sum1d;
            k.separableY[i] = k.separableX[i];
        }

        return k;
    }

    // Define Sobel X & Y filter
    inline Kernel createSobelX() 
    {
        std::vector<float> data = {
            -1, 0, 1,
            -2, 0, 2,
            -1, 0, 1
        };

        Kernel k("Sobel X", data, 3, true);
        k.separableX = {-1, 0, 1};
        k.separableY = {1, 2, 1};

        return k;
    }

    inline Kernel createSobelY() 
    {
        std::vector<float> data = {
            -1, -2, -1,
             0,  0,  0,
             1,  2,  1
        };

        Kernel k("Sobel Y", data, 3, true);
        k.separableX = {1, 2, 1};
        k.separableY = {-1, 0, 1};

        return k;
    }

    // Define Laplace filter
    // Creates 4-connectivity Laplacian (only direct neighbors)
    inline Kernel createLaplacian4() 
    {
        Kernel k;
        k.name = "Laplacian_4conn";
        k.size = 3;
        k.separable = false;
        k.data = {
             0, -1,  0,
            -1,  4, -1,
             0, -1,  0
        };
        return k;
    }

    // Creates 8-connectivity Laplacian (includes diagonal neighbors)
    inline Kernel createLaplacian8() 
    {
        Kernel k;
        k.name = "Laplacian_8conn";
        k.size = 3;
        k.separable = false;
        k.data = {
            -1, -1, -1,
            -1,  8, -1,
            -1, -1, -1
        };
        return k;
    }

    // Define Laplacian of Gaussian (LoG) filter 
    inline Kernel createLoG(int size = 9, float sigma = -1.0f) 
    {
        if(sigma < 0) 
        {
            sigma = size / 6.0f;  // Auto-calculate: size ≈ 6σ
        }

        std::vector<float> data(size * size);
        int half = size / 2;
        float sum = 0.0f;

        for(int y = -half; y <= half; y++) 
        {
            for(int x = -half; x <= half; x++) 
            {
                float r2 = x * x + y * y;
                float sigma2 = sigma * sigma;
                // LoG formula: (r² - 2σ²) / σ⁴ × e^(-r²/2σ²)
                float value = (r2 - 2 * sigma2) / (sigma2 * sigma2) * std::exp(-r2 / (2 * sigma2));
                data[(y + half) * size + (x + half)] = value;
                sum += value;
            }
        }

        // Normalize to zero-sum (important for edge detector)
        float mean = sum / (size * size);
        for(int i = 0; i < size * size; i++) 
        {
            data[i] -= mean;
        }

        Kernel k("LoG " + std::to_string(size) + "x" + std::to_string(size), data, size, false);
        return k;
    }

    // Define Sharpening filter - enhances edges
    inline Kernel createSharpen() 
    {
        Kernel k;
        k.name = "Sharpen";
        k.size = 3;
        k.separable = false;
        k.data = {
             0, -1,  0,
            -1,  5, -1,
             0, -1,  0
        };
        return k;
    }
    
    // Define High-pass filter - removes low frequencies, keeps edges
    inline Kernel createHighPass()
    {
        Kernel k;
        k.name = "High-pass";
        k.size = 3;
        k.separable = false;
        k.data = {
            -1, -2, -1,
            -2, 12, -2,
            -1, -2, -1
        };
        return k;
    }

    // Define Gabor filter - texture/pattern detection
    inline Kernel createGabor(int size = 11, float sigma = 3.0f, float theta = 0.0f, float lambda = 4.0f, float gamma = 0.5f, float psi = 0.0f) 
    {   
        std::vector<float> data(size * size);
        int half = size / 2;
        
        float cosTheta = std::cos(theta);
        float sinTheta = std::sin(theta);
        
        for(int y = -half; y <= half; y++) 
        {
            for(int x = -half; x <= half; x++) 
            {
                // Rotate coordinates by theta
                float xPrime = x * cosTheta + y * sinTheta;
                float yPrime = -x * sinTheta + y * cosTheta;

                // Gaussian envelope
                float gaussian = std::exp(-(xPrime * xPrime + gamma * gamma * yPrime * yPrime) / (2.0f * sigma * sigma));

                // Sinusoidal wave
                float sinusoid = std::cos(2.0f * M_PI * xPrime / lambda + psi);

                // Gabor = Gaussian × Sinusoid
                data[(y + half) * size + (x + half)] = gaussian * sinusoid;
            }
        }

        Kernel k("Gabor " + std::to_string(size) + "x" + std::to_string(size), data, size, false);
        return k;
    }

    // Define Large-scale Kernels filter (For GPU Stress test)
    // Large Box filter
    inline Kernel createLargeBoxFilter(int size = 31) 
    {
        std::vector<float> data(size * size);
        float value = 1.0f / (size * size);

        for(int i = 0; i < size * size; i++) 
        {
            data[i] = value;
        }

        Kernel k("Large Box " + std::to_string(size) + "x" + std::to_string(size), data, size, true);
        k.separableX.resize(size, 1.0f / size);
        k.separableY.resize(size, 1.0f);

        return k;
    }

    // Large Gaussian filter - for GPU stress testing  
    inline Kernel createLargeGaussianFilter(int size = 31, float sigma = -1.0f) 
    {
        if(sigma < 0) 
        {
            sigma = size / 6.0f;  // Auto: size ≈ 6σ
        }

        std::vector<float> data(size * size);
        int half = size / 2;
        float sum = 0.0f;

        for(int y = -half; y <= half; y++) 
        {
            for(int x = -half; x <= half; x++) 
            {
                float value = std::exp(-(x * x + y * y) / (2.0f * sigma * sigma));
                data[(y + half) * size + (x + half)] = value;
                sum += value;
            }
        }

        for(int i = 0; i < size * size; i++) 
        {
            data[i] /= sum;
        }

        Kernel k("Large Gaussian " + std::to_string(size) + "x" + std::to_string(size), data, size, true);

        // Build separable components
        k.separableX.resize(size);
        k.separableY.resize(size);
        float sum1d = 0.0f;
        for(int i = -half; i <= half; i++) 
        {
            float val = std::exp(-(i * i) / (2.0f * sigma * sigma));
            k.separableX[i + half] = val;
            sum1d += val;
        }

        for(int i = 0; i < size; i++) 
        {
            k.separableX[i] /= sum1d;
            k.separableY[i] = k.separableX[i];
        }

        return k;
    }

    // Define Utility Functions
    // Get all kernels for comprehensive benchmark
    inline std::vector<Kernel> getAllBenchmarkKernels() 
    {
        return 
        {
            createBoxFilter(3),
            createBoxFilter(5),
            createGaussianFilter(3),
            createGaussianFilter(5),
            createSobelX(),
            createSobelY(),
            createLaplacian4(),
            createLaplacian8(),
            createLoG(9),
            createSharpen(),
            createHighPass(),
            createGabor(11),
            createLargeBoxFilter(31),
            createLargeGaussianFilter(31)
        };
    }

    // Get small subset for quick testing
    inline std::vector<Kernel> getQuickTestKernels() 
    {
        return 
        {
            createBoxFilter(3),
            createGaussianFilter(3),
            createSobelX(),
            createLaplacian4()
        };
    }
}

#endif