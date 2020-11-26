#include "Triangle.hpp"

Triangle::Triangle() {}
Triangle::Triangle(std::array<glm::vec3, 3> const &vtx,
                   std::array<glm::vec3, 3> const &nor,
                   std::array<Color, 3> const &    col)
    : vtx{vtx[0], vtx[1], vtx[2]}, nor{nor[0], nor[1], nor[2]}, col{col[0],
                                                                    col[1],
                                                                    col[2]} {
    _init();
}
Triangle::Triangle(glm::vec3 const &a, glm::vec3 const &b, glm::vec3 const &c,
                   glm::vec3 const &na, glm::vec3 const &nb,
                   glm::vec3 const &nc, Color const &ca, Color const &cb,
                   Color const &cc)
    : vtx{a, b, c}, nor{na, nb, nc}, col{ca, cb, cc} {
    _init();
}

glm::vec3 Triangle::a() const { return vtx[0]; }
glm::vec3 Triangle::b() const { return vtx[1]; }
glm::vec3 Triangle::c() const { return vtx[2]; }

glm::vec3 Triangle::na() const { return nor[0]; }
glm::vec3 Triangle::nb() const { return nor[1]; }
glm::vec3 Triangle::nc() const { return nor[2]; }

flt Triangle::area() const { return A; }

bool Triangle::contains(flt x, flt y) const {
    glm::vec3 v[3];
    flt       z[3];
    v[0] = glm::vec3((vtx[0].x - x), (vtx[0].y - y), 0);
    v[1] = glm::vec3((vtx[1].x - x), (vtx[1].y - y), 0);
    v[2] = glm::vec3((vtx[2].x - x), (vtx[2].y - y), 0);
    z[0] = glm::cross(v[0], v[1]).z;
    z[1] = glm::cross(v[1], v[2]).z;
    z[2] = glm::cross(v[2], v[0]).z;
    return glm::sign(z[0]) == glm::sign(z[1]) &&
           glm::sign(z[1]) == glm::sign(z[2]);
}

/*** Operator overrides ***/
// Matrix left multiplication.
// Caveat: glm implements matrix multiplication in reversed order.
Triangle Triangle::operator*(glm::mat4 const &m) const {
    // Assign color values and indices of each vertex to the returned
    // triangle.  Positions of vertices are overwritten, normal directons
    // of vertices doesn't matter.
    Triangle ret(*this);
    for (int i = 0; i < 3; ++i) {
        auto const &v            = ret.vtx[i];
        flt         homo_value[] = {v.x, v.y, v.z, 1};
        glm::vec4   homo         = glm::make_vec4(homo_value);

        homo         = homo * m;
        ret.vtx[i].x = homo.x / homo.w;
        ret.vtx[i].y = homo.y / homo.w;
        ret.vtx[i].z = homo.z / homo.w;
    }
    return ret;
}
// Matrix right multiplication.
// Caveat: glm implements matrix multiplication in reversed order.
Triangle operator*(glm::mat4 const &m, Triangle const &t) {
    // Assign color values and indices of each vertex to the returned
    // triangle.  Positions of vertices are overwritten, normal directons
    // of vertices doesn't matter.
    Triangle ret(t);
    for (int i = 0; i < 3; ++i) {
        auto const &v            = t.vtx[i];
        flt         homo_value[] = {v.x, v.y, v.z, 1};
        glm::vec4   homo         = glm::make_vec4(homo_value);

        homo         = m * homo;
        ret.vtx[i].x = homo.x / homo.w;
        ret.vtx[i].y = homo.y / homo.w;
        ret.vtx[i].z = homo.z / homo.w;
    }
    return ret;
}
// Calculate barycentric coordinates of point 'pos' in triangle 't'
std::tuple<flt, flt, flt> operator%(glm::vec3 const &pos, Triangle const &t) {
    // Point position `pos` should be inside the triangle `t`
    assert(t.contains(pos.x, pos.y));

    Triangle ta(pos, t.b(), t.c());
    Triangle tb(pos, t.a(), t.c());
    Triangle tc(pos, t.a(), t.b());
    flt      ca = ta.area() / t.area();
    flt      cb = tb.area() / t.area();
    flt      cc = tc.area() / t.area();
    return {ca, cb, cc};
}

// private
void Triangle::_init() {
    // clang-format off
    flt det_value[] = {
        vtx[0].x, vtx[1].x, vtx[2].x,
        vtx[0].y, vtx[1].y, vtx[2].y,
               1,        1,        1,
    };
    // clang-format on

    // Calculate area of a 2D triangle (with homogeneous coordinates)
    glm::mat3 det = glm::make_mat3(det_value);
    A             = glm::determinant(det);
    // Initialize facing direction
    facing = glm::normalize(glm::cross(vtx[1] - vtx[0], vtx[2] - vtx[1]));
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 23 2020, 13:29 [CST]
