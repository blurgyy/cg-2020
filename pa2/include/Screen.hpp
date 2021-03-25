#pragma once

#include "Camera.hpp"
#include "Scene.hpp"
#include "global.hpp"

class Screen {
  private:
    std::size_t w, h;
    std::size_t iter;

    Scene  sce;
    Camera cam;

    Image_t<vec3> img;

  private:
    void _init();

  public:
    Screen();
    Screen(std::size_t const &width, std::size_t const &height,
           Scene const &world = Scene{}, Camera const &cam = Camera{});

    // Attach screen to a scene.
    void attach_scene(Scene const &world);
    // Set up camera
    void set_cam(Camera const &cam);

    void render(flt const &rr, std::string const &outputfile);

    Image image() const;
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 01 2021, 17:17 [CST]
