#include "Camera.hpp"
#include "Scene.hpp"
#include "Triangle.hpp"
#include "global.hpp"

#include "tinyobjloader/tiny_obj_loader.h"

#include <filesystem>
#include <iostream>

int main(int argc, char **argv) {
    std::string objfile{""};
    std::string camconf{"default.conf"};
    int         width = 1920, height = 1080;
    flt         fovy = 45;

    /* [Parse arguments] */
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--config")) {
            ++i;
            if (i >= argc) {
                break;
            }
            camconf = std::string{argv[i]};
        }
        objfile = std::string{argv[i]};
    }
    if (objfile.length() == 0) {
        errorm("No <model.obj> file specified\n");
    }
    /* [/Parse arguments] */
    flt aspect_ratio = static_cast<flt>(height) / static_cast<flt>(width);

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

    /* [Init Camera] */
    Camera cam(fovy, aspect_ratio);
    cam.load(camconf);
    msg("Cam pos: ");
    output(cam.pos());
    msg("Cam gaze: ");
    output(cam.gaze());
    msg("Cam up: ");
    output(cam.up());
    /* [/Init Camera] */

    return 0;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Jan 29 2021, 15:32 [CST]
