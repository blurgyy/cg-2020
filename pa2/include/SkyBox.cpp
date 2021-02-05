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

vec3 SkyBox::operator()(vec3 const &dir) const {
    vec3 normed = glm::normalize(dir);
    // \phi \in [0, \pi/2].
    flt phi = std::acos(normed.z);
    if (normed.z < 0) {
        phi *= -1;
    }
    // \theta \in [-pi, pi].
    flt theta = std::acos(normed.x / std::sin(phi));
    if (sign(normed.y) != sign(std::sin(phi))) {
        theta *= -1;
    }
    std::size_t x =
        static_cast<std::size_t>((theta - -pi) / twopi * this->width() - 0.5);
    std::size_t y =
        static_cast<std::size_t>((phi - -pi / 2) / pi * this->height() - 0.5);
    return (*this)(x, y);
}

size_t const &SkyBox::width() const { return this->w; }
size_t const &SkyBox::height() const { return this->h; }

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 05 2021, 13:07 [CST]
