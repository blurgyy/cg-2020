#include "Scene.hpp"
#include "global.hpp"

#include <array>
#include <cstdio>

Scene::Scene() {}
Scene::Scene(objl::Mesh const &mesh) {
    debugm("%lu vertices found in loaded mesh\n", mesh.Vertices.size());
    for (int i = 0; i < mesh.Vertices.size(); i += 3) {
        std::array<glm::vec3, 3> verts;
        for (int j = 0; j < 3; ++j) {
            glm::vec3 vertex(mesh.Vertices[i + j].Position.X,
                             mesh.Vertices[i + j].Position.Y,
                             mesh.Vertices[i + j].Position.Z);
            verts[j] = vertex;
        }
        this->triangles.emplace_back(verts[0], verts[1], verts[2]);
    }
    msg("Loaded %lu triangles into scene\n", triangles.size());
}
Scene::Scene(std::vector<Triangle> const &tgs) { this->triangles = tgs; }

std::vector<Triangle> const &Scene::primitives() const {
    return this->triangles;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 23 2020, 15:38 [CST]
