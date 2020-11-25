#include "OBJ_Loader.hpp"
#include "Scene.hpp"
#include "Triangle.hpp"
#include "Zbuf.hpp"
#include "global.hpp"

#include <cstdio>
#include <cstring>

#include <glm/glm.hpp>
#include <opencv2/opencv.hpp>

void show_help(char *selfname) {
    printf("%s: zbuffer demo\n\n", selfname);
    printf("    usage: %s [-h|--help] <objfile>\n", selfname);
    printf("\n");
    printf("    options:\n");
    printf("        -h|--help:              Show this message and quit\n");
    printf("\n");
}

int main(int argc, char **argv) {
    std::string objfile; // Path to the obj file

    /*************************** Parse arguments ****************************/
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            show_help(argv[0]);
            return 0;
        } else {
            objfile = argv[i];
        }
    }
    if (objfile.size() == 0) {
        // errorm("No obj file specified\n");
        // return 1;
    }

    /****************************** Load model ******************************/
    // Uncomment below lines when testing is finished!
    //objl::Loader loader;
    //debugm("loading object from file '%s' ..\n", objfile.c_str());
    //if (!loader.LoadFile(objfile)) {
    //errorm("failed to load object from '%s'\n", objfile.c_str());
    //}
    //printf("object loaded\n");
    // // printf("%lu meshes loaded\n", loader.LoadedMeshes.size());

    /****************************** playground ******************************/
    // The single triangle to be rendered
    glm::vec3             v1(0, 2, -5);
    glm::vec3             v2(-1, -1, -5);
    glm::vec3             v3(0, 0, -5);
    Triangle              t(v1, v2, v3);
    std::vector<Triangle> prims{t};

    // Load the triangle into scene
    Scene scene(prims);

    // Screen (viewport) size
    int width  = 640;
    int height = 480;
    // Create a renderer on scene
    Zbuf zbuf(scene, height, width);
    // errorm("zbuf.img's size is height-%d, width-%d\n", zbuf.img.h,
    // zbuf.img.w);
    // Camera extrinsincs
    glm::vec3 eye(0, 0, 0);
    glm::vec3 gaze(0, 0, -1);
    glm::vec3 up(0, 1, 0);
    flt       fovy         = 45;
    flt       aspect_ratio = 1.0 * height / width;
    flt       znear        = -.1;
    flt       zfar         = -50;
    // Initialize camera
    zbuf.init_cam(eye, fovy, aspect_ratio, znear, zfar, gaze, up);

    zbuf.init_mvp(glm::identity<glm::mat4>()); // Use no model transformation

    zbuf.render();

    write_ppm("testfile.ppm", zbuf.img);

    return 0;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 18 2020, 17:33 [CST]
