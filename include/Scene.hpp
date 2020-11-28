#pragma once

#include "OBJ_Loader.hpp"
#include "Triangle.hpp"

#include <vector>

class Scene {
  private:
    std::vector<Triangle> triangles;

  public:
    Scene();
    // Construct a scene with loaded mesh
    Scene(objl::Mesh const &mesh);
    // Construct a scene with a list of triangles
    Scene(std::vector<Triangle> const &tgs);

    std::vector<Triangle> const &primitives() const;

    void build_octtree();
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 23 2020, 15:34 [CST]
