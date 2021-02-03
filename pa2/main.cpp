#include "Camera.hpp"
#include "Scene.hpp"
#include "Screen.hpp"
#include "Triangle.hpp"
#include "global.hpp"

#include "tinyobjloader/tiny_obj_loader.h"

#include <filesystem>
#include <iostream>

int main(int argc, char **argv) {
    // Path to the obj file.
    std::string objfile{""};
    // Path to camera pose file.
    std::string camconf{"default.conf"};

    // Resolution (horizontal).
    std::size_t width = 64;
    // Resolution (vertical).
    std::size_t height = 64;

    // Field of view (in degrees).
    flt fovy = 45;

    // Sample(s) per pixel
    std::size_t spp = 16;

    /* [Parse arguments] */
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--config")) {
            ++i;
            if (i >= argc) {
                break;
            }
            camconf = std::string{argv[i]};
        } else {
            objfile = std::string{argv[i]};
        }
    }
    if (objfile.length() == 0) {
        errorm("No <model.obj> file specified\n");
    }
    /* [/Parse arguments] */
    flt aspect_ratio = static_cast<flt>(width) / static_cast<flt>(height);

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

    /* [Spawn Camera] */
    Camera camera(fovy, aspect_ratio);
    camera.load(camconf);
    /* [/Spawn Camera] */

    /* [Initialize screen] */
    Screen screen(width, height);
    /* [/Initialize screen] */

    screen.attach_scene(world);
    screen.set_cam(camera);
    screen.render(spp);

    write_ppm("x.ppm", screen.image());

    return 0;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Jan 29 2021, 15:32 [CST]
