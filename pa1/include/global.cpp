#include "global.hpp"

#include <cassert>
#include <fstream>

// Constants
flt const pi      = std::acos(-1.0);
flt const twopi   = pi * 2.0;
flt const halfpi  = pi / 2.0;
flt const degree  = pi / 180.0;
flt const epsilon = 1e-7;

// struct Color
Color::Color() : red{0}, green{0}, blue{0} {}
Color::Color(unsigned char const &r, unsigned char const &g,
             unsigned char const &b)
    : red{r}, green{g}, blue{b} {}
Color::Color(unsigned char const &x) : red{x}, green{x}, blue{x} {}
unsigned char &      Color::r() { return this->red; }
unsigned char &      Color::g() { return this->green; }
unsigned char &      Color::b() { return this->blue; }
unsigned char const &Color::r() const { return this->red; }
unsigned char const &Color::g() const { return this->green; }
unsigned char const &Color::b() const { return this->blue; }

// struct Image
Image::Image() {}
Image::Image(size_t const &width, size_t const &height)
    : w{width}, h{height} {
    this->data.resize(this->w * this->h);
}
void Image::init(const size_t &width, const size_t &height) {
    this->w    = width;
    this->h    = height;
    this->data = std::vector<Color>(this->w * this->h);
}
void Image::fill(Color const &value) {
    std::fill(this->data.begin(), this->data.end(), value);
}
Color &Image::operator()(size_t const &x, size_t const &y) {
    return this->data[this->w * y + x];
}
Color const &Image::operator()(size_t const &x, size_t const &y) const {
    return this->data[this->w * y + x];
}

void write_ppm(std::string const &filename, Image const &img) {
    debugm("Writing image (%zux%zu) to %s ..\n", img.w, img.h,
           filename.c_str());
    std::ofstream f(filename, std::ios_base::out | std::ios_base::binary);
    char          ppm_head[50] = {0};
    int           height       = img.h;
    int           width        = img.w;
    // Magic number (P6), width, height, maximum color value,
    // seperated with whitespaces.
    sprintf(ppm_head, "P6\n%d %d\n255\n", width, height);
    f << ppm_head;

    for (size_t j = 0; j < img.h; ++j) {
        for (size_t i = 0; i < img.w; ++i) {
            Color const &col = img(i, img.h - 1 - j);
            f << (char)(col.r()) << (char)(col.g()) << (char)(col.b());
        }
    }
    f.close();
    msg("Render result (%zux%zu) saved in %s\n", img.w, img.h,
        filename.c_str());
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 18 2020, 17:41 [CST]
