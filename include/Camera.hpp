#ifndef CAM_HPP
#define CAM_HPP

#include "global.hpp"
#include <glm/glm.hpp>

class Camera {
  private:
    // Extrinsincs
    glm::vec3 e; // Position
    glm::vec3 g; // Direction (gaze)
    glm::vec3 t; // Direction (up)

    // Intrinsincs
    flt fov;       // Field of view (Vertical, in degrees)
    flt ar;        // \frac{height}{width}
    flt near, far; // Near and far clipping plane's z coordinates

  public:
    Camera();
    Camera(glm::vec3 const &pos, flt const &fovy, flt const &aspect_ratio,
           flt const &near, flt const &far,
           glm::vec3 const &gaze = glm::vec3(0, 0, -1),
           glm::vec3 const &up   = glm::vec3(0, 1, 0));

    void init(glm::vec3 const &pos, flt const &fovy, flt const &aspect_ratio,
              flt const &near, flt const &far,
              glm::vec3 const &gaze = glm::vec3(0, 0, -1),
              glm::vec3 const &up   = glm::vec3(0, 1, 0));

    // Get
    glm::vec3 pos() const;
    glm::vec3 gaze() const;
    glm::vec3 up() const;
    flt       fovy() const;
    flt       aspect_ratio() const;
    flt       znear() const;
    flt       zfar() const;
};

#endif

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 25 2020, 11:27 [CST]
