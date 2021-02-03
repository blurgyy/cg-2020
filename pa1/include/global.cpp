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
Color::Color() : r{0}, g{0}, b{0} {}
Color::Color(unsigned char const &r, unsigned char const &g,
             unsigned char const &b)
    : r{r}, g{g}, b{b} {}
Color::Color(unsigned char const &x) : r{x}, g{x}, b{x} {}
Color::Color(vec3 const &values) {
    this->r = static_cast<unsigned char>(clamp(values.r, 0, 1) * 255);
    this->g = static_cast<unsigned char>(clamp(values.g, 0, 1) * 255);
    this->b = static_cast<unsigned char>(clamp(values.b, 0, 1) * 255);
}
Color Color::operator+=(Color const &rhs) {
    this->r += rhs.r;
    this->b += rhs.b;
    this->g += rhs.g;
    return (*this);
}

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

// struct BBox
BBox::BBox()
    : minp{std::numeric_limits<flt>::max()},
      maxp{std::numeric_limits<flt>::lowest()} {}
BBox::BBox(vec3 const &p) : minp{p}, maxp{p} {}
BBox::BBox(vec3 const &p1, vec3 const &p2)
    : minp{std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)},
      maxp{std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)} {
}
BBox BBox::operator|(BBox const &rhs) const { // Merged BBox
    vec3 nminp{
        std::min(this->minp.x, rhs.minp.x),
        std::min(this->minp.y, rhs.minp.y),
        std::min(this->minp.z, rhs.minp.z),
    };
    vec3 nmaxp{
        std::max(this->maxp.x, rhs.maxp.x),
        std::max(this->maxp.y, rhs.maxp.y),
        std::max(this->maxp.z, rhs.maxp.z),
    };
    return BBox(nminp, nmaxp);
}
BBox BBox::operator|(vec3 const &rhs) const { // Merged BBox
    vec3 nminp{
        std::min(this->minp.x, rhs.x),
        std::min(this->minp.y, rhs.y),
        std::min(this->minp.z, rhs.z),
    };
    vec3 nmaxp{
        std::max(this->maxp.x, rhs.x),
        std::max(this->maxp.y, rhs.y),
        std::max(this->maxp.z, rhs.z),
    };
    return BBox(nminp, nmaxp);
}
BBox BBox::operator|=(BBox const &rhs) {
    this->minp = vec3{
        std::min(this->minp.x, rhs.minp.x),
        std::min(this->minp.y, rhs.minp.y),
        std::min(this->minp.z, rhs.minp.z),
    };
    this->maxp = vec3{
        std::max(this->maxp.x, rhs.maxp.x),
        std::max(this->maxp.y, rhs.maxp.y),
        std::max(this->maxp.z, rhs.maxp.z),
    };
    return *this;
}
BBox BBox::operator|=(vec3 const &rhs) {
    this->minp = vec3{
        std::min(this->minp.x, rhs.x),
        std::min(this->minp.y, rhs.y),
        std::min(this->minp.z, rhs.z),
    };
    this->maxp = vec3{
        std::max(this->maxp.x, rhs.x),
        std::max(this->maxp.y, rhs.y),
        std::max(this->maxp.z, rhs.z),
    };
    return *this;
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
            f << (char)(col.r) << (char)(col.g) << (char)(col.b);
        }
    }
    f.close();
    msg("Render result (%zux%zu) saved in %s\n", img.w, img.h,
        filename.c_str());
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 18 2020, 17:41 [CST]
