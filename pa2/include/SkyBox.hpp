#pragma once

#include "global.hpp"

#include <string>

#include <Magick++.h>

class SkyBox {
  private:
    Magick::Image data;
    std::size_t   w;
    std::size_t   h;

  public:
    SkyBox();
    SkyBox(std::string const &imgfile);

    vec3 operator()(std::size_t const &x, std::size_t const &y) const;
    vec3 operator()(vec3 const &dir) const;

    std::size_t const &width() const;
    std::size_t const &height() const;
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 05 2021, 13:07 [CST]
