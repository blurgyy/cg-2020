#pragma once

#include <functional>

#include "Camera.hpp"
#include "Pyramid.hpp"
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

    size_t w, h;     // Screen size, in pixels
    mat4   viewport; // Viewport transformation matrix, transforms the
                     // canonical cube to screen size on xOy plane
    bool viewport_initialized;

    Pyramid zpyramid;

  private:
    // Set default values
    void _init();
    // Check if screen space coordinate (x, y) is inside the triangle t,
    // coordinates of vertices of triangle t should be in screen space, too.
    bool inside(flt x, flt y, Triangle const &t) const;
    // Set image pixel at coordinate (x, y), origin is located at left-bottom
    // corner of the image.
    void set_pixel(size_t const &x, size_t const &y,
                   Color const &color = Color{255});
    // Active fragment shader function.  Triangle t has screen coordinates,
    // triangle v has view-space coordinates, barycentric is a tuple
    // consists of the 3 weights on each vertex
    std::function<Color(Triangle const &t, Triangle const &v,
                        std::tuple<flt, flt, flt> const &barycentric)>
        frag_shader;
    // Naive z-buffer implementation.
    // @param v: Triangle with **viewspace** coordinates
    void draw_triangle_with_aabb(Triangle const &v);
    // Use hierarchical z-buffer (depth MIP-map) to achieve ``early reject''.
    // @brief: Compare the triangle's nearest z value with the smallest
    //         QuadTree node's depth value, if the triangle's nearest z value
    //         is farther than current node's depth value, then this triangle
    //         can be safely ignored.
    //         If the triangle is not ignored, draw it with aabb.
    //         (todo: scan conversion).
    // @param v: Triangle with **viewspace** coordinates
    void draw_triangle_with_zpyramid(Triangle const &v);
    // Depth buffer value at image coordinate (x, y), origin is located at
    // left-bottom corner of the image.
    flt &      z(size_t const &x, size_t const &y);
    flt const &z(size_t const &x, size_t const &y) const;
    // Recurse octree from give node address, convert coordinates and render
    // on the fly.
    void _render(Node8 const *node);

  public:
    Image img;

  public:
    Zbuf();
    // Initialize a zbuffer object with given scene
    Zbuf(Scene const &s);
    // Initialize a zbuffer object with given scene and viewport size
    Zbuf(Scene const &s, size_t const &height, size_t const &width);

    // Set fragment shader
    void set_shader(
        std::function<Color(Triangle const &t, Triangle const &v,
                            std::tuple<flt, flt, flt> const &barycentric)>);
    // Set camera's {ex,in}trinsincs
    void init_cam(vec3 const &ey, flt const &fovy, flt const &aspect_ratio,
                  flt const &znear, flt const &zfar,
                  vec3 const &g = vec3{0, 0, -1},
                  vec3 const &u = vec3{0, 1, 0});
    // Use given camera
    void init_cam(Camera const &cam);
    // Set mvp transformation matrix
    // @param model: Model's transformation matrix, uses identity if not
    //               specified.
    void set_model_transformation(mat4 const &model = glm::identity<mat4>());
    // Set viewport transformation matrix
    void init_viewport(size_t const &height, size_t const &width);

    // Render scene
    void render();
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 24 2020, 12:14 [CST]
