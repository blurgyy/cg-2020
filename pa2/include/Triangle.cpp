#include "Triangle.hpp"

#include <iostream>
#include <sstream>

Triangle::Triangle() : mat{nullptr} { this->_init(); }
Triangle::Triangle(std::array<vec3, 3> const & vtx,
                   std::array<vec3, 3> const & nor,
                   std::array<vec2, 3> const & tex,
                   std::array<Color, 3> const &col)
    : v{vtx[0], vtx[1], vtx[2]}, nor{glm::normalize(nor[0]),
                                     glm::normalize(nor[1]),
                                     glm::normalize(nor[2])},
      tex{tex[0], tex[1], tex[2]}, col{col[0], col[1], col[2]}, mat{nullptr} {
    this->_init();
}
Triangle::Triangle(vec3 const &a, vec3 const &b, vec3 const &c,
                   vec3 const &na, vec3 const &nb, vec3 const &nc,
                   vec2 const &ta, vec2 const &tb, vec2 const &tc,
                   Color const &ca, Color const &cb, Color const &cc)
    : v{a, b, c}, nor{glm::normalize(na), glm::normalize(nb),
                      glm::normalize(nc)},
      tex{ta, tb, tc}, col{ca, cb, cc}, mat{nullptr} {
    this->_init();
}

void Triangle::set_material(tinyobj::material_t const &m) {
    if (this->mat) {
        delete this->mat;
    }
    this->mat            = new Material;
    this->mat->ambient   = vec3{m.ambient[0], m.ambient[1], m.ambient[2]};
    this->mat->diffuse   = vec3{m.diffuse[0], m.diffuse[1], m.diffuse[2]};
    this->mat->specular  = vec3{m.specular[0], m.specular[1], m.specular[2]};
    this->mat->shineness = m.shininess;
    if (m.unknown_parameter.size()) {
        for (auto const &kv : m.unknown_parameter) {
            if (kv.first == "Le") {
                std::istringstream emit_input(kv.second);
                emit_input >> this->mat->emission.x >>
                    this->mat->emission.y >> this->mat->emission.z;
                this->mat->has_emission = true;
            }
        }
    }
}

vec3 const &Triangle::a() const { return this->v[0]; }
vec3 const &Triangle::b() const { return this->v[1]; }
vec3 const &Triangle::c() const { return this->v[2]; }

vec3 const &Triangle::na() const { return this->nor[0]; }
vec3 const &Triangle::nb() const { return this->nor[1]; }
vec3 const &Triangle::nc() const { return this->nor[2]; }

Color const &Triangle::ca() const { return this->col[0]; }
Color const &Triangle::cb() const { return this->col[1]; }
Color const &Triangle::cc() const { return this->col[2]; }

BBox const &Triangle::boundingbox() const { return this->bbox; }

Material const *Triangle::material() const {
    if (this->mat) {
        return this->mat;
    } else {
        errorm("No material has been assigned to face\n");
    }
}

flt Triangle::doublearea() const {
    return glm::length(
        glm::cross(this->v[1] - this->v[0], this->v[2] - this->v[1]));
}
flt Triangle::area() const { return .5 * this->doublearea(); }

vec3 Triangle::sample() const {
    flt                b1  = uniform();
    flt                b2  = uniform();
    flt                b3  = uniform();
    flt                sum = b1 + b2 + b3;
    std::array<flt, 3> b{b1 / sum, b2 / sum, b3 / sum};
    return berp(this->v, b);
}

bool Triangle::vert_in_canonical() const {
    for (int i = 0; i < 3; ++i) {
        if (this->v[i].x >= -1 && this->v[i].x <= 1 && this->v[i].y >= -1 &&
            this->v[i].y <= 1 && this->v[i].z >= -1 && this->v[i].z <= 1) {
            return true;
        }
    }
    return false;
}

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
    Color a             = this->col[0];
    Color b             = this->col[1];
    Color c             = this->col[2];
    flt   az            = this->v[0].z;
    flt   bz            = this->v[1].z;
    flt   cz            = this->v[2].z;
    flt   zv_reciprocal = 1.0 / z_viewspace;
    // r
    flt red =
        .5 + (ca * a.r / az + cb * b.r / bz + cc * c.r / cz) / zv_reciprocal;
    // g
    flt green =
        .5 + (ca * a.g / az + cb * b.g / bz + cc * c.g / cz) / zv_reciprocal;
    // b
    flt blue =
        .5 + (ca * a.b / az + cb * b.b / bz + cc * c.b / cz) / zv_reciprocal;
    return Color{
        static_cast<unsigned char>(red),
        static_cast<unsigned char>(green),
        static_cast<unsigned char>(blue),
    };
}

/*** Operator overrides ***/
// Matrix left multiplication.
// Caveat: glm implements matrix multiplication in reversed order.
Triangle Triangle::operator*(mat4 const &m) const {
    // Assign color values and indices of each vertex to the returned
    // triangle.  Positions of vertices are overwritten, normal directons
    // of vertices doesn't matter.
    Triangle ret(*this);
    for (int i = 0; i < 3; ++i) {
        auto const &v            = ret.v[i];
        flt         homo_value[] = {v.x, v.y, v.z, 1};
        vec4        homo         = glm::make_vec4(homo_value);

        homo       = homo * m;
        ret.v[i].x = homo.x / homo.w;
        ret.v[i].y = homo.y / homo.w;
        ret.v[i].z = homo.z / homo.w;
    }
    // Note: Remember to update bbox and facing directions.
    ret._init();
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
    // Note: Remember to update bbox and facing directions.
    ret._init();
    return ret;
}
// Calculate barycentric coordinates of point 'pos' in triangle 't'
std::tuple<flt, flt, flt> Triangle::operator%(vec3 const &pos) const {
    // Point position `pos` should be inside the triangle `t`
    assert(this->contains(pos.x, pos.y));

    Triangle ta(pos, this->v[1], this->v[2]);
    Triangle tb(pos, this->v[0], this->v[2]);
    flt      ca = ta.doublearea() / this->doublearea();
    flt      cb = tb.doublearea() / this->doublearea();
    flt      cc = 1 - ca - cb;
    return {ca, cb, cc};
}

// private
void Triangle::_init() {
    // Initialize facing direction.
    this->facing = glm::normalize(
        glm::cross(this->v[1] - this->v[0], this->v[2] - this->v[1]));
    // Initialize bounding box.
    this->bbox = BBox(this->v[0]) | BBox(this->v[1]) | BBox(this->v[2]);
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 23 2020, 13:29 [CST]
