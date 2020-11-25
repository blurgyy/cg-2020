#include "Camera.hpp"

Camera::Camera() {}
Camera::Camera(glm::vec3 const &pos, flt const &fovy, flt const &aspect_ratio,
               flt const &znear, flt const &zfar, glm::vec3 const &gaze,
               glm::vec3 const &up)
    : e(pos), g(gaze), t(up),      // Extrinsincs
      fov(fovy), ar(aspect_ratio), // Intrinsincs
      near(znear), far(zfar)       // Intrinsincs
{}

void Camera::init(const glm::vec3 &pos, const flt &fovy,
                  const flt &aspect_ratio, const flt &znear, const flt &zfar,
                  glm::vec3 const &gaze, glm::vec3 const &up) {
    // Extrinsincs
    this->e = pos;
    this->g = gaze;
    this->t = up;
    // Intrinsincs
    this->fov  = fovy;
    this->ar   = aspect_ratio;
    this->near = znear;
    this->far  = zfar;
}

glm::vec3 Camera::pos() const { return e; }
glm::vec3 Camera::gaze() const { return g; }
glm::vec3 Camera::up() const { return t; }
flt       Camera::fovy() const { return fov; }
flt       Camera::aspect_ratio() const { return ar; }
flt       Camera::znear() const { return near; }
flt       Camera::zfar() const { return far; }

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 25 2020, 11:31 [CST]
