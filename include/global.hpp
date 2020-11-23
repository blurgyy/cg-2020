#ifndef GLOBAL_HPP
#define GLOBAL_HPP

// message functions `debugm` (debug messages), `errorm` (error messages)
// Reference: https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html
#ifndef NDEBUG
#define debugm(fmt, ...)                                                     \
    do {                                                                     \
        fprintf(stdout, " [*] %s::%d::%s(): " fmt, __FILE__, __LINE__,       \
                __func__, ##__VA_ARGS__);                                    \
    } while (0)
#endif
#define errorm(fmt, ...)                                                     \
    do {                                                                     \
        fprintf(stderr, " [X] %s::%d::%s(): " fmt, __FILE__, __LINE__,       \
                __func__, ##__VA_ARGS__);                                    \
    } while (0)

#include <string>

#include <opencv2/opencv.hpp>

// Write cv::Mat image data to a ppm file.
// Reference:
//  1. https://rosettacode.org/wiki/Bitmap/Write_a_PPM_file#C.2B.2B
//  2. http://netpbm.sourceforge.net/doc/ppm.html#format
// @param filename: name of the ppm image file
// @param data: image data (of type CV_8UC3)
void write_ppm(std::string const &filename, cv::Mat const &data);

#endif

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 18 2020, 17:36 [CST]
