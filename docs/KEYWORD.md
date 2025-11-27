# KEYWORDS - Image Processing Glossary

## Basic Concepts

### Noise
Unwanted random variations in pixel values. Like "static" on old TVs.
- **Sources**: sensor imperfection, low light, transmission errors
- **Types**: Gaussian noise (smooth), Salt & Pepper (black/white dots)

### Pixel
Smallest unit of an image. Has a value (0-255 for grayscale).

### Convolution
Sliding a kernel over an image, multiplying and summing at each position.

### Kernel (Filter)
A small matrix of weights used in convolution. Determines the effect (blur, edge, sharpen).

### Edge
Boundary between two regions with different intensities. Where image "changes quickly."

### Blur
Smoothing an image by averaging nearby pixels. Reduces noise but loses detail.

### Sharpen
Enhancing edges to make image look "crisper."

---

## Filter Types

### Low-Pass Filter
Allows low frequencies (smooth areas), blocks high frequencies (edges, noise).
- Example: Gaussian blur, Box filter

### High-Pass Filter
Allows high frequencies (edges, details), blocks low frequencies (smooth areas).
- Example: Laplacian, Sobel

### Band-Pass Filter
Allows only a specific range of frequencies.
- Example: LoG (Laplacian of Gaussian)

---

## Mathematical Terms

### Derivative
Rate of change. First derivative = slope. Second derivative = curvature.

### Gradient
Direction and magnitude of steepest change. Used for edge detection.

### Normalization
Scaling values to a standard range (e.g., sum to 1, or range 0-255).

### Separable
A 2D kernel that can be split into two 1D kernels for faster computation.

---

## Connectivity

### 4-Connectivity
Only direct neighbors (up, down, left, right) are considered "connected."

### 8-Connectivity
All 8 surrounding pixels (including diagonals) are considered "connected."

---

## Image Properties

### Intensity
Brightness value of a pixel (0 = black, 255 = white for 8-bit).

### Frequency (in images)
How fast pixel values change spatially.
- **Low frequency**: Smooth, slowly changing regions
- **High frequency**: Edges, textures, noise

### Resolution
Number of pixels in an image (width × height).

---

## Performance Terms

### Parallelization
Splitting work across multiple processors to run simultaneously.

### Throughput
Amount of data processed per unit time (e.g., megapixels/second).

### Latency
Time delay from input to output.

### Memory Bandwidth
Rate at which data can be read/written to memory.

### Cache
Fast, small memory close to CPU. Accessing cached data is much faster.

---

## CUDA Terms

### Thread
Smallest unit of execution in GPU.

### Block
Group of threads that can share memory and synchronize.

### Grid
Collection of all blocks for a kernel launch.

### Shared Memory
Fast memory shared by threads in a block.

### Global Memory
Large but slow GPU memory accessible by all threads.

### Coalescing
When adjacent threads access adjacent memory locations (efficient).

---

*Add more keywords as you encounter them!*
