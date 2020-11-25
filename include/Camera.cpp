#include "Camera.hpp"

Camera::Camera() {}
Camera::Camera(glm::vec3 const &pos, flt const &fovy, flt const &aspect_ratio,
               flt const &near, flt const &far, glm::vec3 const &gaze,
               glm::vec3 const &up)
    : e(pos), g(gaze), t(up),      // Extrinsincs
      fov(fovy), ar(aspect_ratio), // Intrinsincs
      near(near), far(far)         // Intrinsincs
{}

void Camera::init(const glm::vec3 &pos, const flt &fovy,
                  const flt &aspect_ratio, const flt &near, const flt &far,
                  glm::vec3 const &gaze, glm::vec3 const &up) {
    // Extrinsincs
    e = pos;
    g = gaze;
    t = up;
    // Intrinsincs
    fov        = fovy;
    ar         = aspect_ratio;
    this->near = near;
    this->far  = far;
}

// Get
glm::vec3 Camera::pos() const { return e; }
glm::vec3 Camera::gaze() const { return g; }
glm::vec3 Camera::up() const { return t; }
flt       Camera::fovy() const { return fov; }
flt       Camera::aspect_ratio() const { return ar; }
flt       Camera::znear() const { return near; }
flt       Camera::zfar() const { return far; }

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 25 2020, 11:31 [CST]
