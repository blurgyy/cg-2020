#include "Scene.hpp"
#include "global.hpp"

#include "tinyobjloader/tiny_obj_loader.h"

#include <iostream>

int main(int argc, char **argv) {
    std::string objfile{""};

    /* [Parse arguments] */
    for (int i = 1; i < argc; ++i) {
        objfile = std::string{argv[i]};
    }
    if (objfile.length() == 0) {
        errorm("No <model.obj> file specified\n");
    }
    /* [/Parse arguments] */

    /* [Read object file] */
    tinyobj::ObjReader loader;
    loader.ParseFromFile(objfile);
    /* [/Read object file] */

    /* [Load model] */
    auto const &attrib    = loader.GetAttrib();
    auto const &shapes    = loader.GetShapes();
    auto const &materials = loader.GetMaterials();
    for (tinyobj::shape_t const &shape : loader.GetShapes()) {
        std::size_t index_offset = 0;
        // Loop over faces in shape
        for (std::size_t fi = 0; fi < shape.mesh.num_face_vertices.size();
             ++fi) {
            std::size_t numverts = shape.mesh.num_face_vertices[fi];
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
            }
        }
    }
    /* [/Load model] */

    return 0;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Jan 29 2021, 15:32 [CST]
