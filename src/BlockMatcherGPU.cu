#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <nvToolsExt.h>
#include <nvToolsExtCuda.h>
#include "timer.h"
#include "BlockMatcherGPU.h"
#include <iostream>
#include <cmath>

timer t0;

#define NVTX_START(name) nvtxRangePushA(name)
#define NVTX_STOP() nvtxRangePop()

using namespace std;


BlockMatcherGPU::BlockMatcherGPU(int rows, int cols, int block_size, int search_range) {
    r = rows;
    c = cols;
    this->block_size = block_size;
    half_block_size = block_size / 2;
    this->search_range = search_range;
    disparity_map.resize(rows * cols, 0.0);
}

double BlockMatcherGPU::compute_sos(
            const std::vector<double>& kernelCutLeft,
            const std::vector<double>& kernelCutRight
) {

    // return if value is over the max
    if (kernelCutLeft.size() != kernelCutRight.size()) {
        return std::numeric_limits<double>::max();
    }

    double sum_of_squares = 0.0;

    // Iterate over the pixels in the cutouts and compute the Sum of Squares.
    for (size_t i = 0; i < kernelCutLeft.size(); i++) {
        double diff = kernelCutLeft[i] - kernelCutRight[i];
        sum_of_squares += diff * diff;
    }

    return sum_of_squares;
}

__global__ void gpu_sum_of_squares(double* kernel_cut_left, double* kernel_cut_right, int kernel_size, float*  global_sum_of_squares) 
{

    int thread_id = blockIdx.x * blockDim.x + threadIdx.x;
    int nthreads = gridDim.x * blockDim.x;

    double sum_of_squares = 0.0;

    // Iterate over the pixels in the cutouts and compute the Sum of Squares.
    for (size_t i = thread_id; i < kernel_size; i += nthreads) {
        double diff = kernel_cut_left[i] - kernel_cut_right[i];
        sum_of_squares += diff * diff;
    }

    // reduction process
    atomicAdd(global_sum_of_squares, (float)sum_of_squares);

}

double BlockMatcherGPU::compute_sos_gpu(
            const std::vector<double>& kernelCutLeft,
            const std::vector<double>& kernelCutRight
) {

    // return if value is over the max
    if (kernelCutLeft.size() != kernelCutRight.size()) {
        return std::numeric_limits<double>::max();
    }

    // defaults from Lars, set via function once we get the GPU kernel working    
    int rows = 1000;
    int cols = 1000;
    dim3 threads = {1024};
    dim3 blocks;

    NVTX_START("Initializing Cuda inside compute Disparity");
    cudaFree(0);
    int deviceID;
    cudaDeviceProp prop;
    cudaGetDevice(&deviceID);
    cudaGetDeviceProperties(&prop, deviceID); 
    blocks = {(unsigned)prop.multiProcessorCount};
    NVTX_STOP();

    // overall variables to fill in
    double sum_of_squares = 0.0;
    float* global_sum_of_squares;

    NVTX_START("Allocating and copying GPU memory");
    //cout << "Allocating and copying GPU memory\n";
    
    double* kernelCutLeft_device;
    double* kernelCutRight_device;
    float* global_sum_of_squares_device;

    //cudaMallocManaged(&p, kernelCutRight.size()*sizeof(int));
    cudaMallocManaged(&kernelCutLeft_device, kernelCutRight.size()*sizeof(double));
    cudaMallocManaged(&kernelCutRight_device, kernelCutRight.size()*sizeof(double));
    cudaMallocManaged(&global_sum_of_squares_device, sizeof(float));

    cudaError_t err = cudaGetLastError();
    if (err) {
        printf("GPU Error: %s\n", cudaGetErrorString(err));
    }

    //cout << "Done with cudaMallocManaged\n";
    //cout << "Size " << kernelCutRight.size()<< "\n";

    // add the values to the GPU - kernelCutLeft, kernelCutRight
    for (int j = 0; j < kernelCutRight.size(); j++) {
        //cout << j << "\n";
        kernelCutLeft_device[j] = kernelCutLeft[j];
        kernelCutRight_device[j] = kernelCutRight[j];
    }
    NVTX_STOP();

    //cout << "Done with cudaMallocManaged assignments\n";

    // initialize global_sum_of_squares
    *global_sum_of_squares_device = 0.0;

    //cout << "Starting GPU processing\n";

    NVTX_START("GPU Exec");
    t0.tick();
    gpu_sum_of_squares<<<blocks, threads>>>(kernelCutLeft_device, kernelCutRight_device, kernelCutRight.size(), global_sum_of_squares_device);
    cudaDeviceSynchronize();

    err = cudaGetLastError();
    if (err) {
        printf("GPU Error: %s\n", cudaGetErrorString(err));
    }

    double dt = t0.tock();
    NVTX_STOP();

    //cout << "Finished GPU processing, returning global sum of squares\n";

    //cudaFree(kernelCutLeft_device);
    //cudaFree(kernelCutRight_device);
    //cudaFree(global_sum_of_squares_device);

    return *global_sum_of_squares_device;
}

