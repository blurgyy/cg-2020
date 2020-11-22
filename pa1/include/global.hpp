#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <string>

#include <opencv2/opencv.hpp>

// Write cv::Mat image data to a ppm file.
// Reference:
//  1. https://rosettacode.org/wiki/Bitmap/Write_a_PPM_file#C.2B.2B
//  2. http://netpbm.sourceforge.net/doc/ppm.html#format
// @param filename: name of the ppm image file
// @param data: image data (of type CV_8UC3)
void write_ppm(std::string const &filename, cv::Mat data);

#endif

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 18 2020, 17:36 [CST]
