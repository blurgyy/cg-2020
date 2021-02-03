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

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 03 2021, 16:27 [CST]
