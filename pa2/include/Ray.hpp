#pragma once

#include "Material.hpp"
#include "Triangle.hpp"
#include "global.hpp"

struct Intersection {
    Intersection()
        : occurred{false}, distance{std::numeric_limits<flt>::max()},
          position{vec3{0}}, normal{vec3{0}}, tri{nullptr} {}
    void output() {
        printf("occurred: %s\n", this->occurred ? "true" : "false");
        printf("distance: %f\n", this->distance);
        printf("position: %f, %f, %f\n", this->position.x, this->position.y,
               this->position.z);
        printf("normal:   %f, %f, %f\n", this->normal.x, this->normal.y,
               this->normal.z);
    }
    operator bool() const { return this->occurred; }

    bool occurred;
    flt  distance;
    vec3 position;
    vec3 normal;

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
    if (sign(denom) == 0) {
        // debugm("barycentric#1\n");
        return isect;
    }
    vec3 s  = this->origin - t->v[0];
    flt  b1 = glm::dot(s1, s);
    if (sign(b1) != sign(denom)) {
        // debugm("barycentric#2\n");
        return isect;
    }
    vec3 s2 = glm::cross(s, e1);
    flt  b2 = glm::dot(s2, this->direction);
    if (sign(b2) != sign(denom)) {
        // debugm("barycentric#3\n");
        return isect;
    }
    b1 /= denom;
    b2 /= denom;
    if (sign(1 - b1 - b2) < 0) {
        // debugm("barycentric#4\n");
        return isect;
    }
    std::array<flt, 3> b = {1 - b1 - b2, b1, b2};

    vec3 isectpos = berp(t->v, b);
    if (sign(isectpos.x - this->origin.x) != sign(this->direction.x) ||
        sign(isectpos.y - this->origin.y) != sign(this->direction.y) ||
        sign(isectpos.z - this->origin.z) != sign(this->direction.z)) {
        // Ignore intersections behind ray origin (according to ray
        // direction).
        // debugm("Intersetion point is behind ray origin\n");
        return isect;
    }

    isect.occurred = true;
    isect.position = isectpos;
    isect.normal   = glm::normalize(berp(t->nor, b));
    isect.distance = glm::length(isect.position - this->origin);
    isect.tri      = t;
    return isect;
}

inline bool Ray::intersect(BBox const &bbox) const {
    flt t_enter = std::numeric_limits<flt>::lowest();
    flt t_exit  = std::numeric_limits<flt>::max();
    for (std::size_t dim = 0; dim < 3; ++dim) {
        flt mint =
            (bbox.minp[dim] - this->origin[dim]) / this->direction[dim];
        flt maxt =
            (bbox.maxp[dim] - this->origin[dim]) / this->direction[dim];
        if (sign(this->direction[dim]) < 0) {
            std::swap(mint, maxt);
        }
        t_enter = std::max(t_enter, mint);
        t_exit  = std::min(t_exit, maxt);
    }
    // debugm("t_enter is %.2f, t_exit is %.2f\n", t_enter, t_exit);
    return sign(t_exit - t_enter) >= 0 && sign(t_exit) > 0;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 01 2021, 23:54 [CST]
