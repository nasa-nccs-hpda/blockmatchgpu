#include "BlockMatcherCPU.h"
#include <iostream>
#include <cmath>

BlockMatcherCPU::BlockMatcherCPU(int rows, int cols, int block_size, int search_range) {
    r = rows;
    c = cols;
    this->block_size = block_size;
    half_block_size = block_size / 2;
    this->search_range = search_range;
    disparity_map.resize(rows * cols, 0.0);
}

double BlockMatcherCPU::compute_sos(const std::vector<double>& kernelCutLeft,
                                    const std::vector<double>& kernelCutRight) {
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

double BlockMatcherCPU::compute_box_sum(const std::vector<double>& kernelCutLeft,
                        const std::vector<double>& kernelCutRight) {
    double box_sum = 0.0;

    // Iterate over the pixels in the cutouts and compute the box sum
    for (size_t i = 0; i < kernelCutLeft.size(); i++) {
        double diff = kernelCutLeft[i] - kernelCutRight[i];
        box_sum += std::abs(diff); // Absolute difference for the box sum
    }

    return box_sum;
}

void compute_disparity(const double* left_image, const double* right_image) {
    int max_displacement = search_range;

    for (int i = half_block_size; i < r - half_block_size; i++) {
        for (int j = half_block_size; j < c - half_block_size; j++) {
           
            // Do work
        }
    }
}
