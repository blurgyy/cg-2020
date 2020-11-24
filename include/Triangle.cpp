#include "Triangle.hpp"

Triangle::Triangle() {}
Triangle::Triangle(glm::vec3 const &a, glm::vec3 const &b, glm::vec3 const &c,
                   glm::vec3 const &na, glm::vec3 const &nb,
                   glm::vec3 const &nc, glm::vec3 const &ca,
                   glm::vec3 const &cb, glm::vec3 const &cc)
    : vtx{a, b, c}, nor{na, nb, nc}, col{ca, cb, cc} {
    // vtx[0] = a, vtx[1] = b, vtx[2] = c;
    // nor[0] = na, nor[1] = nb, nor[2] = nc;
}

glm::vec3 Triangle::a() const { return vtx[0]; }
glm::vec3 Triangle::b() const { return vtx[1]; }
glm::vec3 Triangle::c() const { return vtx[2]; }

glm::vec3 Triangle::na() const { return nor[0]; }
glm::vec3 Triangle::nb() const { return nor[1]; }
glm::vec3 Triangle::nc() const { return nor[2]; }

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 23 2020, 13:29 [CST]
