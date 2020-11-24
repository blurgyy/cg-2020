#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <glm/glm.hpp>

class Triangle {
  private:
    glm::vec3 vtx[3]; // Spatial coordinate of the 3 vertices
    glm::vec3 nor[3]; // Normal directions of the 3 vertices
    glm::vec3 col[3]; // Color values of the 3 vertices

  public:
    Triangle();
    Triangle(glm::vec3 const &a, glm::vec3 const &b, glm::vec3 const &c,
             glm::vec3 const &na = glm::vec3(0),
             glm::vec3 const &nb = glm::vec3(0),
             glm::vec3 const &nc = glm::vec3(0),
             glm::vec3 const &ca = glm::vec3(0),
             glm::vec3 const &cb = glm::vec3(0),
             glm::vec3 const &cc = glm::vec3(0));

    // returns spatial location of the vertex
    glm::vec3 a() const;
    glm::vec3 b() const;
    glm::vec3 c() const;

    // returns normal direction of the corresponding vertex
    glm::vec3 na() const;
    glm::vec3 nb() const;
    glm::vec3 nc() const;
};

#endif

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 23 2020, 13:20 [CST]
