#include "global.hpp"

#include <cassert>
#include <fstream>

// Constants
flt const pi     = std::acos(-1.0);
flt const twopi  = pi * 2.0;
flt const halfpi = pi / 2.0;

void write_ppm(std::string const &filename, cv::Mat const &data) {
    // Only accepts 3-channel image
    assert(data.type() == CV_8UC3);

    std::ofstream f(filename, std::ios_base::out | std::ios_base::binary);
    char          ppm_head[50] = {0};
    int           height       = data.rows;
    int           width        = data.cols;
    // Magic number (P6), width, height, maximum color value,
    // seperated with whitespaces.
    sprintf(ppm_head, "P6\n%d %d\n255\n", width, height);
    f << ppm_head;

    for (int i = 0; i < height; ++i) {
        uchar const *row = data.ptr(i);
        for (int j = 0; j < width; j += 3) {
            // Note: cv::Mat with CV_8UC3 data type has channel order 'BGR'
            f << (char)(row[j + 2]) << (char)(row[j + 1]) << (char)(row[j]);
        }
    }
    f.close();
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 18 2020, 17:41 [CST]
