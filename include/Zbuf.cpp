#include "Zbuf.hpp"
#include "glm/gtc/type_ptr.hpp"

Zbuf::Zbuf() { this->_init(); }
Zbuf::Zbuf(Scene const &s) : scene{s} { this->_init(); }
Zbuf::Zbuf(Scene const &s, size_t const &height, size_t const &width)
    : scene{s} {
    this->_init();
    this->init_viewport(height, width);
}

void Zbuf::set_shader(
    std::function<Color(Triangle const &t, Triangle const &v,
                        std::tuple<flt, flt, flt> const &barycentric)>
        shader_func) {
    this->frag_shader = shader_func;
}

void Zbuf::init_cam(vec3 const &ey, flt const &fovy, flt const &aspect_ratio,
                    flt const &znear, flt const &zfar, vec3 const &gaze,
                    vec3 const &up) {
    this->cam.init(ey, fovy, aspect_ratio, znear, zfar, gaze, up);
    this->cam_initialized = true;
}
void Zbuf::init_cam(Camera const &camera) {
    this->cam             = camera;
    this->cam_initialized = true;
}

void Zbuf::set_model_transformation(mat4 const &model) {
    if (!this->cam_initialized) {
        errorm("Camera position is not initilized\n");
    }
    // Set model matrix
    this->model = model;
    // debugm("model matrix is\n");
    // output(model);

    // Set view matrix, according to camera information
    vec3 right = glm::normalize(glm::cross(cam.gaze(), cam.up()));
    // clang-format off
    flt trans_value[] = {
        1, 0, 0, -cam.pos().x,
        0, 1, 0, -cam.pos().y,
        0, 0, 1, -cam.pos().z,
        0, 0, 0,            1,
    };
    flt rot_value[] = {
              right.x,       right.y,       right.z, 0,
           cam.up().x,    cam.up().y,    cam.up().z, 0,
        -cam.gaze().x, -cam.gaze().y, -cam.gaze().z, 0,
                    0,             0,             0, 1,
    };
    // clang-format on
    this->view = glm::make_mat4(trans_value) * glm::make_mat4(rot_value);
    // debugm("view matrix is\n");
    // output(view);

    // Set projection matrix, according to fov, aspect ratio, etc.
    mat4 persp_ortho; // Squeezes the frustum into a rectangular box
    mat4 ortho_trans; // Centers the rectangular box at origin
    mat4 ortho_scale; // Scales the rectangular box to the canonical cube
    flt  n    = this->cam.znear();
    flt  f    = this->cam.zfar();
    flt  fovy = this->cam.fovy() * piover180;
    // debugm("cam.fovy is %f degrees, aka %f rad\n", cam.fovy(), fovy);
    flt aspect_ratio = this->cam.aspect_ratio();
    // debugm("aspect ratios is %f\n", aspect_ratio);
    flt screen_top   = std::tan(fovy / 2) * fabs(n);
    flt screen_right = screen_top / aspect_ratio;
    // debugm("screen top is %f, screen right is %f\n", screen_top,
    // screen_right);
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
    // debugm("perspective to orthographic matrix is\n");
    // output(persp_ortho);
    // debugm("orthographic translation matrix is\n");
    // output(ortho_trans);
    // projection = ortho_scale * ortho_trans * persp_ortho;
    this->projection = persp_ortho * ortho_trans * ortho_scale;
    // debugm("orthographic scaling matrix is\n");
    // output(ortho_scale);

    // Set mvp
    // mvp             = projection * view * model;
    this->mvp             = model * view * projection;
    this->mvp_initialized = true;
}

void Zbuf::init_viewport(const size_t &height, const size_t &width) {
    this->h = height;
    this->w = width;
    // clang-format off
    // flt viewport_value[] = {
    // w/2.0,     0, 0, 0,
    // 0, h/2.0, 0, 0,
    // 0,     0, 1, 0,
    // 0,     0, 0, 1,
    // };
    // Todo: use single initilization array
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
    mat4 vtrans = glm::make_mat4(vtrans_value);
    mat4 vscale = glm::make_mat4(vscale_value);
    // viewport         = vscale * vtrans;
    this->viewport = vtrans * vscale;
    // debugm("viewport matrix is xxxxx\n");
    // output(viewport);
    img.init(this->h, this->w);
    // Initilize the depth buffer, initial values are infinitely far (negative
    // infinity).
    // this->depth_buffer = std::vector<flt>(
    // this->h * this->w, -std::numeric_limits<double>::infinity());
    this->zpyramid             = Pyramid(this->h, this->w);
    this->viewport_initialized = true;
}

void Zbuf::render() {
    if (!this->cam_initialized) {
        errorm("Camera position is not initilized\n");
    }
    if (!this->mvp_initialized) {
        errorm("Transformation matrices are not initialized\n");
    }
    if (!this->viewport_initialized) {
        errorm("Viewport size is not initialized\n");
    }
    scene.to_viewspace(mvp, cam.gaze());
    for (Triangle const &v : scene.primitives()) {
        // Draw triangle
        // this->draw_triangle_with_aabb(v);
        this->draw_triangle_with_zpyramid(v);
    }
}

