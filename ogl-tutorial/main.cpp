#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> // math lib for 3D geometry
using namespace glm;

#include <cstdio>

int main() {
    // Initialize glfw
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return 1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);               // 4x AA
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL 3.3

    // Create window
    GLFWwindow *window;
    window = glfwCreateWindow(1024, 768, "Tutorial", nullptr, nullptr);

    if (nullptr == window) {
        fprintf(stderr, "Failed to create GLFWwindow\n");
        glfwTerminate();
        return 2;
    }

    // Initialize glew
    glfwMakeContextCurrent(window);
    glewExperimental = true;

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize glew\n");
        return 3;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    int cnt = 0;
    do {
        glfwSwapBuffers(window);
        // printf("%d-th frame\n", ++cnt);
        printf("Q %s pressed\n", glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS
                                     ? "is"
                                     : "is not");
        glfwPollEvents();
        // } while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
        // glfwWindowShouldClose(window) == 0);
    } while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == GL_FALSE);

    glfwTerminate();

    return 0;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 18 2020, 15:27 [CST]
