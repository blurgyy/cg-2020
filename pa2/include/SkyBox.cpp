#include "SkyBox.hpp"

SkyBox::SkyBox() {}
SkyBox::SkyBox(std::string const &imgfile) {
    this->data.read(imgfile);
    this->w = this->data.columns();
    this->h = this->data.rows();
}

vec3 SkyBox::operator()(std::size_t const &x, std::size_t const &y) const {
    Magick::ColorRGB mcol = this->data.pixelColor(x, this->h - 1 - y);
    return vec3{
        mcol.red(),
        mcol.green(),
        mcol.blue(),
    };
}

size_t const &SkyBox::width() const { return this->w; }
size_t const &SkyBox::height() const { return this->h; }

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 05 2021, 13:07 [CST]
