#pragma once

#include "Triangle.hpp"

#include "tinyobjloader/tiny_obj_loader.h"

#include <vector>

class Scene {
  private:
    std::vector<Triangle>            tris;
    std::vector<tinyobj::material_t> mats;

  public:
    Scene();
    Scene(tinyobj::ObjReader const &loader);

    std::vector<Triangle> const &           triangles() const;
    std::vector<tinyobj::material_t> const &materials() const;
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Jan 31 2021, 21:11 [CST]
