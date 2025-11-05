# Parallel Image Convolution Benchmark
This project studies the computational performance of 2D image convolution (a fundamental operation in computer vision) under different parallelization strategies.

##💡Description
This project implements and benchmarks multiple approaches:
- Naive (single-threaded) convolution
- Parallelized convolution using OpenMP on a multi-core CPU
- GPU-accelerated version (CUDA)

##📈Research Objectives
- Quantify how parallelism affects runtime efficiency in image convolution.
- Identify the point of diminishing returns (where adding more threads stops improving performance).
- Evaluate the computational trade-offs between CPU parallelism and GPU acceleration.
- Provide insights useful for real-time computer vision applications (like filtering, edge detection, CNN preprocessing).

The project evaluates how much performance improvement can be achieved through parallel programming, and analyzes the trade-offs between speed, scalability, and computational cost.
