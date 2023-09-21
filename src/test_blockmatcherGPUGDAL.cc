#include <iostream>
#include "BlockMatcherGPU.h" 
#include <nvToolsExt.h>
#include <nvToolsExtCuda.h>
#include <gdal.h>
#include <gdal_priv.h>

int main(int argc, char** argv) {

    // set filename variables
    const char* leftImagePath;
    const char* rightImagePath;

    // set default block size and search range for benchmarking
    int block_size = 21;
    int search_range = 20;

    // if filename is specified, read that filename
	if (argc > 1) {
        // set image paths
		leftImagePath = argv[1];
        rightImagePath = argv[2];
    } else {
        std::cerr << "Error: Missing left and right raster paths" << std::endl;
        return 1;
    }

    // Register GDAL drivers
    GDALAllRegister();

    // Open the left and right images using GDAL
    GDALDataset *leftImageDataset = static_cast<GDALDataset *>(GDALOpen(leftImagePath, GA_ReadOnly));
    GDALDataset *rightImageDataset = static_cast<GDALDataset *>(GDALOpen(rightImagePath, GA_ReadOnly));

    if (!leftImageDataset || !rightImageDataset)
    {
        // unable to open image datasets
        std::cerr << "Error: Unable to open input images using GDAL." << std::endl;
        GDALClose(leftImageDataset);
        GDALClose(rightImageDataset);
        return 1;
    }

    // get number of columns and rows, left and right images have the same size
    int cols = leftImageDataset->GetRasterXSize();
    int rows = leftImageDataset->GetRasterYSize();

    // output some of the information
    std::cout << "RASTER ROWS" << " " << rows << " \n";
    std::cout << "RASTER COLS" << " " << cols << " \n";

    // define vectors
    std::vector<double> left_image(rows * cols);
    std::vector<double> right_image(rows * cols);

    // read the imagery and load the data into the vector dataset
    leftImageDataset->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, cols, rows, left_image.data(), cols, rows, GDT_Float32, 0, 0);
    rightImageDataset->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, cols, rows, right_image.data(), cols, rows, GDT_Float32, 0, 0);

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

    // print out the contents of the disparity_map

    return 0;
}