#include "global.hpp"

#include <cassert>
#include <fstream>

// Constants
flt const pi        = std::acos(-1.0);
flt const twopi     = pi * 2.0;
flt const halfpi    = pi / 2.0;
flt const piover180 = pi / 180.0;

// struct Color
Color::Color() : red(0), green(0), blue(0) {}
Color::Color(unsigned char const &r, unsigned char const &g,
             unsigned char const &b)
    : red(0), green(0), blue(0) {}
Color::Color(unsigned char const &x) : red(x), green(x), blue(x) {}
unsigned char &      Color::r() { return this->red; }
unsigned char &      Color::g() { return this->green; }
unsigned char &      Color::b() { return this->blue; }
unsigned char const &Color::r() const { return this->red; }
unsigned char const &Color::g() const { return this->green; }
unsigned char const &Color::b() const { return this->blue; }

// struct Image
Image::Image() {}
Image::Image(unsigned int const &height, unsigned int const &width)
    : h(height), w(width) {
    data = std::vector<Color>(h * w);
}
void Image::init(const unsigned int &height, const unsigned int &width) {
    this->h = height;
    this->w = width;
    data    = std::vector<Color>(h * w);
}
Color &Image::operator()(unsigned int const &x, unsigned int const &y) {
    return data[w * y + x];
}
Color const &Image::operator()(unsigned int const &x,
                               unsigned int const &y) const {
    return data[w * y + x];
}

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

void write_ppm(std::string const &filename, Image const &img) {
    std::ofstream f(filename, std::ios_base::out | std::ios_base::binary);
    char          ppm_head[50] = {0};
    int           height       = img.h;
    int           width        = img.w;
    // Magic number (P6), width, height, maximum color value,
    // seperated with whitespaces.
    sprintf(ppm_head, "P6\n%d %d\n255\n", width, height);
    f << ppm_head;

    // for (int i = 0; i < img.data.size(); ++i) {
    // for (int j = 0; j < img.h; ++j) {
    for (int j = img.h - 1; j >= 0; --j) {
        for (int i = 0; i < img.w; ++i) {
            Color const &col = img(i, j);
            f << (char)(col.r()) << (char)(col.g()) << (char)(col.b());
        }
    }
    f.close();
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 18 2020, 17:41 [CST]
