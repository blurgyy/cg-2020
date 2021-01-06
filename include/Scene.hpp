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
        : children{nullptr},         // Initialize child nodes to nullptr
          mincord{xmin, ymin, zmin}, // Min values of current cube
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

    // Check if triangle `t` lies on any of the dividing planes of the cube
    // associated with current node.
    bool owns(Triangle const &t) {
        assert(this->mincord[0] < t.a().x && t.a().x < this->maxcord[0]);
        assert(this->mincord[0] < t.b().x && t.b().x < this->maxcord[0]);
        assert(this->mincord[0] < t.c().x && t.c().x < this->maxcord[0]);
        assert(this->mincord[1] < t.a().y && t.a().y < this->maxcord[1]);
        assert(this->mincord[1] < t.b().y && t.b().y < this->maxcord[1]);
        assert(this->mincord[1] < t.c().y && t.c().y < this->maxcord[1]);
        assert(this->mincord[2] < t.a().z && t.a().z < this->maxcord[2]);
        assert(this->mincord[2] < t.b().z && t.b().z < this->maxcord[2]);
        assert(this->mincord[2] < t.c().z && t.c().z < this->maxcord[2]);
        flt const &x = this->midcord[0];
        flt const &y = this->midcord[1];
        flt const &z = this->midcord[2];
        if (sign(t.a().x - x) != sign(t.b().x - x) ||
            sign(t.b().x - x) != sign(t.c().x - x) ||
            sign(t.a().y - y) != sign(t.b().y - y) ||
            sign(t.b().y - y) != sign(t.c().y - y) ||
            sign(t.a().z - z) != sign(t.b().z - z) ||
            sign(t.b().z - z) != sign(t.c().z - z)) {
            return true;
        }
        return false;
    }

    // When triangle `t` does not intersect any of the dividing planes of the
    // cube associated with current node, returns the index of the child that
    // `t` should be assigned to.
    // Returned index is an integer in range [0, 8], aka [000, 111].
    size_t index(Triangle const &t) {
        unsigned char masks[3];
        masks[0] = (t.a().x > this->midcord[0]);      // Lowest bit for x
        masks[1] = (t.a().y > this->midcord[1]) << 1; // Second bit for y
        masks[2] = (t.a().z > this->midcord[2]) << 2; // Third bit for z
        return masks[0] | masks[1] | masks[2];
    }

    // Father
    Node8 *fa;
    // Children
    std::array<Node8 *, 8> children;

    // Min values of current cube (0:x, 1:y, 2:z)
    std::array<flt, 3> mincord;
    // Max values of current cube (0:x, 1:y, 2:z)
    std::array<flt, 3> maxcord;
    // Splitting values (0:x, 1:y, 2:z)
    std::array<flt, 3> midcord;
    // 6 faces, 2 triangles per face.
    std::array<Triangle, 12> facets;
    // Associated primitives
    std::vector<Triangle> prims;
};

class Scene {
  private:
    // Triangles with real-world coordinates
    std::vector<Triangle> realworld_triangles;

    // Triangles with view-space coordinates
    std::vector<Triangle> viewspace_triangles;

  private:
    void _init();

    // This function is the frontend of octree construction.
    // It is called upon succesfully load of mesh triangles, the octree is
    // built upon all real world triangles.
    void _build_octree();

    // Actual octree recursive construction function
    Node8 *_build(flt const &xmin, flt const &ymin, flt const &zmin,
                  flt const &xmax, flt const &ymax, flt const &zmax,
                  std::vector<Triangle> const &prims, Node8 *fa);

  public:
    // Root node of object space octree
    Node8 *root;

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
