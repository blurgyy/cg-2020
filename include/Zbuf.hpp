#ifndef ZBUF_HPP
#define ZBUF_HPP

#include "Scene.hpp"
#include "global.hpp"

#include <glm/glm.hpp>

class Zbuf {
  private:
    Scene scene; // Scene to be rendered

    glm::vec3 eye;   // Camera position
    glm::mat4 model; // Transform the model to the next timestep
    glm::mat4 view;  // Transform the camera to origin, model moves with it
    glm::mat4 projection; // M_{persp} @ M_{ortho}, where M_{ortho}
                          // "compresses" the scene into the canonical box;
                          // M_{persp} performs perspective projection.

  public:
    Zbuf();
    // Initialize a zbuffer object with given scene
    Zbuf(Scene const &s);

    void set_eye(glm::vec3);
    void set_model(glm::mat3);
    void set_view(glm::mat3);
    void set_projection(glm::mat3);

    void naive(); // Naive z-buffer
};

#endif

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 24 2020, 12:14 [CST]
