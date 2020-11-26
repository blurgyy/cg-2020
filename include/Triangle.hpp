#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <array>
#include <tuple>

#include "global.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Triangle {
  public:
    glm::vec3 vtx[3]; // Spatial coordinate of the 3 vertices, in
                      // counter-clockwise order
    glm::vec3 facing; // Facing direction (for face culling), computed by:
                      // glm::cross(v[1] - v[0], v[2] - v[1]);
    glm::vec3 nor[3]; // Normal directions of the 3 vertices
    Color     col[3]; // Color values of the 3 vertices

  private:
    void _init();

  public:
    Triangle();
    // Assumes indices is {0, 1, 2}
    Triangle(glm::vec3 const &a, glm::vec3 const &b, glm::vec3 const &c,
             glm::vec3 const &na = glm::vec3(0),
             glm::vec3 const &nb = glm::vec3(0),
             glm::vec3 const &nc = glm::vec3(0), Color const &ca = Color(0),
             Color const &cb = Color(0), Color const &cc = Color(0));
    Triangle(std::array<glm::vec3, 3> const &vtx,
             std::array<glm::vec3, 3> const &nor =
                 {
                     glm::vec3(0),
                     glm::vec3(0),
                     glm::vec3(0),
                 },
             std::array<Color, 3> const &col = {
                 Color(0),
                 Color(0),
                 Color(0),
             });

    glm::vec3 a() const; // returns spatial location of the first vertex
    glm::vec3 b() const; // returns spatial location of the second vertex
    glm::vec3 c() const; // returns spatial location of the third vertex

    glm::vec3 na() const; // returns normal direction of the first vertex
    glm::vec3 nb() const; // returns normal direction of the second vertex
    glm::vec3 nc() const; // returns normal direction of the third vertex

    Color ca() const; // returns color value of the first vertex
    Color cb() const; // returns color value of the second vertex
    Color cc() const; // returns color value of the third vertex

    flt area() const; // returns area of the triangle's orthographic
                      // projection onto the xOy plane

    bool contains(flt x, flt y) const;
    // When calling this function, make sure the triangle has **view-space**
    // coordinates (as opposed to screen-space coordinates).
    Color color_at(flt const &ca, flt const &cb, flt const &cc,
                   flt const &z_viewspace) const;

  public: // Operator overrides
    Triangle                  operator*(glm::mat4 const &m) const;
    std::tuple<flt, flt, flt> operator%(glm::vec3 const &pos) const;
};

#endif

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 23 2020, 13:20 [CST]
