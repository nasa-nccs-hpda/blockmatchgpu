#include <iostream>
#include "BlockMatcherGPU.h" 
#include <nvToolsExt.h>
#include <nvToolsExtCuda.h>
#include <string>
#include <vector>
#include <gdal.h>
#include <gdal_priv.h>

int main() {
    
    int rows = 1000;
    int cols = 1000;

    int block_size = 11;
    int search_range = 20;

    /*
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

    /*
    // Adding gdal components
    GDALAllRegister(); // Register GDAL drivers

    // set filenames

    // if filename is specified, read that filename
	if (argc > 1) {
		leftImagePath = argv[1];
        rightImagePath = argv[2];
	}

    // Open the left and right images using GDAL
    GDALDataset *leftImageDataset = static_cast<GDALDataset *>(GDALOpen("/gpfsm/dnb06/projects/p206/code/hackathon/test_cuda_function/aster-L.tif", GA_ReadOnly));
    GDALDataset *rightImageDataset = static_cast<GDALDataset *>(GDALOpen("/gpfsm/dnb06/projects/p206/code/hackathon/test_cuda_function/aster-R.tif", GA_ReadOnly));

    if (!leftImageDataset || !rightImageDataset)
    {
        std::cerr << "Error: Unable to open input images using GDAL." << std::endl;
        GDALClose(leftImageDataset);
        GDALClose(rightImageDataset);
        return 1;
    }

    int cols = leftImageDataset->GetRasterXSize();
    int rows = leftImageDataset->GetRasterYSize();

    // Create C-style arrays for left and right images
    double **left_image = new double *[rows];
    double **right_image = new double *[rows];

    std::cout << "Allocating\n" << " ";

    for (int i = 0; i < rows; ++i)
    {
        left_image[i] = new double[cols];
        right_image[i] = new double[cols];
    }

    std::cout << "Reading in\n" << " ";
    // Read data into the C-style arrays
    leftImageDataset->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, cols, rows, left_image, cols, rows, GDT_Float32, 0, 0);
    rightImageDataset->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, cols, rows, right_image, cols, rows, GDT_Float32, 0, 0);
    std::cout << "Done copying\n" << " ";
    GDALClose(leftImageDataset);
    GDALClose(rightImageDataset);
    std::cout << "Closed\n" << " ";
    */

    // Create a BlockMatcherGPU instance
    BlockMatcherGPU blockMatchergpu(rows, cols, block_size, search_range);

    std::cout << "ROWS" << " ";
    std::cout << rows << " ";
    std::cout << cols << "\n";
    // Compute the disparity map using the dummy data
    blockMatchergpu.compute_disparity(left_image, right_image);

    // Access the disparity map (disparity values are stored in disparityProcessor.disparity_map)
    std::vector<double> disparity_map = blockMatchergpu.disparity_map;

    return 0;
}