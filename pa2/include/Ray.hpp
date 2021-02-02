#pragma once

#include "Triangle.hpp"
#include "global.hpp"

struct Intersection {
    Intersection()
        : occurred{false}, matid{-1},
          distance{std::numeric_limits<flt>::max()}, position{vec3{0}},
          normal{vec3{0}}, emit{0, 0, 0} {}
    bool  occurred;
    int   matid;
    flt   distance;
    vec3  position;
    vec3  normal;
    Color emit;
};

struct Ray {
    Ray(vec3 const &origin = vec3{0}, vec3 const &direction = vec3{0})
        : origin{origin}, direction{glm::normalize(direction)} {}
    vec3 origin;
    vec3 direction;

    // Moller Trumbore's algorithm to test intersection with a triangle `t`.
    Intersection intersect(Triangle const &t) const;
};

/* Implementations */

inline Intersection Ray::intersect(Triangle const &t) const {
    Intersection isect;
    vec3         e1    = t.v[1] - t.v[0];
    vec3         e2    = t.v[2] - t.v[0];
    vec3         s1    = glm::cross(this->direction, e2);
    flt          denom = glm::dot(s1, e1);
    if (sign(denom) == 0) {
        return isect;
    }
    vec3 s  = this->origin - t.v[0];
    flt  b1 = glm::dot(s1, s);
    if (sign(b1) < 0 || sign(b1 - denom) >= 0) {
        return isect;
    }
    vec3 s2 = glm::cross(s, e1);
    flt  b2 = glm::dot(s2, this->direction);
    if (sign(b2) < 0 || sign(b1 + b2 - denom) >= 0) {
        return isect;
    }
    b1 /= denom;
    b2 /= denom;
    std::array<flt, 3> b = {1 - b1 - b2, b1, b2};

    isect.occurred = true;
    isect.matid    = t.matid;
    isect.position = berp(t.v, b);
    isect.normal   = berp(t.nor, b);
    isect.distance = glm::length(isect.position - this->origin);
    return isect;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 01 2021, 23:54 [CST]
