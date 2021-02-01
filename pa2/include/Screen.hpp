#pragma once

#include "Camera.hpp"
#include "Scene.hpp"
#include "global.hpp"

class Screen {
  private:
    std::size_t w, h;

    Scene  sce;
    Camera cam;

    Image img;

  private:
    void _init();

  public:
    Screen();
    Screen(std::size_t const &width, std::size_t const &height,
           Scene const &world = Scene{}, Camera const &cam = Camera{});

    // Attach screen to a scene.
    void attach(Scene const &world);
    // Set up camera
    void set_cam(Camera const &cam);

    void render(std::size_t const &spp = 16);
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 01 2021, 17:17 [CST]
