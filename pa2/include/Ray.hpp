#pragma once

#include "Triangle.hpp"
#include "global.hpp"

struct Intersection {
    Intersection()
        : occurred{false}, matid{-1},
          distance{std::numeric_limits<flt>::max()}, position{vec3{0}},
          normal{vec3{0}}, emit{0, 0, 0}, tri{nullptr} {}
    void output() {
        printf("occurred: %s\n", this->occurred ? "true" : "false");
        printf("matid:    %d\n", this->matid);
        printf("distance: %f\n", this->distance);
        printf("position: %f, %f, %f\n", this->position.x, this->position.y,
               this->position.z);
        printf("normal:   %f, %f, %f\n", this->normal.x, this->normal.y,
               this->normal.z);
        printf("emit:     %u, %u, %u\n", this->emit.r(), this->emit.g(),
               this->emit.b());
    }

    bool  occurred;
    int   matid;
    flt   distance;
    vec3  position;
    vec3  normal;
    Color emit;

    Triangle const *tri;
};

struct Ray {
    Ray(vec3 const &origin = vec3{0}, vec3 const &direction = vec3{0})
        : origin{origin}, direction{glm::normalize(direction)} {}
    vec3 origin;
    vec3 direction;

    // Moller Trumbore's algorithm to test intersection with a triangle `t`.
    Intersection intersect(Triangle const &t) const;
    Intersection intersect(Triangle const *t) const;

    // Test intersection with an axis-aligned bounding box.
    bool intersect(BBox const &bbox) const;
};

/* Implementations */

inline Intersection Ray::intersect(Triangle const &t) const {
    return this->intersect(&t);
}
inline Intersection Ray::intersect(Triangle const *t) const {
    Intersection isect;
    vec3         e1    = t->v[1] - t->v[0];
    vec3         e2    = t->v[2] - t->v[0];
    vec3         s1    = glm::cross(this->direction, e2);
    flt          denom = glm::dot(s1, e1);
    if (sign(denom) <= 0) {
        return isect;
    }
    vec3 s  = this->origin - t->v[0];
    flt  b1 = glm::dot(s1, s);
    if (sign(b1) < 0 || sign(b1 - denom) > 0) {
        return isect;
    }
    vec3 s2 = glm::cross(s, e1);
    flt  b2 = glm::dot(s2, this->direction);
    if (sign(b2) < 0 || sign(b1 + b2 - denom) > 0) {
        return isect;
    }
    b1 /= denom;
    b2 /= denom;
    // if (sign(b1) <= 0 || sign(b2) <= 0 || sign(1 - b1 - b2) <= 0) {
    // return isect;
    // }
    std::array<flt, 3> b = {1 - b1 - b2, b1, b2};

    vec3 isectpos = berp(t->v, b);
    if (sign(isectpos.x - this->origin.x) != sign(this->direction.x) ||
        sign(isectpos.y - this->origin.y) != sign(this->direction.y) ||
        sign(isectpos.z - this->origin.z) != sign(this->direction.z)) {
        // Ignore intersections behind ray origin (according to ray
        // direction).
        return isect;
    }

    isect.occurred = true;
    isect.matid    = t->matid;
    isect.position = isectpos;
    isect.normal   = berp(t->nor, b);
    isect.distance = glm::length(isect.position - this->origin);
    isect.tri      = t;
    return isect;
}

inline bool Ray::intersect(BBox const &bbox) const {
    flt  t_enter = std::numeric_limits<flt>::lowest();
    flt  t_exit  = std::numeric_limits<flt>::max();
    vec3 minp    = bbox.minp - epsilon;
    vec3 maxp    = bbox.maxp + epsilon;
    for (std::size_t dim = 0; dim < 3; ++dim) {
        flt mint = (minp[dim] - this->origin[dim]) / this->direction[dim];
        flt maxt = (maxp[dim] - this->origin[dim]) / this->direction[dim];
        if (sign(this->direction[dim]) < 0) {
            std::swap(mint, maxt);
        }
        t_enter = std::max(t_enter, mint);
        t_exit  = std::min(t_exit, maxt);
    }
    return sign(t_exit - t_enter) > 0 && sign(t_exit) > 0;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 01 2021, 23:54 [CST]
