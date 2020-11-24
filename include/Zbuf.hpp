#ifndef ZBUF_HPP
#define ZBUF_HPP

#include "Scene.hpp"
#include "global.hpp"

#include <glm/glm.hpp>

class Zbuf {
  private:
    Scene scene; // Scene to be rendered

    glm::vec3 eye;  // Camera position
    glm::vec3 gaze; // Camera's gaze direction
    glm::vec3 up;   // Camera's top direction
    bool      eye_initialized;

    glm::mat4 model; // Transform the model to the next timestep
    glm::mat4 view;  // Transform the camera to origin, model moves with it
    glm::mat4 projection; // M_{persp} @ M_{ortho}, where M_{ortho}
                          // "compresses" the scene into the canonical box;
                          // M_{persp} performs perspective projection.

    glm::mat4 mvp; // mvp = projection @ view @ model,
                   // where '@' stands for matrix production
  public:
    Zbuf();
    // Initialize a zbuffer object with given scene
    Zbuf(Scene const &s);
    // Set default values
    void init();

    void set_cam(glm::vec3 const &ey, glm::vec3 const &g, glm::vec3 const &u);

    // Set mvp transformation matrix
    void init_mvp(glm::mat4 const &_model = glm::mat4(0));

    void naive(); // Naive z-buffer
};

#endif

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 24 2020, 12:14 [CST]
