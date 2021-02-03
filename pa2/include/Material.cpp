#include "Material.hpp"

Material::Material()
    : ambient{0}, diffuse{0}, specular{0}, emission{0}, shineness{0},
      has_emission{false} {}

void Material::output() const {
    printf("ambient: ");
    ::output(this->ambient);
    printf("diffuse: ");
    ::output(this->diffuse);
    printf("specular: ");
    ::output(this->specular);
    printf("emission: ");
    ::output(this->emission);
    printf("has_emission: %s\n", this->has_emission ? "true" : "false");
    printf("shineness: %f\n", shineness);
}

vec3 Material::fr(vec3 const &wi, vec3 const &wo, vec3 const &normal) const {
    if (sign(glm::dot(wi, normal)) <= 0 || sign(glm::dot(wo, normal)) <= 0) {
        return vec3{0};
    }
    return this->diffuse / pi;
}

vec3 Material::sample(vec3 const &wo, vec3 const &normal) const {
    /* Uniformly sample the hemisphere */
    flt  z     = uniform();
    flt  theta = uniform() * twopi;
    flt  r     = std::sqrt(1 - z * z);
    vec3 local{r * std::cos(theta), r * std::sin(theta), z};
    // Convert to view-space coordinates.
    vec3 xaxis, yaxis;
    if (normal.x > normal.y) {
        flt invlen = 1.0 / (normal.x * normal.x + normal.z * normal.z);
        xaxis      = glm::cross(normal, vec3{
                                       normal.z * invlen,
                                       0,
                                       -normal.x * invlen,
                                   });
    } else {
        flt invlen = 1.0 / (normal.y * normal.y + normal.z * normal.z);
        xaxis      = glm::cross(normal, vec3{
                                       0,
                                       normal.z * invlen,
                                       -normal.y * invlen,
                                   });
    }

    yaxis = glm::cross(xaxis, normal);
    return local.x * xaxis + local.y * yaxis + local.z * normal;
}

flt Material::pdf(vec3 const &wi, vec3 const &wo, vec3 const &normal) const {
    if (sign(glm::dot(wo, normal) > 0)) {
        return 1.0 / twopi;
    } else {
        return 0;
    }
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 03 2021, 16:27 [CST]
