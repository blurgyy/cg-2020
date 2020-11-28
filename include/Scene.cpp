#include "Scene.hpp"
#include "global.hpp"

#include <array>
#include <cstdio>

Scene::Scene() { _init(); }
Scene::Scene(objl::Mesh const &mesh) {
    _init();
    debugm("%lu vertices found in loaded mesh\n", mesh.Vertices.size());
    for (int i = 0; i < mesh.Vertices.size(); i += 3) {
        std::array<vec3, 3> verts;
        for (int j = 0; j < 3; ++j) {
            vec3 vertex(mesh.Vertices[i + j].Position.X,
                        mesh.Vertices[i + j].Position.Y,
                        mesh.Vertices[i + j].Position.Z);
            verts[j] = vertex;
        }
        this->realworld_triangles.emplace_back(verts[0], verts[1], verts[2]);
    }
    msg("Loaded %lu triangles into scene\n", realworld_triangles.size());
}
Scene::Scene(std::vector<Triangle> const &triangles)
    : realworld_triangles(triangles) {
    _init();
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
        viewspace_triangles.push_back(t * mvp);
    }
    debugm("real world: %zu triangles, viewspace: %zu triangles\n",
           this->realworld_triangles.size(),
           this->viewspace_triangles.size());
}

// private:
void Scene::_init() { viewspace_triangles.clear(); }

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 23 2020, 15:38 [CST]
