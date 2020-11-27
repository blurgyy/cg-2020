#ifndef ZBUF_HPP
#define ZBUF_HPP

#include <functional>

#include "Camera.hpp"
#include "Scene.hpp"
#include "global.hpp"

#include <glm/ext/matrix_transform.hpp>

class Zbuf {
  private:
    Scene scene; // Scene to be rendered

    Camera cam;
    bool   cam_initialized;

    mat4 model;      // Transform the model to the next timestep
    mat4 view;       // Transform the camera to origin, model moves with it
    mat4 projection; // M_{persp} @ M_{ortho}, where M_{ortho}
                     // "compresses" the scene into the canonical cube;
                     // M_{persp} performs perspective projection, '@'
                     // stands for matrix multiplication

    mat4 mvp; // mvp = projection @ view @ model, '@' stands for
              // matrix multiplication
    bool mvp_initialized;

    unsigned int w, h;     // Screen size, in pixels
    mat4         viewport; // Viewport transformation matrix, transforms the
                           // canonical cube to screen size on xOy plane
    bool viewport_initialized;

    std::vector<flt> depth_buffer;

  private:
    // Set default values
    void _init();
    // Check if screen space coordinate (x, y) is inside the triangle t,
    // coordinates of vertices of triangle t should be in screen space, too.
    bool inside(flt x, flt y, Triangle const &t) const;
    // Set image pixel at coordinate (x, y), origin is located at left-bottom
    // corner of the image.
    void set_pixel(unsigned int const &x, unsigned int const &y,
                   Color const &color = Color(255));
    // Active fragment shader function.  Triangle t has screen coordinates,
    // triangle v has view-space coordinates, (x, y) is the center of the
    // pixel to be shaded.
    std::function<Color(Triangle const &t, Triangle const &v, flt const &x,
                        flt const &y)>
        frag_shader;
    // Naive z-buffer implementation.
    // @param t: Triangle with screen space (x,y) coordinates
    // @param v: Triangle with view space coordinates
    void draw_triangle_naive(Triangle const &t, Triangle const &v);
    // Depth buffer value at image coordinate (x, y), origin is located at
    // left-bottom corner of the image.
    flt &      z(unsigned int const &x, unsigned int const &y);
    flt const &z(unsigned int const &x, unsigned int const &y) const;

  public:
    Image img;

  public:
    Zbuf();
    // Initialize a zbuffer object with given scene
    Zbuf(Scene const &s);
    // Initialize a zbuffer object with given viewport size
    Zbuf(Scene const &s, unsigned int const &height,
         unsigned int const &width);

    // Set fragment shader
    void set_shader(std::function<Color(Triangle const &t, Triangle const &v,
                                        flt const &x, flt const &y)>);
    // Set camera's {ex,in}trinsincs
    void init_cam(vec3 const &ey, flt const &fovy, flt const &aspect_ratio,
                  flt const &znear, flt const &zfar,
                  vec3 const &g = vec3(0, 0, -1),
                  vec3 const &u = vec3(0, 1, 0));
    // Use given camera
    void init_cam(Camera const &cam);
    // Set mvp transformation matrix
    // @param model: Model's transformation matrix, uses identity if not
    //               specified.
    void init_mvp(mat4 const &model = glm::identity<mat4>());
    // Set viewport transformation matrix
    void init_viewport(unsigned int const &height, unsigned int const &width);

    // Render scene
    void render();
};

#endif

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 24 2020, 12:14 [CST]
