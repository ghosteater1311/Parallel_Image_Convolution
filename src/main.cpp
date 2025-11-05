#include <iostream>
#include <omp.h>
#include <opencv2/opencv.hpp>

int main() 
{
    // --- OpenMP test ---
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int total = omp_get_num_threads();
        #pragma omp critical
        std::cout << "Hello from thread " << id << " of " << total << std::endl;
    }

    // --- OpenCV test ---
    cv::Mat img = cv::Mat::zeros(400, 400, CV_8UC3);
    cv::putText(img, "OpenCV + OpenMP OK", {40, 200}, cv::FONT_HERSHEY_SIMPLEX, 0.8, {255, 255, 255}, 2);
    cv::imshow("Verification", img);
    cv::waitKey(0);

    return 0;
}
