#ifndef BLOCKMATCHERGPU_H
#define BLOCKMATCHERGPU_H

#include <vector>

class BlockMatcherGPU {
public:
    int r, c, block_size, half_block_size, search_range;
    std::vector<std::vector<double>> disparity_map;
    BlockMatcherGPU(int rows, int c, int block_size, int search_range);

    void compute_disparity(const std::vector<std::vector<double>>& left_image, const std::vector<std::vector<double>>& right_image);

    const std::vector<std::vector<double>>& getDisparityMap() const;
private:
    double compute_box_sum(const std::vector<std::vector<double>>& kernelCutLeft,
                           const std::vector<std::vector<double>>& kernelCutRight);
    double compute_sos(const std::vector<std::vector<double>>& kernelCutLeft,
                           const std::vector<std::vector<double>>& kernelCutRight);
};

#endif // BlockMatcherGPU_H