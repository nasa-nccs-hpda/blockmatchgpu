#include "BlockMatcherCPU.h"
#include <iostream>
#include <cmath>
#include "timer.h"
timer t1;

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

void BlockMatcherCPU::compute_disparity(const std::vector<double>& left_image, const std::vector<double>& right_image) {
    t1.tick();
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
                        // int idxRight = (i + y) * c + (j + x + d);
                        kernelCutRight[(y + half_block_size) * block_size + (x + half_block_size)] = right_image[(i + y) * c + (j + x + d)];
                    }
                }

                // Compute the Sum of Squares (SOS) between the cutout and the matching cutout
                double sos = compute_sos(kernelCutLeft, kernelCutRight);

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
    double dt = t1.tock();
    printf("[CPU] Time: %f\n", dt);
    printf("[CPU] GB: %f\n", r*c*sizeof(double)/1e9);
    printf("[CPU] GB/s: %f\n", r*c*sizeof(double)/dt/1e9);
}

std::vector<double>& BlockMatcherCPU::getDisparityMap() {
    return disparity_map;
}
