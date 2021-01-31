#include "Scene.hpp"

Scene::Scene() {}
Scene::Scene(std::vector<tinyobj::shape_t> const &loaded_shapes)
    : shapes(loaded_shapes) {}

/* public */
std::vector<tinyobj::shape_t> const &Scene::primitives() const {
    return this->shapes;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Jan 31 2021, 21:13 [CST]
