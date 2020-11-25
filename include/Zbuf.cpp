#include "Zbuf.hpp"
#include "glm/gtc/type_ptr.hpp"

Zbuf::Zbuf() { init(); }
Zbuf::Zbuf(Scene const &s) : scene(s) { init(); }
Zbuf::Zbuf(Scene const &s, unsigned int const &width,
           unsigned int const &height)
    : scene(s) {
    init();
    init_viewport(width, height);
}

void Zbuf::init() {
    cam_initialized = mvp_initialized = viewport_initialized = false;
}

void Zbuf::init_cam(glm::vec3 const &ey, flt const &fovy,
                    flt const &aspect_ratio, flt const &znear,
                    flt const &zfar, glm::vec3 const &gaze,
                    glm::vec3 const &up) {
    cam.init(ey, fovy, aspect_ratio, znear, zfar, gaze, up);
    cam_initialized = true;
}

void Zbuf::init_mvp(glm::mat4 const &model) {
    if (!cam_initialized) {
        errorm("Camera position is not initilized\n");
    }
    // Set model matrix
    this->model = model;

    // Set view matrix, according to camera information
    glm::vec3 right = glm::cross(cam.gaze(), cam.up());
    // clang-format off
    flt view_value[] = {
              right.x,       right.y,       right.z, 0,
           cam.up().x,    cam.up().y,    cam.up().z, 0,
        -cam.gaze().x, -cam.gaze().y, -cam.gaze().z, 0,
                    0,             0,             0, 1,
    };
    // clang-format on
    view = glm::make_mat4(view_value);

    // Set projection matrix, according to fov, aspect ratio, etc.
    glm::mat4 persp_ortho; // Squeezes the frustum into a rectangular box
    glm::mat4 ortho_trans; // Centers the rectangular box at origin
    glm::mat4 ortho_scale; // Scales the rectangular box to the canonical cube
    flt       n            = cam.znear();
    flt       f            = cam.zfar();
    flt       fovy         = cam.fovy() * piover180;
    flt       aspect_ratio = cam.aspect_ratio();
    flt       screen_top   = std::tan(fovy / 2) * fabs(n);
    flt       screen_right = screen_top / aspect_ratio;
    // clang-format off
    flt po_value[] = { // values for persp_ortho
        n,    0,     0,      0,
        0,    n,     0,      0,
        0,    0, n + f, -n * f,
        0,    0,     1,      0,
    };
    flt ot_value[] = { // values for ortho_trans
        1, 0, 0,        0,
        0, 1, 0,        0,
        0, 0, 1, -(n+f)/2,
        0, 0, 0,        1,
    };
    flt os_value[] = { // values for ortho_scale
        1/screen_right,            0,           0, 0,
                     0, 1/screen_top,           0, 0,
                     0,            0, fabs(f-n)/2, 0,
                     0,            0,           0, 1
    };
    // clang-format on
    persp_ortho = glm::make_mat4(po_value);
    ortho_trans = glm::make_mat4(ot_value);
    ortho_scale = glm::make_mat4(os_value);
    projection  = ortho_scale * ortho_trans * persp_ortho;

    // Set mvp
    mvp             = projection * view * model;
    mvp_initialized = true;
}

void Zbuf::init_viewport(const unsigned int &width,
                         const unsigned int &height) {
    this->w = width;
    this->h = height;
    // clang-format off
    flt viewport_value[] = {
        w/2.0,     0, 0, 0,
            0, h/2.0, 0, 0,
            0,     0, 1, 0,
            0,     0, 0, 1,
    };
    // clang-format on
    viewport             = glm::make_mat4(viewport_value);
    viewport_initialized = true;
}

void Zbuf::naive() {
    if (!cam_initialized) {
        errorm("Camera position is not initilized\n");
    }
    if (!mvp_initialized) {
        errorm("Transformation matrices are not initialized\n");
    }
    if (!viewport_initialized) {
        errorm("Viewport size is not initialized\n");
    }

}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 24 2020, 12:15 [CST]
