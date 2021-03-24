#pragma once

#include "global.hpp"

struct Material {
    Material();
    void output() const;

    // BRDF.
    vec3 fr(vec3 const &wi, vec3 const &wo, vec3 const &normal) const;

    // Fresnel
    flt fresnel() const;

    // Sample incoming light direction.
    vec3 sample_uniform(vec3 const &wo, vec3 const &normal) const;
    vec3 sample_importance(vec3 const &wo, vec3 const &normal) const;

    // Sample diffuse reflection direction.
    vec3 sample_diffuse(vec3 const &wo, vec3 const &normal) const;
    // Sample specular reflection direction.
    vec3 sample_specular(vec3 const &wo, vec3 const &normal) const;

    flt diffuse_amount() const;
    flt specular_amount() const;
    flt reflected_sum() const;

    // Convert locally sampled ray direction to viewspace coordinate,
    // according to a surface normal direction in viespace.
    vec3 to_viewspace(vec3 const &local, vec3 const &normal) const;

    // Get probability distribution function value of given incoming&outgoing
    // light directions.
    flt pdf(vec3 const &wi, vec3 const &wo, vec3 const &normal) const;
    flt pdf_importance(vec3 const &wi, vec3 const &wo,
                       vec3 const &normal) const;

  public:
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emission;
    flt  shineness;
    flt  roughness;
    bool has_emission;
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 03 2021, 16:27 [CST]
