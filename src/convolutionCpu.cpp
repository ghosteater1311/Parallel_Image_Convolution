#include <opencv2/opencv.hpp>
#include <vector>
#include <stdexcept>
#include "convolution.hpp"
#include "utils.hpp"
#include "timer.hpp"

double convolveCPU(const cv::Mat& input, const std::vector<float>& kernel, int kernelSize, cv::Mat& output)
{
    if(!utils::isValidKernel(kernel, kernelSize)) 
    {
        throw std::invalid_argument("Invalid kernel: size must be positive odd and match kernel vector length!");
    }

    cv::Mat in = utils::convertToFloat(input);
    if(in.empty()) 
    {
        throw std::invalid_argument("Input image is empty or could not be converted to float!");
    }

    utils::ensureOutputLike(in, output, CV_32F);
    const int H = in.rows;
    const int W = in.cols;
    const int C = in.channels();
    const int half = kernelSize / 2;

    const float* kp = kernel.data();

    // Timing the whole convolution operation
    timer::ScopedTimer st;

    for(int y = 0; y < H; y++) 
    {
        float* outRow = output.ptr<float>(y);
        for(int x = 0; x < W; x++) 
        {
            for(int c = 0; c < C; c++) 
            {
                float sum = 0.0f;

                for(int ky = -half; ky <= half; ky++) 
                {
                    int iy = y + ky;
                    if (iy < 0 || iy >= H) continue; // zero-padding

                    const float* inRow = in.ptr<float>(iy);
                    int krow = (ky + half) * kernelSize;

                    for(int kx = -half; kx <= half; kx++) 
                    {
                        int ix = x + kx;
                        if(ix < 0 || ix >= W) continue;
                        float val = inRow[ix * C + c];
                        float kval = kp[krow + (kx + half)];
                        sum += val * kval;
                    }
                }

                outRow[x * C + c] = sum;
            }
        }
    }

    return st.elapsedMs();
}