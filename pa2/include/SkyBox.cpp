#include "SkyBox.hpp"

SkyBox::SkyBox() {}
SkyBox::SkyBox(std::string const &imgfile) {
    this->data.read(imgfile);
    this->w = this->data.columns();
    this->h = this->data.rows();
}

vec3 SkyBox::operator()(std::size_t const &x, std::size_t const &y) const {
    Magick::ColorRGB mcol = this->data.pixelColor(x, this->height() - 1 - y);
    return vec3{
        mcol.red(),
        mcol.green(),
        mcol.blue(),
    };
}

vec3 SkyBox::operator()(vec3 const &dir) const {
    vec3 normed = glm::normalize(dir);
    // Rotate
    normed = vec3{-normed.z, normed.y, normed.x};
    flt phi, theta;
    // \phi \in [0, \pi].
    phi = std::acos(normed.y);
    // \theta \in [-pi, pi].
    if (fabs(phi) < epsilon) {
        theta = std::acos(normed.x);
    } else {
        theta = std::acos(clamp(normed.x / std::sin(phi), 0, 1));
    }
    if (sign(normed.z) < 0) {
        theta *= -1;
    }
    std::size_t x =
        static_cast<std::size_t>((theta - -pi) / twopi * this->width() - 0.5);
    std::size_t y =
        static_cast<std::size_t>((pi - phi) / pi * this->height() - 0.5);
    return (*this)(x, y);
}

size_t const &SkyBox::width() const { return this->w; }
size_t const &SkyBox::height() const { return this->h; }

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 05 2021, 13:07 [CST]
