#pragma once

#include "global.hpp"

struct Material {
    Material();
    void output() const;

    // BRDF.
    vec3 fr(vec3 const &wi, vec3 const &wo, vec3 const &normal) const;

  public:
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emission;
    flt  shineness;
    bool has_emission;
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 03 2021, 16:27 [CST]
