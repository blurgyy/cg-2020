#include "Camera.hpp"
#include "Scene.hpp"
#include "Screen.hpp"
#include "Timer.hpp"
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
    std::size_t width = 784;
    // Resolution (vertical).
    std::size_t height = 784;

    // Field of view (in degrees).
    flt fovy = 45;

    // Sample(s) per pixel.
    std::size_t spp = 32;

    // Russian roulette.
    flt rr = 0.8;

    /* [Parse arguments] */
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--config")) {
            ++i;
            if (i >= argc) {
                break;
            }
            camconf = std::string{argv[i]};
        } else if (!strcmp(argv[i], "-r") ||
                   !strcmp(argv[i], "--resolution")) {
            ++i;
            if (i >= argc) {
                break;
            }
            width   = std::atoi(argv[i]);
            int len = strlen(argv[i]);
            for (int x = 0; x < len; ++x) {
                if (!std::isdigit(argv[i][x])) {
                    if (++x < len) {
                        height = std::atoi(argv[i] + x);
                    }
                }
            }
        } else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--spp")) {
            ++i;
            if (i >= argc) {
                break;
            }
            spp = std::atoi(argv[i]);
        } else if (!strcmp(argv[i], "-rr")) {
            ++i;
            if (i >= argc) {
                break;
            }
            rr = clamp(std::atof(argv[i]), 0, 1);
        } else {
            objfile = std::string{argv[i]};
        }
    }
    if (objfile.length() == 0) {
        errorm("No <model.obj> file specified\n");
    }
    // Summary
    msg("[ resolution: %zux%zu | spp: %zu | rr: %.3f ]\n", width, height, spp,
        rr);
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

    Timer timer;
    msg("Rendering scene ..\n");
    timer.start();
    screen.render(spp, rr);
    timer.end();
    msg("Elapsed %.2f ms \n", timer.elapsedms());

    write_ppm("x.ppm", screen.image());

    return 0;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Jan 29 2021, 15:32 [CST]
