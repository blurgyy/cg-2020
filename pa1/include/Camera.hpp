#pragma once

#include "global.hpp"

class Camera {
  private:
    // Extrinsincs
    vec3 e; // Position
    vec3 g; // Direction (gaze)
    vec3 t; // Direction (up)

    // Intrinsincs
    flt fov;       // Field of view (Vertical, in degrees)
    flt ar;        // \frac{height}{width}
    flt near, far; // Near and far clipping plane's z coordinates

    // Multiply this matrix with world-space object to get its corresponding
    // camera-space coordinate.
    mat4 view;

  private:
    void _init_view_matrix();

  public:
    Camera();
    Camera(flt const &fovy, flt const &aspect_ratio);
    Camera(vec3 const &pos, flt const &fovy, flt const &aspect_ratio,
           flt const &znear, flt const &zfar,
           vec3 const &gaze = vec3{0, 0, -1}, vec3 const &up = vec3{0, 1, 0});

    void init(vec3 const &pos, flt const &fovy, flt const &aspect_ratio,
              flt const &znear, flt const &zfar,
              vec3 const &gaze = vec3{0, 0, -1},
              vec3 const &up   = vec3{0, 1, 0});

    void load(std::string const &configfile);

    // Get Position
    vec3 const &pos() const;
    // Get Direction (gaze)
    vec3 const &gaze() const;
    // Get Direction (up)
    vec3 const &up() const;
    // Get Field of view (Vertical, in degrees)
    flt const &fovy() const;
    // Get \frac{height}{width}
    flt const &aspect_ratio() const;
    // Get Near clipping plane's z coordinate
    flt const &znear() const;
    // Get Far clipping plane's z coordinate
    flt const &zfar() const;
    // Get view matrix
    mat4 const &view_matrix() const;
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 25 2020, 11:27 [CST]
