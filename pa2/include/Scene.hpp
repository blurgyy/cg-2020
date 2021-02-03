#pragma once

#include "Camera.hpp"
#include "Ray.hpp"
#include "Triangle.hpp"

#include "tinyobjloader/tiny_obj_loader.h"

#include <algorithm>
#include <vector>

// Node for constructing BVH.
struct BVHNode : Node<2> {
  public:
    BBox bbox;
    flt  area;

    BVHNode *fa;

    Triangle const *tri;

    std::array<BVHNode *, 2> children;

  public:
    BVHNode() : area{0}, tri{nullptr}, children{nullptr} {}

    void build(std::vector<Triangle> tris, BVHNode *fa = nullptr) {
        if (fa) {
            this->tdep = fa->tdep + 1;
        }
        /* Naive */
        if (tris.size() == 1) {
            this->tri = new Triangle(tris[0]);
            this->bbox |= tris[0].boundingbox();
            // this->area += t->area();
            this->area   = tris[0].doublearea();
            this->isleaf = true;
            return;
        }
        if (tris.size() == 2) {
            this->children[0] = new BVHNode();
            this->children[1] = new BVHNode();
            this->children[0]->build(std::vector<Triangle>{tris[0]}, this);
            this->children[1]->build(std::vector<Triangle>{tris[1]}, this);
            this->bbox = this->children[0]->bbox | this->children[1]->bbox;
            this->area = this->children[0]->area + this->children[1]->area;
            return;
        }
        BBox centroid_box;
        for (Triangle const &t : tris) {
            centroid_box |= t.boundingbox().centroid();
        }
        // Split along the direction that has the maximum extent.
        auto begining = tris.begin();
        auto ending   = tris.end();
        auto middling = begining + tris.size() * 0.5;
        switch (centroid_box.max_dir()) {
        case 0:
            std::nth_element(begining, middling, ending,
                             [](Triangle const &t1, Triangle const &t2) {
                                 return t1.boundingbox().centroid().x <
                                        t2.boundingbox().centroid().x;
                             });
            break;
        case 1:
            std::nth_element(begining, middling, ending,
                             [](Triangle const &t1, Triangle const &t2) {
                                 return t1.boundingbox().centroid().y <
                                        t2.boundingbox().centroid().y;
                             });
            break;
        case 2:
            std::nth_element(begining, middling, ending,
                             [](Triangle const &t1, Triangle const &t2) {
                                 return t1.boundingbox().centroid().z <
                                        t2.boundingbox().centroid().z;
                             });
            break;
        default:
            errorm("?\n");
        }
        this->children[0] = new BVHNode();
        this->children[1] = new BVHNode();
        this->children[0]->build(std::vector<Triangle>(begining, middling),
                                 this);
        this->children[1]->build(std::vector<Triangle>(middling, ending),
                                 this);
        this->bbox = this->children[0]->bbox | this->children[1]->bbox;
        this->area = this->children[0]->area + this->children[1]->area;
    }

    Intersection intersect(Ray const &ray) const {
        Intersection ret;
        // If ray does not intersect with bbox of node:
        if (!ray.intersect(this->bbox)) {
            return ret;
        }
        // If this node is a leaf node:
        if (this->isleaf) {
            return ray.intersect(this->tri);
        }
        // if this node is an intermediate node:
        Intersection l = this->children[0]->intersect(ray);
        Intersection r = this->children[1]->intersect(ray);

        ret = l;
        if (r.occurred && r.distance < ret.distance) {
            ret = r;
        }

        return ret;
    }
};

class Scene {
  private:
    std::vector<Triangle> orig_tris;
    std::vector<Triangle> tris;

    std::vector<tinyobj::material_t> mats;

    BVHNode *root;

  public:
    Scene();
    Scene(tinyobj::ObjReader const &loader);

    void to_camera_space(Camera const &cam);

    void build_BVH();

    Intersection intersect(Ray const &ray) const;

    std::vector<Triangle> const &           triangles() const;
    std::vector<tinyobj::material_t> const &materials() const;
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Jan 31 2021, 21:11 [CST]
