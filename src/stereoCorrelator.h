#ifndef STEREOCORRELATOR_H
#define STEREOCORRELATOR_H

class StereoCorrelator{
public:
    StereoCorrelator(const char* leftImagePath, const char* rightImagePath,
                        int block_size, int search_range, const char* outputImagePath);

    void calculateDisparityMap(
        const char* leftImagePath,
        const char* rightImagePath,
        const char* outputImagePath
    );

private:
    const char* leftImagePath;
    const char* rightImagePath;
    int block_size;
    int search_range;
    const char* outputImagePath;
};


#endif