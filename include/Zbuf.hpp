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
                          // M_{persp} performs perspective projection.

    glm::mat4 mvp; // mvp = projection @ view @ model,
                   // where '@' stands for matrix production
  public:
    Zbuf();
    // Initialize a zbuffer object with given scene
    Zbuf(Scene const &s);
    // Set default values
    void init();

    void set_cam(glm::vec3 const &ey, flt const &fovy,
                 flt const &aspect_ratio, flt const &near, flt const &far,
                 glm::vec3 const &g = glm::vec3(0, 0, -1),
                 glm::vec3 const &u = glm::vec3(0, 1, 0));

    // Set mvp transformation matrix
    // @param _model: Model's transformation matrix, uses identity if not
    //                specified
    void init_mvp(glm::mat4 const &_model = glm::identity<glm::mat4>());

    void naive(); // Naive z-buffer
};

#endif

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 24 2020, 12:14 [CST]
