#include "Triangle.hpp"

Triangle::Triangle() {}
Triangle::Triangle(std::array<vec3, 3> const & vtx,
                   std::array<vec3, 3> const & nor,
                   std::array<Color, 3> const &col)
    : v{vtx[0], vtx[1], vtx[2]}, nor{nor[0], nor[1], nor[2]}, col{col[0],
                                                                  col[1],
                                                                  col[2]} {
    this->_init();
}
Triangle::Triangle(vec3 const &a, vec3 const &b, vec3 const &c,
                   vec3 const &na, vec3 const &nb, vec3 const &nc,
                   Color const &ca, Color const &cb, Color const &cc)
    : v{a, b, c}, nor{na, nb, nc}, col{ca, cb, cc} {
    this->_init();
}

vec3 Triangle::a() const { return this->v[0]; }
vec3 Triangle::b() const { return this->v[1]; }
vec3 Triangle::c() const { return this->v[2]; }

vec3 Triangle::na() const { return this->nor[0]; }
vec3 Triangle::nb() const { return this->nor[1]; }
vec3 Triangle::nc() const { return this->nor[2]; }

Color Triangle::ca() const { return this->col[0]; }
Color Triangle::cb() const { return this->col[1]; }
Color Triangle::cc() const { return this->col[2]; }

flt Triangle::doublearea() const {
    return fabs((v[1].x - v[0].x) * (v[2].y - v[0].y) -
                (v[2].x - v[0].x) * (v[1].y - v[0].y));
}
flt Triangle::area() const { return .5 * this->doublearea(); }

bool Triangle::contains(flt x, flt y) const {
    vec3 v[3];
    flt  z[3];
    v[0] = vec3{(this->v[0].x - x), (this->v[0].y - y), 0};
    v[1] = vec3{(this->v[1].x - x), (this->v[1].y - y), 0};
    v[2] = vec3{(this->v[2].x - x), (this->v[2].y - y), 0};
    z[0] = glm::cross(v[0], v[1]).z;
    z[1] = glm::cross(v[1], v[2]).z;
    z[2] = glm::cross(v[2], v[0]).z;
    return sign(z[0]) == sign(z[1]) && sign(z[1]) == sign(z[2]);
}

Color Triangle::color_at(flt const &ca, flt const &cb, flt const &cc,
                         flt const &z_viewspace) const {
    Color a = this->col[0];
    Color b = this->col[1];
    Color c = this->col[2];
    // debugm("color-a: [%u, %u, %u]\n", a.r(), a.g(), a.b());
    // debugm("color-b: [%u, %u, %u]\n", b.r(), b.g(), b.b());
    // debugm("color-c: [%u, %u, %u]\n", c.r(), c.g(), c.b());
    flt az            = this->v[0].z;
    flt bz            = this->v[1].z;
    flt cz            = this->v[2].z;
    flt zv_reciprocal = 1.0 / z_viewspace;
    // debugm("az %f, bz %f, cz %f, real_z %f\n", az, bz, cz, z_viewspace);
    // r
    flt red = .5 + (ca * a.red / az + cb * b.red / bz + cc * c.red / cz) /
                       zv_reciprocal;
    // g
    flt green =
        .5 + (ca * a.green / az + cb * b.green / bz + cc * c.green / cz) /
                 zv_reciprocal;
    // b
    flt blue = .5 + (ca * a.blue / az + cb * b.blue / bz + cc * c.blue / cz) /
                        zv_reciprocal;
    // debugm("red %f, green %f, blue %f\n", red, green, blue);
    return Color{red, green, blue};
}

/*** Operator overrides ***/
// Matrix left multiplication.
// Caveat: glm implements matrix multiplication in reversed order.
Triangle Triangle::operator*(mat4 const &m) const {
    // Assign color values and indices of each vertex to the returned
    // triangle.  Positions of vertices are overwritten, normal directons
    // of vertices doesn't matter.
    Triangle ret(*this);
    // debugm("*this has color (%u, %u, %u) on first vert\n",
    // this->col[0].r(), this->col[0].g(), this->col[0].b()); errorm("ret has
    // color (%u, %u, %u) on first vert\n", ret.col[0].r(), ret.col[0].g(),
    // ret.col[0].b());
    for (int i = 0; i < 3; ++i) {
        auto const &v            = ret.v[i];
        flt         homo_value[] = {v.x, v.y, v.z, 1};
        vec4        homo         = glm::make_vec4(homo_value);

        homo       = homo * m;
        ret.v[i].x = homo.x / homo.w;
        ret.v[i].y = homo.y / homo.w;
        ret.v[i].z = homo.z / homo.w;
    }
    return ret;
}
// Matrix right multiplication.
// Caveat: glm implements matrix multiplication in reversed order.
Triangle operator*(mat4 const &m, Triangle const &t) {
    // Assign color values and indices of each vertex to the returned
    // triangle.  Positions of vertices are overwritten, normal directons
    // of vertices doesn't matter.
    Triangle ret(t);
    for (int i = 0; i < 3; ++i) {
        auto const &v            = t.v[i];
        flt         homo_value[] = {v.x, v.y, v.z, 1};
        vec4        homo         = glm::make_vec4(homo_value);

        homo       = m * homo;
        ret.v[i].x = homo.x / homo.w;
        ret.v[i].y = homo.y / homo.w;
        ret.v[i].z = homo.z / homo.w;
    }
    return ret;
}
// Calculate barycentric coordinates of point 'pos' in triangle 't'
std::tuple<flt, flt, flt> Triangle::operator%(vec3 const &pos) const {
    // std::tuple<flt, flt, flt> operator%(vec3 const &pos, Triangle
    // const &t) { Point position `pos` should be inside the triangle `t`
    assert(this->contains(pos.x, pos.y));

    Triangle ta(pos, this->v[1], this->v[2]);
    Triangle tb(pos, this->v[0], this->v[2]);
    // debugm("pos (%f, %f, %f)\n", pos.x, pos.y, pos.z);
    // debugm("first vert of triangle: (%f, %f, %f)\n", vtx[0].x, vtx[0].y,
    // vtx[0].z);
    // debugm("total area %f\n", this->area());
    // debugm("aarea %f\n", ta.area());
    // debugm("barea %f\n", tb.area());
    flt ca = ta.doublearea() / this->doublearea();
    flt cb = tb.doublearea() / this->doublearea();
    flt cc = 1 - ca - cb;
    return {ca, cb, cc};
}

// private
void Triangle::_init() {
    // Initialize facing direction
    this->facing = glm::normalize(
        glm::cross(this->v[1] - this->v[0], this->v[2] - this->v[1]));
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 23 2020, 13:29 [CST]
