#pragma once

#include "global.hpp"

#include "tinyobjloader/tiny_obj_loader.h"

#include <vector>

class Scene {
  private:
    std::vector<tinyobj::shape_t> shapes;

  public:
    Scene();
    Scene(std::vector<tinyobj::shape_t> const &loaded_shapes);

    std::vector<tinyobj::shape_t> const &primitives() const;
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Jan 31 2021, 21:11 [CST]
