#include "Scene.hpp"
#include "Triangle.hpp"
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
    tinyobj::ObjReader       loader;
    tinyobj::ObjReaderConfig config;
    // Search from the same directory of the objfile.
    config.mtl_search_path = "";
    // Triangulate polygon.
    config.triangulate = true;
    // Parse vertex colors, fill default color when no color is present in the
    // objfile.
    config.vertex_color = true;
    loader.ParseFromFile(objfile, config);
    /* [/Read object file] */

    /* [Load model] */
    Scene world(loader);
    /* [/Load model] */

    msg("Loaded %zu triangles from %s\n", world.triangles().size(),
        objfile.c_str());

    for (Triangle const &t : world.triangles()) {
        msg("material id: %d\n", t.material());
    }

    return 0;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Jan 29 2021, 15:32 [CST]
