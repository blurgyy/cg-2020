#include "Camera.hpp"

#include <fstream>
#include <sstream>

Camera::Camera() {}
Camera::Camera(flt const &aspect_ratio) : ar{aspect_ratio} {}
Camera::Camera(vec3 const &pos, flt const &fovy, flt const &aspect_ratio,
               flt const &znear, flt const &zfar, vec3 const &gaze,
               vec3 const &up)
    : e{pos}, g{glm::normalize(gaze)}, t{glm::normalize(up)}, // Extrinsincs
      fov{fovy}, ar{aspect_ratio},                            // Intrinsincs
      near{znear}, far{zfar}                                  // Intrinsincs
{
    this->_init_view_matrix();
}

void Camera::init(const vec3 &pos, const flt &fovy, const flt &aspect_ratio,
                  const flt &znear, const flt &zfar, vec3 const &gaze,
                  vec3 const &up) {
    // Extrinsincs
    this->e = pos;
    this->g = glm::normalize(gaze);
    this->t = glm::normalize(up);
    // Intrinsincs
    this->fov  = fovy;
    this->ar   = aspect_ratio;
    this->near = znear;
    this->far  = zfar;
}

void Camera::load(std::string const &configfile) {
    std::ifstream from(configfile);
    if (from.fail()) {
        errorm("Failed opening file '%s'\n", configfile.c_str());
    }
    vec3 position, lookat, up;
    flt  ffov;
    for (std::string curline; std::getline(from, curline);) {
        std::istringstream input(curline);
        std::string        token;
        flt                x, y, z;
        input >> token;
        if (token.length() == 0 || token[0] == '#') {
            continue;
        }
        if (token[0] == 'p' || token[0] == 'P') { // Position (eye)
            input >> position.x >> position.y >> position.z;
        } else if (token[0] == 'l' || token[0] == 'L') { // Look at (gaze)
            input >> lookat.x >> lookat.y >> lookat.z;
        } else if (token[0] == 'u' || token[0] == 'U') { // Up (top)
            input >> up.x >> up.y >> up.z;
        } else if (token[0] == 'f' || token[0] == 'F') { // Field of view
            input >> ffov;
        }
    }
    from.close();

    // Assign values.
    this->e   = position;
    this->g   = glm::normalize(lookat - position);
    this->t   = glm::normalize(up);
    this->fov = ffov;

    // Initialize world-to-camera transformation matrix.
    this->_init_view_matrix();
}

vec3 const &Camera::pos() const { return this->e; }
vec3 const &Camera::gaze() const { return this->g; }
vec3 const &Camera::up() const { return this->t; }
flt const & Camera::fovy() const { return this->fov; }
flt const & Camera::aspect_ratio() const { return this->ar; }
flt const & Camera::znear() const { return this->near; }
flt const & Camera::zfar() const { return this->far; }
mat4 const &Camera::view_matrix() const { return this->view; }

/* Private */

void Camera::_init_view_matrix() {
    vec3 right = glm::cross(this->gaze(), this->up());
    // clang-format off
    mat4 translation{
        1, 0, 0, -this->pos().x,
        0, 1, 0, -this->pos().y,
        0, 0, 1, -this->pos().z,
        0, 0, 0,              1,
    };
    mat4 rotation{
                right.x,         right.y,         right.z, 0,
           this->up().x,    this->up().y,    this->up().z, 0,
        -this->gaze().x, -this->gaze().y, -this->gaze().z, 0,
                      0,               0,               0, 1,
    };
    // clang-format on
    this->view = translation * rotation;
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 25 2020, 11:31 [CST]
