#pragma once

#include "Triangle.hpp"
#include "global.hpp"

namespace shdr {

// Normal shader
Color normal_shader(Triangle const &t, Triangle const &v,
                    std::tuple<flt, flt, flt> const &barycentric);

// Interpolate colors on vertices with barycentric coordinates
Color vertex_interpolation_shader(
    Triangle const &t, Triangle const &v,
    std::tuple<flt, flt, flt> const &barycentric);

}; // namespace shdr

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 26 2020, 23:41 [CST]
