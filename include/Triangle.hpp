#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

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

    // returns spatial location of the vertex
    glm::vec3 a() const;
    glm::vec3 b() const;
    glm::vec3 c() const;

    // returns normal direction of the corresponding vertex
    glm::vec3 na() const;
    glm::vec3 nb() const;
    glm::vec3 nc() const;

  public: // Operator overrides
    friend Triangle operator*(glm::mat4 const &m, Triangle const &self) {
        // Assign color values and indices of each vertex to the returned
        // triangle.  Positions of vertices are overwritten, normal directons
        // of vertices doesn't matter.
        Triangle ret(self);

        for (int i = 0; i < 3; ++i) {
            auto const &v            = self.vtx[i];
            flt         homo_value[] = {v.x, v.y, v.z, 1};
            glm::vec4   homo         = glm::make_vec4(homo_value);

            homo         = m * homo;
            ret.vtx[i].x = homo.x / homo.w;
            ret.vtx[i].y = homo.y / homo.w;
            ret.vtx[i].z = homo.z / homo.w;
        }

        return ret;
    }
};

#endif

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 23 2020, 13:20 [CST]
