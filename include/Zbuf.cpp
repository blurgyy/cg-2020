#include "Zbuf.hpp"
#include "glm/gtc/type_ptr.hpp"

Zbuf::Zbuf() { init(); }
Zbuf::Zbuf(Scene const &s) : scene(s) { init(); }
void Zbuf::init() { cam_initialized = false; }

void Zbuf::set_cam(glm::vec3 const &ey, flt const &fovy,
                   flt const &aspect_ratio, flt const &near, flt const &far,
                   glm::vec3 const &gaze, glm::vec3 const &up) {
    cam.init(ey, fovy, aspect_ratio, near, far, gaze, up);
    cam_initialized = true;
    // cam_initialized = cam.init(ey, fovy, aspect_ratio, near, far, gaze,
    // up);
}

void Zbuf::init_mvp(glm::mat4 const &_model) {
    if (!cam_initialized) {
        errorm("Camera position is not initilized\n");
    }
    // Set model matrix
    model = _model;

    /**/
    glm::vec3 right = glm::cross(cam.gaze(), cam.up());
    // clang-format off
    flt view_value[] = {
              right.x,       right.y,       right.z, 0,
           cam.up().x,    cam.up().y,    cam.up().z, 0,
        -cam.gaze().x, -cam.gaze().y, -cam.gaze().z, 0,
                    0,             0,             0, 1,
    };
    // clang-format on
    // Set view matrix, according to camera information
    view = glm::make_mat4(view_value);

    // Set projection matrix, according to fov, aspect ratio, etc.
    glm::vec4 persp_ortho; // Squeezes the frustum into a rectangular box
    glm::vec4 ortho_trans; // Centers the rectangular box at origin
    glm::vec4 ortho_scale; // Scales the rectangular box to the canonical cube
    // clang-format off
    flt n = cam.znear();
    flt f = cam.zfar();
    flt po_value[] = { // values for persp_ortho
        n,    0,     0,      0,
        0,    n,     0,      0,
        0,    0, n + f, -n * f,
        0,    0,     1,      0,
    };
    flt ot_value[] = { // values for ortho_trans

    };
    flt os_value[] = { // values for ortho_scale

    };
    // clang-format on
}

void Zbuf::naive() {}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 24 2020, 12:15 [CST]
