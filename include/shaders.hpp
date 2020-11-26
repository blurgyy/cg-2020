#ifndef SHADER_HPP
#define SHADER_HPP

#include "Triangle.hpp"
#include "global.hpp"

namespace shdr {

// Normal shader
Color normal_shader(Triangle const &t, Triangle const &v, flt const &x,
                    flt const &y);

// Interpolate colors on vertices with barycentric coordinates
Color vertex_interpolation_shader(Triangle const &t, Triangle const &v,
                                  flt const &x, flt const &y);

}; // namespace shdr

#endif

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 26 2020, 23:41 [CST]
