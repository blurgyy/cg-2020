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
    printf("    usage:\n");
    printf("        -h|--help               print this message\n");
    printf(
        "        -o|--object <file>      load .obj file stored in <file>\n");
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
        fprintf(stderr, " [x] No obj file specified\n");
        return 1;
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
    // Scene scene(loader.LoadedMeshes[0]);
    glm::vec3 v1(-1, -1, -1);
    glm::vec3 v2(0, 2, -1);
    glm::vec3 v3(1, 0, -1);
    Triangle  t(v1, v2, v3);

    std::vector<Triangle> prims{t};
    Scene                 scene(prims);

    Zbuf zbuf(scene);

    zbuf.naive();

    return 0;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 18 2020, 17:33 [CST]