double BlockMatcherGPU::compute_box_sum(const std::vector<double>& kernelCutLeft,
                        const std::vector<double>& kernelCutRight) {
    double box_sum = 0.0;

    // Iterate over the pixels in the cutouts and compute the box sum
    for (size_t i = 0; i < kernelCutLeft.size(); i++) {
        double diff = kernelCutLeft[i] - kernelCutRight[i];
        box_sum += std::abs(diff); // Absolute difference for the box sum
    }

    return box_sum;
}

void BlockMatcherGPU::compute_disparity(const std::vector<double>& left_image, const std::vector<double>& right_image) {

    int max_displacement = search_range;

    for (int i = half_block_size; i < r - half_block_size; i++) {
        for (int j = half_block_size; j < c - half_block_size; j++) {
            
            // Take a cutout centered on each pixel in the left image
            std::vector<double> kernelCutLeft(block_size * block_size, 0.0);
            for (int y = -half_block_size; y <= half_block_size; y++) {
                for (int x = -half_block_size; x <= half_block_size; x++) {
                    kernelCutLeft[(y + half_block_size) * block_size + (x + half_block_size)] = left_image[(i + y) * c + (j + x)];
                }
            }

            int min_disparity = 0;
            double min_sos = std::numeric_limits<double>::max();
            // std::cout << i << " ";
            // std::cout << std::endl;

            // Search within the specified search range
            for (int d = 0; d <= max_displacement; d++) {
                // std::cout << "Searching disparity range" << d << " ";
                // std::cout << std::endl;
                // Shift the right image by the current disparity
                std::vector<double> kernelCutRight(block_size * block_size, 0.0);
                for (int y = -half_block_size; y <= half_block_size; y++) {
                    for (int x = -half_block_size; x <= half_block_size; x++) {
                        kernelCutRight[(y + half_block_size) * block_size + (x + half_block_size)] = right_image[(i + y) * c + (j + x - d)];
                    }
                }

                // Compute the Sum of Squares (SOS) between the cutout and the matching cutout
                //double sos = compute_sos(kernelCutLeft, kernelCutRight);
                //cout << "Sum of squares output " << sos << " \n";

                // call the global function here
                //cout << "Calling compute_sos_gpu\n";
                //cout << "Left side kernel " << kernelCutLeft.size() << "\n";
                //cout << "Right side kernel " << kernelCutRight.size() << "\n";

                double sos = compute_sos_gpu(kernelCutLeft, kernelCutRight);

                //cout << "Done with compute_sos_gpu\n";

                // for loop to sum the values

                // Update the disparity if the SOS is smaller
                if (sos < min_sos) {
                    min_sos = sos;
                    min_disparity = d;
                }
            }

            // Store the disparity in the disparity map
            disparity_map[i * c + j] = min_disparity;
        }
    }
}

std::vector<double>& BlockMatcherGPU::getDisparityMap() {
    return disparity_map;
}
