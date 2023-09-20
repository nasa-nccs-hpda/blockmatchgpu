#include "stereoCorrelator.h"
#include "BlockMatcherGPU.h"

#include <iostream>
#include <string>
#include <vector>
#include <gdal.h>
#include <gdal_priv.h>

StereoCorrelator::StereoCorrelator(const char* leftImagePath,
                                    const char* rightImagePath,
                                    int block_size,
                                    int search_range,
                                    const char* outputImagePath)
    : leftImagePath(leftImagePath), rightImagePath(rightImagePath), block_size(block_size),
      search_range(search_range), outputImagePath(outputImagePath) {}

void StereoCorrelator::calculateDisparityMap(
    const char* leftImagePath,
    const char* rightImagePath,
    const char* outputImagePath
)
{
    GDALAllRegister(); // Register GDAL drivers

    // Open the left and right images using GDAL
    GDALDataset *leftImageDataset = static_cast<GDALDataset *>(GDALOpen(leftImagePath, GA_ReadOnly));
    GDALDataset *rightImageDataset = static_cast<GDALDataset *>(GDALOpen(rightImagePath, GA_ReadOnly));

    if (!leftImageDataset || !rightImageDataset)
    {
        std::cerr << "Error: Unable to open input images using GDAL." << std::endl;
        GDALClose(leftImageDataset);
        GDALClose(rightImageDataset);
        return;
    }

    int cols = leftImageDataset->GetRasterXSize();
    int rows = leftImageDataset->GetRasterYSize();

    std::vector<std::vector<double>> left_image(rows, std::vector<double>(cols));
    std::vector<std::vector<double>> right_image(rows, std::vector<double>(cols));

    leftImageDataset->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, cols, rows, left_image.data(), cols, rows, GDT_Float32, 0, 0);
    rightImageDataset->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, cols, rows, right_image.data(), cols, rows, GDT_Float32, 0, 0);

    GDALClose(leftImageDataset);
    GDALClose(rightImageDataset);

    BlockMatcherGPU blockmatching(rows, cols, block_size, search_range);

    // Compute the disparity map
    blockmatching.compute_disparity(left_image, right_image);

    // Get the disparity map
    //const std::vector<std::vector<double>> &disparity_map = stereoProcessor.getDisparityMap();

    // Create a GDAL dataset for the output disparity map
    //GDALDriver *driver = GetGDALDriverManager()->GetDriverByName("GTiff");
    //GDALDataset *disparityDataset = driver->Create(outputImagePath.c_str(), cols, rows, 1, GDT_Float64, nullptr);

    // Write disparity map data to the GDAL dataset
    //disparityDataset->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, cols, rows, disparity_map.data(), cols, rows, GDT_Float64, 0, 0);

    // Close the GDAL dataset
    //GDALClose(disparityDataset);
}
