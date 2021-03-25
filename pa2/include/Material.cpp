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
    vec3 f_lambert = this->diffuse / pi;

    vec3 h      = glm::normalize(wi + wo);
    flt  alpha2 = sq(this->roughness);
    flt  nh     = glm::dot(normal, h);
    flt  D      = alpha2 / (pi * sq(sq(nh) * (alpha2 - 1) + 1));

    flt F = this->fresnel();

    flt nv = glm::dot(normal, wo);
    flt nl = glm::dot(normal, wi);
    flt k  = sq(this->roughness + 1) / 8;
    flt Gv = nv / (nv * (1 - k) + k);
    flt Gl = nl / (nl * (1 - k) + k);
    flt G  = Gv * Gl;

    flt f_cook = D * F * G / (4 * nv * nl);

    return this->diffuse * f_lambert + this->specular * f_cook;
}

flt Material::fresnel() const { return 0.2; }

vec3 Material::sample_uniform(vec3 const &wo, vec3 const &normal) const {
    /* Uniformly sample the hemisphere */
    flt  phi   = std::acos(std::fabs(1 - 2 * uniform()));
    flt  theta = uniform() * twopi;
    vec3 local{
        std::sin(phi) * std::cos(theta),
        std::sin(phi) * std::sin(theta),
        std::cos(phi),
    };
    // Convert to view-space coordinates.
    return this->to_viewspace(local, normal);
}

/* Reference:
 *  - https://agraphicsguy.wordpress.com/2015/11/01/sampling-microfacet-brdf/
 */
vec3 Material::sample_importance(vec3 const &wo, vec3 const &normal) const {
    flt  r0 = uniform(), r1 = uniform();
    flt  alpha2 = sq(this->roughness);
    flt  phi    = std::acos(std::sqrt((1 - r0) / (r0 * (alpha2 - 1) + 1)));
    flt  theta  = twopi * r1;
    flt  x      = std::sin(phi) * std::cos(theta);
    flt  y      = std::sin(phi) * std::sin(theta);
    flt  z      = std::cos(phi);
    vec3 half   = this->to_viewspace(vec3(x, y, z), normal);
    vec3 wi     = 2 * glm::dot(half, wo) * half - wo;
    return wi;
}

vec3 Material::to_viewspace(vec3 const &local, vec3 const &normal) const {
    vec3 xaxis, yaxis;
    if (std::fabs(normal.x) > std::fabs(normal.y)) {
        flt invlen =
            1.0 / std::sqrt(normal.x * normal.x + normal.z * normal.z);
        xaxis = vec3{
            normal.z * invlen,
            0,
            -normal.x * invlen,
        };
    } else {
        flt invlen =
            1.0 / std::sqrt(normal.y * normal.y + normal.z * normal.z);
        xaxis = vec3{
            0,
            normal.z * invlen,
            -normal.y * invlen,
        };
    }

    yaxis = glm::cross(normal, xaxis);
    return local.x * xaxis + local.y * yaxis + local.z * normal;
}

flt Material::pdf(vec3 const &wi, vec3 const &wo, vec3 const &normal) const {
    if (sign(glm::dot(wi, normal) > 0) && sign(glm::dot(wo, normal)) > 0) {
        return 1.0 / twopi;
    } else {
        return 0;
    }
}

/* Reference:
 *  - https://agraphicsguy.wordpress.com/2015/11/01/sampling-microfacet-brdf/
 */
flt Material::pdf_importance(vec3 const &wi, vec3 const &wo,
                             vec3 const &normal) const {
    if (sign(glm::dot(wi, normal)) > 0 && sign(glm::dot(wo, normal)) > 0) {
        vec3 wm     = glm::normalize(wi + wo);
        flt  alpha2 = sq(this->roughness);
        flt  cophi  = glm::dot(normal, wm);
        flt  siphi  = std::sqrt(1 - sq(cophi));
        flt  p      = (2 * alpha2 * cophi * siphi) /
                sq(sq(cophi) * (alpha2 - 1) + 1) / (4 * glm::dot(wo, wm));

        return p;
    } else {
        return 0;
    }
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 03 2021, 16:27 [CST]
