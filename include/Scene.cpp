#include "Scene.hpp"
#include "global.hpp"

#include <algorithm>
#include <array>
#include <cstdio>

Scene::Scene() { this->_init(); }
Scene::Scene(objl::Mesh const &mesh) {
    this->_init();
    debugm("%lu vertices found in loaded mesh\n", mesh.Vertices.size());
    for (size_t i = 0; i < mesh.Vertices.size(); i += 3) {
        std::array<vec3, 3> verts;
        for (int j = 0; j < 3; ++j) {
            vec3 vertex(mesh.Vertices[i + j].Position.X,
                        mesh.Vertices[i + j].Position.Y,
                        mesh.Vertices[i + j].Position.Z);
            verts[j] = vertex;
        }
        this->realworld_triangles.emplace_back(verts[0], verts[1], verts[2]);
    }
    msg("Scene created with %lu triangles\n", realworld_triangles.size());
    this->_build_octree();
}
Scene::Scene(std::vector<Triangle> const &triangles)
    : realworld_triangles(triangles) {
    this->_init();
    this->_build_octree();
}

std::vector<Triangle> const &Scene::primitives() const {
    return this->viewspace_triangles;
}

void Scene::to_viewspace(mat4 const &mvp, vec3 const &cam_gaze) {
    for (auto const &t : this->realworld_triangles) {
        // If the triangle has same facing direction as camera's gaze
        // direction, skip it (face culling).
        if (glm::dot(cam_gaze, t.facing) >= 0) {
            continue;
        }
        // Triangle in viewspace
        Triangle v = t * mvp;
        for (int i = 0; i < 3; ++i) {
            // Push viewspace triangle only when it has 1 or more vertices
            // inside the canonical box $[-1, 1]^3$, aka view frustum culling.
            if (v.vert_in_canonical()) {
                viewspace_triangles.push_back(v);
                break;
            }
        }
    }
    debugm("real world: %zu triangles, viewspace: %zu triangles\n",
           this->realworld_triangles.size(),
           this->viewspace_triangles.size());
}

// private:

void Scene::_build_octree() {
    debugm("Constructing octree in object space ..\n");
    flt xmin{std::numeric_limits<flt>::max()}, ymin{xmin}, zmin{xmin};
    flt xmax{std::numeric_limits<flt>::min()}, ymax{xmax}, zmax{xmax};
    // Determine size of root node
    for (Triangle const &t : this->realworld_triangles) {
        xmin = std::min(std::min(xmin, t.a().x), std::min(t.b().x, t.c().x));
        ymin = std::min(std::min(ymin, t.a().y), std::min(t.b().y, t.c().y));
        zmin = std::min(std::min(zmin, t.a().z), std::min(t.b().z, t.c().z));
        xmax = std::max(std::max(xmax, t.a().x), std::max(t.b().x, t.c().x));
        ymax = std::max(std::max(ymax, t.a().y), std::max(t.b().y, t.c().y));
        zmax = std::max(std::max(zmax, t.a().z), std::max(t.b().z, t.c().z));
    }
    this->root = this->_build(xmin - epsilon, ymin - epsilon, zmin - epsilon,
                              xmax + epsilon, ymax + epsilon, zmax + epsilon,
                              this->realworld_triangles, nullptr);
    msg("Object space octree constructed\n");
}

Node8 *Scene::_build(flt const &xmin, flt const &ymin, flt const &zmin,
                     flt const &xmax, flt const &ymax, flt const &zmax,
                     std::vector<Triangle> const &prims, Node8 *fa) {
    // Do not create a node if there is no primitive inside given cubic area.
    if (prims.size() == 0) {
        return nullptr;
    }
    // Pointer to constructed octree node.
    Node8 *ret = new Node8{xmin, ymin, zmin, xmax, ymax, zmax};
    ret->fa    = fa;
    // Stop subdividing when number of primitives inside cube is less than 24.
    if (prims.size() < 24) {
        ret->isleaf = true;
        // Associate all primitives (less than 24) to current node.
        ret->prims.assign(prims.begin(), prims.end());
        return ret;
    }
    // Otherwise, subdivide current cube.
    std::array<std::vector<Triangle>, 8> subprims;
    for (auto const &t : prims) {
        if (ret->owns(t)) {
            ret->prims.push_back(t);
        } else {
            subprims[ret->index(t)].push_back(t);
        }
    }

    ret->children[0] =
        this->_build(xmin, ymin, zmin, ret->midcord[0], ret->midcord[1],
                     ret->midcord[2], subprims[0], ret);
    ret->children[1] =
        this->_build(ret->midcord[0], ymin, zmin, xmax, ret->midcord[1],
                     ret->midcord[2], subprims[1], ret);
    ret->children[2] =
        this->_build(xmin, ret->midcord[1], zmin, ret->midcord[0], ymax,
                     ret->midcord[2], subprims[2], ret);
    ret->children[3] =
        this->_build(ret->midcord[0], ret->midcord[1], zmin, xmax, ymax,
                     ret->midcord[2], subprims[3], ret);
    ret->children[4] =
        this->_build(xmin, ymin, ret->midcord[2], ret->midcord[0],
                     ret->midcord[1], zmax, subprims[4], ret);
    ret->children[5] =
        this->_build(ret->midcord[0], ymin, ret->midcord[2], xmax,
                     ret->midcord[1], zmax, subprims[5], ret);
    ret->children[6] =
        this->_build(xmin, ret->midcord[1], ret->midcord[2], ret->midcord[0],
                     ymax, zmax, subprims[6], ret);
    ret->children[7] =
        this->_build(ret->midcord[0], ret->midcord[1], ret->midcord[2], xmax,
                     ymax, zmax, subprims[7], ret);

    return ret;
}

void Scene::_init() { viewspace_triangles.clear(); }

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 23 2020, 15:38 [CST]
