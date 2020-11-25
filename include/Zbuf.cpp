#include "Zbuf.hpp"
#include "glm/gtc/type_ptr.hpp"

Zbuf::Zbuf() { init(); }
Zbuf::Zbuf(Scene const &s) : scene(s) { init(); }
Zbuf::Zbuf(Scene const &s, unsigned int const &height,
           unsigned int const &width)
    : scene(s) {
    init();
    init_viewport(height, width);
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
    debugm("model matrix is\n");
    output(model);

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
    debugm("view matrix is\n");
    output(view);

    // Set projection matrix, according to fov, aspect ratio, etc.
    glm::mat4 persp_ortho; // Squeezes the frustum into a rectangular box
    glm::mat4 ortho_trans; // Centers the rectangular box at origin
    glm::mat4 ortho_scale; // Scales the rectangular box to the canonical cube
    flt       n    = cam.znear();
    flt       f    = cam.zfar();
    flt       fovy = cam.fovy() * piover180;
    debugm("cam.fovy is %f degrees, aka %f rad\n", cam.fovy(), fovy);
    flt aspect_ratio = cam.aspect_ratio();
    debugm("aspect ratios is %f\n", aspect_ratio);
    flt screen_top   = std::tan(fovy / 2) * fabs(n);
    flt screen_right = screen_top / aspect_ratio;
    debugm("screen top is %f, screen right is %f\n", screen_top,
           screen_right);
    // clang-format off
    flt po_value[] = { // values for persp_ortho
        n, 0,   0,    0,
        0, n,   0,    0,
        0, 0, n+f, -n*f,
        0, 0,   1,    0,
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
                     0,            0, 1/fabs(f-n), 0,
                     0,            0,           0, 1
    };
    // clang-format on
    persp_ortho = glm::make_mat4(po_value);
    ortho_trans = glm::make_mat4(ot_value);
    ortho_scale = glm::make_mat4(os_value);
    debugm("perspective to orthographic matrix is\n");
    output(persp_ortho);
    debugm("orthographic translation matrix is\n");
    output(ortho_trans);
    // projection = ortho_scale * ortho_trans * persp_ortho;
    projection = persp_ortho * ortho_trans * ortho_scale;
    debugm("orthographic scaling matrix is\n");
    output(ortho_scale);

    // Set mvp
    // mvp             = projection * view * model;
    mvp             = model * view * projection;
    mvp_initialized = true;
}

void Zbuf::init_viewport(const unsigned int &height,
                         const unsigned int &width) {
    this->h = height;
    this->w = width;
    // clang-format off
    // flt viewport_value[] = {
    // w/2.0,     0, 0, 0,
    // 0, h/2.0, 0, 0,
    // 0,     0, 1, 0,
    // 0,     0, 0, 1,
    // };
    flt vtrans_value[] = {
        1, 0, 0, 1,
        0, 1, 0, 1,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
    flt vscale_value[] = {
        w*0.5,     0, 0, 0,
            0, h*0.5, 0, 0,
            0,     0, 1, 0,
            0,     0, 0, 1,
    };
    // clang-format on
    glm::mat4 vtrans = glm::make_mat4(vtrans_value);
    glm::mat4 vscale = glm::make_mat4(vscale_value);
    // viewport         = vscale * vtrans;
    viewport = vtrans * vscale;
    debugm("viewport matrix is xxxxx\n");
    output(viewport);
    img.init(this->h, this->w);
    viewport_initialized = true;
}

void Zbuf::render() {
    if (!cam_initialized) {
        errorm("Camera position is not initilized\n");
    }
    if (!mvp_initialized) {
        errorm("Transformation matrices are not initialized\n");
    }
    if (!viewport_initialized) {
        errorm("Viewport size is not initialized\n");
    }
    for (Triangle const &t : scene.primitives()) {
        // If the triangle is facing -z direction, skip it (face culling).
        if (t.facing.z <= 0) {
            continue;
        }
        // Screen-space coordinates
        // Triangle o(viewport * mvp * t);
        Triangle o(t * mvp * viewport);
        debugm("viewport here is\n");
        output(viewport);
        debugm("mvp is\n");
        output(mvp);
        debugm("viewport @ mvp is\n");
        // output(viewport * mvp);
        output(mvp * viewport);
        debugm("original: (%.2f, %.2f), (%.2f, %.2f) (%.2f, %.2f)\n", t.a().x,
               t.a().y, t.b().x, t.b().y, t.c().x, t.c().y);
        debugm("screen space: (%.2f, %.2f), (%.2f, %.2f) (%.2f, %.2f)\n",
               o.a().x, o.a().y, o.b().x, o.b().y, o.c().x, o.c().y);
        // Draw triangle
        draw_triangle_naive(o);
    }
}

// private:
bool Zbuf::inside(flt x, flt y, Triangle const &t) const {
    // glm::vec2 pixel(x, y);
    glm::vec3 v[3];
    flt       z[3];
    v[0] = glm::vec3((t.a().x - x), (t.a().y - y), 0);
    v[1] = glm::vec3((t.b().x - x), (t.b().y - y), 0);
    v[2] = glm::vec3((t.c().x - x), (t.c().y - y), 0);
    // for (int i = 0; i < 3; ++i) {
    // debugm("checking vertex-%d\n", i);
    // output(v[i]);
    // }
    // errorm();
    z[0] = glm::cross(v[0], v[1]).z;
    z[1] = glm::cross(v[1], v[2]).z;
    z[2] = glm::cross(v[2], v[0]).z;
    // return glm::sign(z[1] * z[0]) == glm::sign(z[2] * z[0]);
    return glm::sign(z[0]) == glm::sign(z[1]) &&
           glm::sign(z[1]) == glm::sign(z[2]);
}

void Zbuf::set_pixel(unsigned int const &x, unsigned int const &y,
                     Color const &color) {
    // errorm("Setting pixel (%u, %u)\n", x, y);
    img(x, y) = color;
}

void Zbuf::draw_triangle_naive(Triangle const &t) {
    // todo: AABB
    // Brute force through all pixels
    for (int i = 0; i < this->h; ++i) {
        for (int j = 0; j < this->w; ++j) {
            // debugm("processing pixel (%d, %d)\n", i, j);
            // todo: AA
            if (inside(.5 + i, .5 + j, t)) {
                // debugm("pixel (%.2f, %.2f) is inside triangle\n",
                // (i+.5-.5*h)/h, (j+.5-.5*w)/w);
                debugm("pixel (%d, %d) is inside triangle\n", i, j);
                // char x;
                // scanf("%c", &x);
                set_pixel(i, j);
            }
        }
    }
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 24 2020, 12:15 [CST]
