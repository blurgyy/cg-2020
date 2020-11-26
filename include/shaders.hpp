#ifndef SHADER_HPP
#define SHADER_HPP

#include "Triangle.hpp"
#include "global.hpp"

// std::function<Color(Triangle const &t, Triangle const &v, flt x, flt y)>
// frag_shader;

// Normal shader
Color normal_shader(Triangle const &t, Triangle const &v, flt x, flt y);
Color vertex_interpolation_shader(Triangle const &t, Triangle const &v, flt x,
                                  flt y);

#endif

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 26 2020, 23:41 [CST]
