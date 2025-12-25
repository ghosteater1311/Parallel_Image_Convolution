// Define utils.hpp
// Provides small helper functions used across the benchmark:
// - Image loading/saving
// - Type conversion (to float)
// - Output allocation helpers
// - Kernel validation and normalization
// - Image comparison metrics (MSE, max absolute diff)

#ifndef UTILS_HPP
#define UTILS_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <numeric>
#include <cmath>
#include <stdexcept>

namespace utils 
{
    // loadImage: Load an image from disk and convert to requested
    //            type (default: CV_32F, same number of channels)
    // Returns empty Mat on failure.
    inline cv::Mat loadImage(const std::string& path, int flags = cv::IMREAD_UNCHANGED, int outDepth = CV_32F)
    {
        cv::Mat img = cv::imread(path, flags);
        if(img.empty()) return cv::Mat();

        cv::Mat out;
        img.convertTo(out, outDepth);
        return out;
    }

    // saveImage: Save image to disk. Converts to 8-bit if needed.
    inline bool saveImage(const std::string& path, const cv::Mat& image)
    {
        if(image.empty()) return false;

        cv::Mat out;
        if(image.depth() != CV_8U) 
        {
            // Normalize values if float, then convert to 8-bit
            double minVal, maxVal;
            cv::minMaxLoc(image, &minVal, &maxVal);
            if(minVal == maxVal) 
            {
                image.convertTo(out, CV_8U);
            } 
            else 
            {
                // Scale to 0-255
                image.convertTo(out, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));
            }
        } 
        else 
        {
            out = image;
        }

        return cv::imwrite(path, out);
    }

    // convertToFloat: Convert image to floating point CV_32F (same channels)
    inline cv::Mat convertToFloat(const cv::Mat& src)
    {
        if(src.empty()) return cv::Mat();
        if(src.depth() == CV_32F) return src;
        cv::Mat out;
        src.convertTo(out, CV_32F);
        return out;
    }

    // ensureOutputLike: Make sure output Mat is allocated with the specified depth and same size/channels as input
    inline void ensureOutputLike(const cv::Mat& input, cv::Mat& output, int depth = CV_32F)
    {
        int desiredType = CV_MAKETYPE(depth, input.channels());
        if(output.empty() || output.rows != input.rows || output.cols != input.cols || output.type() != desiredType) 
        {
            output.create(input.size(), desiredType);
        }
    }

    // Kernel helpers
    inline bool isValidKernel(const std::vector<float>& kernel, int ksize)
    {
        return ksize > 0 && (ksize % 2 == 1) && ((int)kernel.size() == ksize * ksize);
    }

    inline void normalizeKernel(std::vector<float>& kernel)
    {
        if(kernel.empty()) return;
        float sum = std::accumulate(kernel.begin(), kernel.end(), 0.0f);
        if(std::abs(sum) < 1e-12f) return; // don't divide by zero
        for(auto& v : kernel) v /= sum;
    }

    // Image comparison metrics
    // Computes Mean Squared Error between two images (same size & channels)
    inline double computeMSE(const cv::Mat& a, const cv::Mat& b)
    {
        if(a.empty() || b.empty()) throw std::invalid_argument("Images must be non-empty");
        if(a.size() != b.size() || a.channels() != b.channels()) throw std::invalid_argument("Images must have same size and channels");

        cv::Mat af = convertToFloat(a);
        cv::Mat bf = convertToFloat(b);

        cv::Mat diff;
        cv::absdiff(af, bf, diff);
        cv::multiply(diff, diff, diff);

        // sum over all channels and pixels
        cv::Scalar s = cv::sum(diff);
        double ssum = 0.0;
        for (int i = 0; i < diff.channels(); ++i) ssum += s[i];

        double mse = ssum / (static_cast<double>(a.rows) * a.cols * a.channels());
        return mse;
    }

    // Computes maximum absolute difference between two images
    inline double computeMaxAbsDiff(const cv::Mat& a, const cv::Mat& b)
    {
        if (a.empty() || b.empty()) throw std::invalid_argument("Images must be non-empty");
        if (a.size() != b.size() || a.channels() != b.channels()) throw std::invalid_argument("Images must have same size and channels");

        cv::Mat af = convertToFloat(a);
        cv::Mat bf = convertToFloat(b);

        cv::Mat diff;
        cv::absdiff(af, bf, diff);

        std::vector<cv::Mat> planes;
        cv::split(diff, planes);

        double maxVal = 0.0;
        for(const auto& p : planes) 
        {
            double m; cv::minMaxLoc(p, nullptr, &m);
            if(m > maxVal) maxVal = m;
        }
        return maxVal;
    }

    // Convenience: compare images and return (maxDiff, mse) and whether within tolerance
    inline bool imagesNearEqual(const cv::Mat& a, const cv::Mat& b, double tol = 1e-3, double* outMaxDiff = nullptr, double* outMSE = nullptr)
    {
        double mse = computeMSE(a, b);
        double maxd = computeMaxAbsDiff(a, b);
        if(outMaxDiff) *outMaxDiff = maxd;
        if(outMSE) *outMSE = mse;
        return (maxd <= tol);
    }
} 

#endif
