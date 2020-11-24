#ifndef SCENE_HPP
#define SCENE_HPP

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

    void build_octtree();
};

#endif

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 23 2020, 15:34 [CST]