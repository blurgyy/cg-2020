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
           flt const &znear, flt const &zfar,
           glm::vec3 const &gaze = glm::vec3(0, 0, -1),
           glm::vec3 const &up   = glm::vec3(0, 1, 0));

    void init(glm::vec3 const &pos, flt const &fovy, flt const &aspect_ratio,
              flt const &znear, flt const &zfar,
              glm::vec3 const &gaze = glm::vec3(0, 0, -1),
              glm::vec3 const &up   = glm::vec3(0, 1, 0));

    // Get Position
    glm::vec3 pos() const;
    // Get Direction (gaze)
    glm::vec3 gaze() const;
    // Get Direction (up)
    glm::vec3 up() const;
    // Get Field of view (Vertical, in degrees)
    flt fovy() const;
    // Get \frac{height}{width}
    flt aspect_ratio() const;
    // Get Near clipping plane's z coordinate
    flt znear() const;
    // Get Far clipping plane's z coordinate
    flt zfar() const;
};

#endif

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 25 2020, 11:27 [CST]
