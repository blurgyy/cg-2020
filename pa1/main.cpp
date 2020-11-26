#include "OBJ_Loader.hpp"
#include "Scene.hpp"
#include "Triangle.hpp"
#include "Zbuf.hpp"
#include "global.hpp"
#include "shaders.hpp"

#include <cstdio>
#include <cstring>

#include <glm/glm.hpp>

void show_help(char *selfname) {
    printf("%s: zbuffer demo\n\n", selfname);
    printf("    usage: %s [-h|--help] <objfile>\n", selfname);
    printf("\n");
    printf("    options:\n");
    printf("        -h|--help               Show this message and quit\n");
    printf("        -n|--normal             Use normal fragment shader\n");
    printf("        -i|--interpolation      Use interpolation fragment "
           "shader\n");
    printf("\n");
}

int main(int argc, char **argv) {
    std::string objfile; // Path to the obj file
    std::function<Color(Triangle const &, Triangle const &, flt const &,
                        flt const &)>
        selected_fragment_shader = shdr::normal_shader;

    /*************************** Parse arguments ****************************/
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            show_help(argv[0]);
            return 0;
        } else if (!strcmp(argv[i], "-n") || !strcmp(argv[i], "--normal")) {
            selected_fragment_shader = shdr::normal_shader;
        } else if (!strcmp(argv[i], "-i") ||
                   !strcmp(argv[i], "--interpolation")) {
            selected_fragment_shader = shdr::vertex_interpolation_shader;
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
    // objl::Loader loader;
    // debugm("loading object from file '%s' ..\n", objfile.c_str());
    // if (!loader.LoadFile(objfile)) {
    // errorm("failed to load object from '%s'\n", objfile.c_str());
    //}
    // printf("object loaded\n");
    // Scene scene(loader.LoadedMeshes[0]);
    // printf("%lu meshes loaded\n", loader.LoadedMeshes.size());

    /****************************** playground ******************************/
    // The first triangle to be rendered
    glm::vec3 v11(0, 2, -6);
    glm::vec3 v12(-1, -1, -5);
    glm::vec3 v13(0, 0, -4);
    Triangle  t1(v11, v12, v13);
    t1.col[0] = Color(29, 135, 96);
    t1.col[1] = Color(209, 232, 5);
    t1.col[2] = Color(32, 192, 21);
    // The second triangle to be rendered
    glm::vec3 v21(-1, 1, -3);
    glm::vec3 v22(0, 0, -4);
    glm::vec3 v23(1, 0, -5);
    Triangle  t2(v21, v22, v23);
    t2.col[0] = Color(41, 23, 206);
    t2.col[1] = Color(160, 163, 83);
    t2.col[2] = Color(149, 149, 224);
    std::vector<Triangle> prims{t1, t2};

    // Load the triangle into scene
    Scene scene(prims);

    // Screen (viewport) size
    int width  = 640;
    int height = 480;
    // Create a renderer on scene
    Zbuf zbuf(scene, height, width);

    // Set fragment shader
    zbuf.set_shader(selected_fragment_shader);

    // Camera extrinsincs
    glm::vec3 eye(0, 0, 0);
    glm::vec3 gaze(0, 0, -1);
    glm::vec3 up(0, 1, 0);
    flt       fovy         = 45;
    flt       aspect_ratio = 1.0 * height / width;
    flt       znear        = -.1;
    flt       zfar         = -50;
    // Camera
    Camera camera(eye, fovy, aspect_ratio, znear, zfar, gaze, up);

    // Initialize camera
    zbuf.init_cam(camera);

    zbuf.init_mvp(glm::identity<glm::mat4>()); // Use no model transformation

    msg("Rendering scene ..\n");
    zbuf.render();

    write_ppm("testfile.ppm", zbuf.img);

    return 0;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 18 2020, 17:33 [CST]
