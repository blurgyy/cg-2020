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
    tinyobj::ObjReader loader;
    loader.ParseFromFile(objfile);
    /* [/Read object file] */

    /* [Load model] */
    Scene world(loader);
    /* [/Load model] */

    msg("Loaded %zu triangles from %s\n", world.facets().size(),
        objfile.c_str());

    return 0;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Jan 29 2021, 15:32 [CST]
