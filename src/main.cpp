#include <iostream>
#include <omp.h>
#include <opencv2/opencv.hpp>

int main() 
{
    std::cout << "=== Parallel Image Convolution Benchmark ===" << std::endl;
    std::cout << std::endl;

    // --- OpenMP test ---
    std::cout << "Testing OpenMP..." << std::endl;
    std::cout << "Maximum threads available: " << omp_get_max_threads() << std::endl;
    
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int total = omp_get_num_threads();
        #pragma omp critical
        std::cout << "  Thread " << id << " of " << total << " reporting" << std::endl;
    }
    std::cout << "OpenMP test completed!" << std::endl;
    std::cout << std::endl;

    // --- OpenCV test ---
    std::cout << "Testing OpenCV..." << std::endl;
    std::cout << "OpenCV Version: " << CV_VERSION << std::endl;
    
    // Create a simple test image
    cv::Mat img = cv::Mat::zeros(100, 100, CV_8UC3);
    std::cout << "Created test image: " << img.cols << "x" << img.rows << std::endl;
    
    // Add some content to the image (white rectangle)
    cv::rectangle(img, cv::Point(10, 10), cv::Point(90, 90), cv::Scalar(255, 255, 255), -1);
    
    // Add text
    cv::putText(img, "Test Image", cv::Point(15, 55), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
    
    // Save the image
    std::string output_path = "test_image.png";
    bool success = cv::imwrite(output_path, img);
    if (success) {
        std::cout << "Test image saved to: " << output_path << std::endl;
    } else {
        std::cout << "Failed to save test image!" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "All tests completed successfully!" << std::endl;
    return 0;
}
