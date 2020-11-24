#include "Zbuf.hpp"

Zbuf::Zbuf() {}
Zbuf::Zbuf(Scene const &s) : scene(s) {}

void Zbuf::set_eye(glm::vec3) {}
void Zbuf::set_model(glm::mat3) {}
void Zbuf::set_view(glm::mat3) {}
void Zbuf::set_projection(glm::mat3) {}

void Zbuf::naive() {}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 24 2020, 12:15 [CST]
