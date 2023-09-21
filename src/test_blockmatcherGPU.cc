#include <iostream>
#include "BlockMatcherGPU.h" 

#include <nvToolsExt.h>
#include <nvToolsExtCuda.h>


int main() {
    int rows = 2000;
    int cols = 2000;
    int block_size = 21;
    int search_range = 20;

    // Create instances of the left and right image data (dummy data)
    std::vector<double> left_image(rows * cols, 0);
    std::vector<double> right_image(rows * cols, 0);

    // Populate the dummy data (e.g., random values for demonstration)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            left_image[i * cols + j] = static_cast<double>(rand() % 256);  // Random values between 0 and 255
            right_image[i * cols + j] = static_cast<double>(rand() % 256);
        }
    }

    // Create a BlockMatcherGPU instance
    BlockMatcherGPU blockMatchergpu(rows, cols, block_size, search_range);

    std::cout << "ROWS" << " ";
    std::cout << rows << " ";
    std::cout << cols << "\n";
    // Compute the disparity map using the dummy data
    blockMatchergpu.compute_disparity(left_image, right_image);

    // Access the disparity map (disparity values are stored in disparityProcessor.disparity_map)
    std::vector<double> disparity_map = blockMatchergpu.disparity_map;

    std::cout << disparity_map[0] << " DISPARITY_MAP_GPU " << " \n";

    return 0;
}