#include "Camera.hpp"
#include "Scene.hpp"
#include "Screen.hpp"
#include "Timer.hpp"
#include "Triangle.hpp"
#include "global.hpp"

#include "tinyobjloader/tiny_obj_loader.h"

#include <csignal>
#include <filesystem>
#include <iostream>
#include <thread>

int main(int argc, char **argv) {
    // Path to the obj file.
    std::string objmodel{""};
    // Path to camera pose file.
    std::string camconf{""};
    // Path to skybox image file.
    std::string skyboximg{""};

    // Resolution (horizontal).
    std::size_t width = 1920;
    // Resolution (vertical).
    std::size_t height = 1080;

    // Russian roulette probability.
    flt rr = 0.85;

    // Gamma.
    flt gamma = 0.5;

    /* [Parse arguments] */
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--config")) {
            ++i;
            if (i >= argc) {
                break;
            }
            camconf = std::string{argv[i]};
        } else if (!strcmp(argv[i], "-k") || !strcmp(argv[i], "--skybox")) {
            ++i;
            if (i >= argc) {
                break;
            }
            skyboximg = std::string{argv[i]};
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
        } else if (!strcmp(argv[i], "-rr")) {
            ++i;
            if (i >= argc) {
                break;
            }
            rr = clamp(std::atof(argv[i]), 0, 1);
        } else if (!strcmp(argv[i], "-g") || !strcmp(argv[i], "--gamma")) {
            ++i;
            if (i >= argc) {
                break;
            }
            gamma = std::atof(argv[i]);
        } else {
            objmodel = std::string{argv[i]};
        }
    }
    if (objmodel.length() == 0) {
        errorm("No <model.obj> file specified\n");
    }
    if (camconf.length() == 0) {
        errorm("No <camera.conf> file specified\n");
    }
    if (skyboximg.length() > 0 &&
        !std::filesystem::is_regular_file(skyboximg)) {
        errorm("The given <skybox> path is not valid\n");
    }
    // Summary
    msg("Summary:\n"
        "          model: '%s'\n"
        "         camera: '%s'\n"
        "         skybox: '%s'\n"
        "     resolution: %zux%zu\n"
        "             rr: %.2f\n"
        "          gamma: %.2f\n",
        objmodel.c_str(), camconf.c_str(), skyboximg.c_str(), width, height,
        rr, gamma);
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
    loader.ParseFromFile(objmodel, config);
    /* [/Read object file] */

    /* [Load model] */
    Scene world(loader);
    /* [/Load model] */

    /* [Setup scene] */
    if (skyboximg.length() > 0) {
        world.load_skybox(skyboximg);
    }
    /* [/Setup scene] */

    /* [Spawn Camera] */
    Camera camera(aspect_ratio);
    camera.load(camconf);
    /* [/Spawn Camera] */

    /* [Initialize screen] */
    Screen screen(width, height);
    screen.set_gamma(gamma);
    /* [/Initialize screen] */

    screen.attach_scene(world);
    screen.set_cam(camera);

    msg("Rendering scene ..\n");
    screen.render(rr, std::filesystem::path(objmodel)
                              .filename()
                              .replace_extension()
                              .string() +
                          ".ppm");

    return 0;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Jan 29 2021, 15:32 [CST]
