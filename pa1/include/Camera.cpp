#include "Camera.hpp"

#include <fstream>
#include <sstream>

Camera::Camera() {}
Camera::Camera(flt const &fovy, flt const &aspect_ratio)
    : fov{fovy}, ar{aspect_ratio} {}
Camera::Camera(vec3 const &pos, flt const &fovy, flt const &aspect_ratio,
               flt const &znear, flt const &zfar, vec3 const &gaze,
               vec3 const &up)
    : e{pos}, g{glm::normalize(gaze)}, t{glm::normalize(up)}, // Extrinsincs
      fov{fovy}, ar{aspect_ratio},                            // Intrinsincs
      near{znear}, far{zfar}                                  // Intrinsincs
{}

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
    for (std::string curline; std::getline(from, curline);) {
        std::istringstream input(curline);
        std::string        token;
        flt                x, y, z;
        input >> token >> x >> y >> z;
        vec3 v{x, y, z};
        if (token.length() == 0 || token[0] == '#') {
            continue;
        }
        if (token[0] == 'p' || token[0] == 'P') { // Position (eye)
            this->e = v;
        } else if (token[0] == 'l' || token[0] == 'L') { // Look at (gaze)
            this->g = v;
        } else if (token[0] == 'u' || token[0] == 'U') { // Up (top)
            this->t = v;
        }
    }
    from.close();
}

vec3 const &Camera::pos() const { return this->e; }
vec3 const &Camera::gaze() const { return this->g; }
vec3 const &Camera::up() const { return this->t; }
flt const & Camera::fovy() const { return this->fov; }
flt const & Camera::aspect_ratio() const { return this->ar; }
flt const & Camera::znear() const { return this->near; }
flt const & Camera::zfar() const { return this->far; }

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 25 2020, 11:31 [CST]
