#include "Scene.hpp"

Scene::Scene() {}
Scene::Scene(tinyobj::ObjReader const &loader) : mats(loader.GetMaterials()) {
    auto const &attrib = loader.GetAttrib();
    for (tinyobj::shape_t const &shape : loader.GetShapes()) {
        std::size_t index_offset = 0;
        // Loop over faces in shape
        for (std::size_t fi = 0; fi < shape.mesh.num_face_vertices.size();
             ++fi) {
            std::size_t numverts = shape.mesh.num_face_vertices[fi];
            if (numverts != 3) {
                errorm("Non-triangular mesh is unhandled\n");
            }
            std::array<vec3, 3> vtx, nor;
            std::array<vec2, 3> tex;
            for (std::size_t v = 0; v < numverts; ++v) {
                // Access vertices
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                flt vx = attrib.vertices[3 * idx.vertex_index + 0];
                flt vy = attrib.vertices[3 * idx.vertex_index + 1];
                flt vz = attrib.vertices[3 * idx.vertex_index + 2];
                flt nx = attrib.normals[3 * idx.vertex_index + 0];
                flt ny = attrib.normals[3 * idx.vertex_index + 1];
                flt nz = attrib.normals[3 * idx.vertex_index + 2];
                flt tu = attrib.texcoords[2 * idx.vertex_index + 0];
                flt tv = attrib.texcoords[2 * idx.vertex_index + 1];
                vtx[v] = vec3{vx, vy, vz};
                nor[v] = vec3{nx, ny, nz};
                tex[v] = vec2{tu, tv};
            }
            index_offset += numverts;
            int      matid = shape.mesh.material_ids[fi];
            Triangle newtri{vtx, nor, tex};
            newtri.set_material(matid);
            this->tris.push_back(newtri);
        }
    }
}

std::vector<Triangle> const &Scene::triangles() const { return this->tris; }
std::vector<tinyobj::material_t> const &Scene::materials() const {
    return this->mats;
}

/* public */

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Jan 31 2021, 21:13 [CST]
