#pragma once

#include "Triangle.hpp"

#include "tinyobjloader/tiny_obj_loader.h"

#include <vector>

class Scene {
  private:
    std::vector<Triangle> triangles;

  public:
    Scene();
    Scene(tinyobj::ObjReader const &loader);

    std::vector<Triangle> const &facets() const;
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Jan 31 2021, 21:11 [CST]
