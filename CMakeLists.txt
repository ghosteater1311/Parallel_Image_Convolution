cmake_minimum_required(VERSION 3.10)

# CUDA 12.3 officially supports VS 2017-2022
set(CMAKE_CUDA_COMPILER "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.3/bin/nvcc.exe")
set(CUDAToolkit_ROOT "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.3")

project(ParallelConvolution LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_ARCHITECTURES 75)  # GTX 1650 is compute capability 7.5

set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")
set(OpenCV_STATIC OFF)

find_package(CUDA REQUIRED)
find_package(OpenMP REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${CUDA_INCLUDE_DIRS} include)

add_executable(Parallel_Image_Convolution_Benchmark
    src/main.cpp
    src/convolutionCpu.cpp
    src/convolutionCuda.cu
    src/convolutionOmp.cpp
)

target_link_libraries(Parallel_Image_Convolution_Benchmark
    ${OpenCV_LIBS}
    ${CUDA_LIBRARIES}
    OpenMP::OpenMP_CXX
)