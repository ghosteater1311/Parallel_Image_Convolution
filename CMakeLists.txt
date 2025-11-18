cmake_minimum_required(VERSION 3.10)
project(ParallelConvolution)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable Qt GUI components for OpenCV
set(OpenCV_STATIC OFF)

find_package(OpenMP REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(Parallel_Image_Convolution_Benchmark
    src/main.cpp
    src/convolution.cpp
    src/utils.cpp
)

target_link_libraries(Parallel_Image_Convolution_Benchmark
    ${OpenCV_LIBS}
    OpenMP::OpenMP_CXX
)