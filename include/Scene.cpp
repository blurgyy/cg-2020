#include "Scene.hpp"
#include "global.hpp"

#include <array>
#include <cstdio>

Scene::Scene() {}
Scene::Scene(objl::Mesh const &mesh) {
    debugm("%lu vertices found in loaded mesh\n", mesh.Vertices.size());
    for (int i = 0; i < mesh.Vertices.size(); i += 3) {
        std::array<glm::vec3, 3> face;
        for (int j = 0; j < 3; ++j) {
            glm::vec3 vertex(mesh.Vertices[i + j].Position.X,
                             mesh.Vertices[i + j].Position.Y,
                             mesh.Vertices[i + j].Position.Z);
            face[j] = vertex;
        }
        triangles.emplace_back(face[0], face[1], face[2]);
    }
    printf("%lu triangles loaded into scene\n", triangles.size());
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 23 2020, 15:38 [CST]
