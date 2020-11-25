#ifndef ZBUF_HPP
#define ZBUF_HPP

#include "Camera.hpp"
#include "Scene.hpp"
#include "global.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

class Zbuf {
  private:
    Scene scene; // Scene to be rendered

    Camera cam;
    bool   cam_initialized;

    glm::mat4 model; // Transform the model to the next timestep
    glm::mat4 view;  // Transform the camera to origin, model moves with it
    glm::mat4 projection; // M_{persp} @ M_{ortho}, where M_{ortho}
                          // "compresses" the scene into the canonical cube;
                          // M_{persp} performs perspective projection, '@'
                          // stands for matrix multiplication

    glm::mat4 mvp; // mvp = projection @ view @ model, '@' stands for
                   // matrix multiplication
    bool mvp_initialized;

    unsigned int w, h;     // Screen size, in pixels
    glm::mat4    viewport; // Viewport transformation matrix, transforms the
                           // canonical cube to screen size on xOy plane
    bool viewport_initialized;

  public:
    Zbuf();
    // Initialize a zbuffer object with given scene
    Zbuf(Scene const &s);
    // Initialize a zbuffer object with given viewport size
    Zbuf(Scene const &s, unsigned int const &width,
         unsigned int const &height);
    // Set default values
    void init();

    // Set camera's {ex,in}trinsincs
    void init_cam(glm::vec3 const &ey, flt const &fovy,
                  flt const &aspect_ratio, flt const &znear, flt const &zfar,
                  glm::vec3 const &g = glm::vec3(0, 0, -1),
                  glm::vec3 const &u = glm::vec3(0, 1, 0));
    // Set mvp transformation matrix
    // @param model: Model's transformation matrix, uses identity if not
    //               specified.
    void init_mvp(glm::mat4 const &model = glm::identity<glm::mat4>());
    // Set viewport transformation matrix
    void init_viewport(unsigned int const &width, unsigned int const &height);

    void naive(); // Naive z-buffer
};

#endif

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 24 2020, 12:14 [CST]