// private:
void Zbuf::_init() {
    this->cam_initialized      = false;
    this->mvp_initialized      = false;
    this->viewport_initialized = false;
    this->frag_shader          = nullptr;
}

bool Zbuf::inside(flt x, flt y, Triangle const &t) const {
    return t.contains(x, y);
}

void Zbuf::set_pixel(size_t const &x, size_t const &y, Color const &color) {
    // errorm("Setting pixel (%u, %u)\n", x, y);
    this->img(x, y) = color;
}

flt &Zbuf::z(size_t const &x, size_t const &y) {
    return this->zpyramid(x, y);
}

flt const &Zbuf::z(size_t const &x, size_t const &y) const {
    return this->zpyramid(x, y);
}

void Zbuf::draw_triangle_with_aabb(Triangle const &v) {
    // Triangle with screen-space coordinates
    Triangle t(v * viewport);
    // AABB
    int xmin = std::floor(std::min(t.a().x, std::min(t.b().x, t.c().x)));
    int xmax = std::ceil(std::max(t.a().x, std::max(t.b().x, t.c().x))) + 1;
    int ymin = std::floor(std::min(t.a().y, std::min(t.b().y, t.c().y)));
    int ymax = std::ceil(std::max(t.a().y, std::max(t.b().y, t.c().y))) + 1;
    xmin = clamp(xmin, 0, w - 1), xmax = clamp(xmax, 0, w - 1);
    ymin = clamp(ymin, 0, h - 1), ymax = clamp(ymax, 0, h - 1);
    for (int j = ymin; j < ymax; ++j) {
        for (int i = xmin; i < xmax; ++i) {
            // todo: AA
            flt x = .5 + i;
            flt y = .5 + j;
            if (this->inside(x, y, t)) {
                // Screen space barycentric coordinates of (x, y) inside
                // triangle t.
                std::tuple<flt, flt, flt> barycentric = t % vec3{x, y, 0};
                // unpack the barycentric coordinates
                auto [ca, cb, cc] = barycentric;
                // debugm("ca = %f, cb = %f, cc = %f\n", ca, cb, cc);
                // z value in view-space
                flt real_z = 1 / (ca / v.a().z + cb / v.b().z + cc / v.c().z);
                if (real_z > this->z(i, j)) {
                    // Calculate interpolated color with given triangle's 3
                    // vertices.
                    // Note: t and v shall have same color values by now.
                    // Color icol    = v.color_at(ca, cb, cc, real_z);
                    Color icol    = this->frag_shader(t, v, barycentric);
                    this->z(i, j) = real_z;
                    this->set_pixel(i, j, icol);
                }
            }
        }
    }
}

void Zbuf::draw_triangle_with_zpyramid(Triangle const &v) {
    // Triangle with screen-space coordinates
    Triangle t(v * viewport);
    if (this->zpyramid.visible(t, nullptr)) {
        // AABB
        int xmin = std::floor(std::min(t.a().x, std::min(t.b().x, t.c().x)));
        int xmax =
            std::ceil(std::max(t.a().x, std::max(t.b().x, t.c().x))) + 1;
        int ymin = std::floor(std::min(t.a().y, std::min(t.b().y, t.c().y)));
        int ymax =
            std::ceil(std::max(t.a().y, std::max(t.b().y, t.c().y))) + 1;
        xmin = clamp(xmin, 0, w), xmax = clamp(xmax, 0, w);
        ymin = clamp(ymin, 0, h), ymax = clamp(ymax, 0, h);
        for (int j = ymin; j < ymax; ++j) {
            for (int i = xmin; i < xmax; ++i) {
                // todo: AA
                flt x = .5 + i;
                flt y = .5 + j;
                if (this->inside(x, y, t)) {
                    // Screen space barycentric coordinates of (x, y) inside
                    // triangle t.
                    std::tuple<flt, flt, flt> barycentric = t % vec3{x, y, 0};
                    // unpack the barycentric coordinates
                    auto [ca, cb, cc] = barycentric;
                    // debugm("ca = %f, cb = %f, cc = %f\n", ca, cb, cc);
                    // z value in view-space
                    flt real_z =
                        1 / (ca / v.a().z + cb / v.b().z + cc / v.c().z);
                    if (real_z > this->z(i, j)) {
                        // Calculate interpolated color with given triangle's
                        // 3 vertices. Note: t and v shall have same color
                        // values by now. Color icol    = v.color_at(ca, cb,
                        // cc, real_z);
                        Color icol = this->frag_shader(t, v, barycentric);
                        this->zpyramid.setz(i, j, real_z);
                        this->set_pixel(i, j, icol);
                    }
                }
            }
        }
    }
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 24 2020, 12:15 [CST]
