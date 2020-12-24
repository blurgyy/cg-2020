#pragma once

#include <array>
#include <tuple>

#include "global.hpp"

#include <glm/gtc/type_ptr.hpp>

class Triangle {
  public:
    vec3 v[3];    // Spatial coordinate of the 3 vertices, in
                  // counter-clockwise order
    vec3 facing;  // Facing direction (for face culling), computed by:
                  // glm::cross(v[1] - v[0], v[2] - v[1]);
    vec3  nor[3]; // Normal directions of the 3 vertices
    Color col[3]; // Color values of the 3 vertices

  private:
    void _init();

  public:
    Triangle();
    // Assumes indices is {0, 1, 2}
    Triangle(vec3 const &a, vec3 const &b, vec3 const &c,
             vec3 const &na = vec3{0}, vec3 const &nb = vec3{0},
             vec3 const &nc = vec3{0}, Color const &ca = Color{0},
             Color const &cb = Color{0}, Color const &cc = Color{0});
    Triangle(std::array<vec3, 3> const &vtx,
             std::array<vec3, 3> const &nor =
                 {
                     vec3{0},
                     vec3{0},
                     vec3{0},
                 },
             std::array<Color, 3> const &col = {
                 Color{0},
                 Color{0},
                 Color{0},
             });

    vec3 a() const; // Returns spatial location of the first vertex
    vec3 b() const; // Returns spatial location of the second vertex
    vec3 c() const; // Returns spatial location of the third vertex

    // Returns normal direction of the first vertex
    vec3 na() const;
    // Returns normal direction of the second vertex
    vec3 nb() const;
    // Returns normal direction of the third vertex
    vec3 nc() const;

    // Returns color value of the first vertex
    Color ca() const;
    // Returns color value of the second vertex
    Color cb() const;
    // Returns color value of the third vertex
    Color cc() const;

    // Returns area of the triangle's orthographic projection onto the xOy
    // plane (computes slightly slower than function Triangle::doublearea()).
    flt area() const;
    // Returns area doubled of the triangle's orthographic projection onto the
    // xOy plane.
    flt doublearea() const;

    bool contains(flt x, flt y) const;
    // When calling this function, make sure the triangle has **view-space**
    // coordinates (as opposed to screen-space coordinates).
    Color color_at(flt const &ca, flt const &cb, flt const &cc,
                   flt const &z_viewspace) const;

  public: // Operator overrides
    Triangle                  operator*(mat4 const &m) const;
    std::tuple<flt, flt, flt> operator%(vec3 const &pos) const;
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 23 2020, 13:20 [CST]
