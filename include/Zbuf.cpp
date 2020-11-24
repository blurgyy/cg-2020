#include "Zbuf.hpp"
#include "glm/gtc/type_ptr.hpp"

Zbuf::Zbuf() { init(); }
Zbuf::Zbuf(Scene const &s) : scene(s) { init(); }
void Zbuf::init() { eye_initialized = false; }

void Zbuf::set_cam(glm::vec3 const &ey, glm::vec3 const &g,
                   glm::vec3 const &u) {
    eye             = ey;
    gaze            = g;
    up              = u;
    eye_initialized = true;
}

void Zbuf::init_mvp(glm::mat4 const &_model) {
    if (!eye_initialized) {
        errorm("Camera position is not initilized\n");
    }
    // Set model matrix
    model = _model;

    /**/
    glm::vec3 right = glm::cross(gaze, up);
    // clang-format off
    double view_value[] = {
        right.x, right.y, right.z, 0,
           up.x,    up.y,    up.z, 0,
        -gaze.x, -gaze.y, -gaze.z, 0,
              0,       0,       0, 1,
    };
    // clang-format on
    // Set view matrix, according to camera information
    view = glm::make_mat4(view_value);

    // Set projection matrix, according to fov, aspect ratio, etc.
    // projection =
}

void Zbuf::naive() {}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 24 2020, 12:15 [CST]
