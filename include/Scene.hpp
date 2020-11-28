#pragma once

#include "OBJ_Loader.hpp"
#include "Triangle.hpp"

#include <vector>

class Scene {
  private:
    // Triangles with real-world coordinates
    std::vector<Triangle> realworld_triangles;

    // Triangles with view-space coordinates
    std::vector<Triangle> viewspace_triangles;

  private:
    void _init();

  public:
    Scene();
    // Construct a scene with loaded mesh
    Scene(objl::Mesh const &mesh);
    // Construct a scene with a list of triangles
    Scene(std::vector<Triangle> const &tgs);

    std::vector<Triangle> const &primitives() const;

    // Transform loaded triangles into viewspace, in viewspace, the observer
    // (camera) rests at position (0, 0, 0) and has gaze direction (0, 0, -1),
    // with up direction (0, 1, 0).
    // @param      mvp: Model-view-projection matrix
    // @param cam_gaze: Camera's gaze direction for face culling
    void to_viewspace(mat4 const &mvp, vec3 const &cam_gaze);

    void build_octtree();
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 23 2020, 15:34 [CST]
