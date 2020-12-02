#include "shaders.hpp"

Color shdr::normal_shader(Triangle const &t, Triangle const &v,
                          std::tuple<flt, flt, flt> const &barycentric) {
    Color ret(.5 + .5 * (v.facing.x + 1.0) * 255,
              .5 + .5 * (v.facing.y + 1.0) * 255,
              .5 + .5 * (v.facing.z + 1.0) * 255);
    return ret;
}

Color shdr::vertex_interpolation_shader(
    Triangle const &t, Triangle const &v,
    std::tuple<flt, flt, flt> const &barycentric) {
    auto [ca, cb, cc] = barycentric;
    flt   real_z      = 1 / (ca / v.a().z + cb / v.b().z + cc / v.c().z);
    Color ret         = v.color_at(ca, cb, cc, real_z);
    return ret;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 26 2020, 23:42 [CST]
