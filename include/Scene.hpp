#pragma once

#include "OBJ_Loader.hpp"
#include "Triangle.hpp"
#include "global.hpp"

#include <array>
#include <vector>

// Node of a full octree.
struct Node8 : Node<8> {
    Node8(flt xmin = 0, flt ymin = 0, flt zmin = 0, flt xmax = 0,
          flt ymax = 0, flt zmax = 0)
        : mincord{xmin, ymin, zmin}, // Min values of current cube
          maxcord{xmax, ymax, zmax}  // Max values of current cube
    {
        for (int i = 0; i < 3; ++i) {
            midcord[i] = (mincord[i] + maxcord[i]) / 2;
        }

        /* Generate facets of corresponding cube of current node */
        // xOz
        facets[0] = Triangle{{
            vec3{xmin, ymin, zmin},
            vec3{xmax, ymin, zmin},
            vec3{xmax, ymin, zmax},
        }};
        facets[1] = Triangle{{
            vec3{xmin, ymin, zmin},
            vec3{xmax, ymin, zmax},
            vec3{xmin, ymin, zmax},
        }};
        facets[6] = Triangle{{
            vec3{xmin, ymax, zmin},
            vec3{xmax, ymax, zmax},
            vec3{xmax, ymax, zmin},
        }};
        facets[7] = Triangle{{
            vec3{xmin, ymax, zmin},
            vec3{xmin, ymax, zmax},
            vec3{xmax, ymax, zmax},
        }};

        // yOz
        facets[2] = Triangle{{
            vec3{xmin, ymin, zmin},
            vec3{xmin, ymin, zmax},
            vec3{xmin, ymax, zmax},
        }};
        facets[3] = Triangle{{
            vec3{xmin, ymin, zmin},
            vec3{xmin, ymax, zmax},
            vec3{xmin, ymax, zmin},
        }};
        facets[8] = Triangle{{
            vec3{xmax, ymin, zmin},
            vec3{xmax, ymax, zmax},
            vec3{xmax, ymin, zmax},
        }};
        facets[9] = Triangle{{
            vec3{xmax, ymin, zmin},
            vec3{xmax, ymax, zmin},
            vec3{xmax, ymax, zmax},
        }};

        // xOy
        facets[4]  = Triangle{{
            vec3{xmin, ymin, zmin},
            vec3{xmax, ymax, zmin},
            vec3{xmax, ymin, zmin},
        }};
        facets[5]  = Triangle{{
            vec3{xmin, ymin, zmin},
            vec3{xmin, ymax, zmin},
            vec3{xmax, ymax, zmin},
        }};
        facets[10] = Triangle{{
            vec3{xmin, ymin, zmax},
            vec3{xmax, ymin, zmax},
            vec3{xmax, ymax, zmax},
        }};
        facets[11] = Triangle{{
            vec3{xmin, ymin, zmax},
            vec3{xmax, ymax, zmax},
            vec3{xmin, ymax, zmax},
        }};
    }

    // Min values of current cube (0:x, 1:y, 2:z)
    std::array<flt, 3> mincord;
    // Max values of current cube (0:x, 1:y, 2:z)
    std::array<flt, 3> maxcord;
    // Splitting values (0:x, 1:y, 2:z)
    std::array<flt, 3> midcord;
    // 6 faces, 2 triangles per face.
    std::array<Triangle, 12> facets;
};

class Scene {
  private:
    // Triangles with real-world coordinates
    std::vector<Triangle> realworld_triangles;

    // Triangles with view-space coordinates
    std::vector<Triangle> viewspace_triangles;

  private:
    void _init();

    // This is called upon succesfully load of mesh triangles, the octree is
    // built upon all real world triangles.
    void build_octree();

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
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 23 2020, 15:34 [CST]
