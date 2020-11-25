#include "Triangle.hpp"

Triangle::Triangle() {}
Triangle::Triangle(std::array<glm::vec3, 3> const &vtx,
                   std::array<glm::vec3, 3> const &nor,
                   std::array<glm::vec3, 3> const &col)
    : vtx{vtx[0], vtx[1], vtx[2]}, nor{nor[0], nor[1], nor[2]}, col{col[0],
                                                                    col[1],
                                                                    col[2]} {
    // Initialize facing direction
    facing = glm::cross(vtx[1] - vtx[0], vtx[2] - vtx[1]);
}
Triangle::Triangle(glm::vec3 const &a, glm::vec3 const &b, glm::vec3 const &c,
                   glm::vec3 const &na, glm::vec3 const &nb,
                   glm::vec3 const &nc, glm::vec3 const &ca,
                   glm::vec3 const &cb, glm::vec3 const &cc)
    : vtx{a, b, c}, nor{na, nb, nc}, col{ca, cb, cc} {
    facing = glm::cross(vtx[1] - vtx[0], vtx[2] - vtx[1]);
}

glm::vec3 Triangle::a() const { return vtx[0]; }
glm::vec3 Triangle::b() const { return vtx[1]; }
glm::vec3 Triangle::c() const { return vtx[2]; }

glm::vec3 Triangle::na() const { return nor[0]; }
glm::vec3 Triangle::nb() const { return nor[1]; }
glm::vec3 Triangle::nc() const { return nor[2]; }


// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 23 2020, 13:29 [CST]
