#include "Screen.hpp"

Screen::Screen() {}
Screen::Screen(std::size_t const &width, std::size_t const &height,
               Scene const &world, Camera const &cam)
    : w(width), h(height), sce(world), cam(cam) {
    this->_init();
}

/* Public */

void Screen::attach(Scene const &world) { this->sce = world; }
void Screen::set_cam(Camera const &cam) { this->cam = cam; }

void Screen::render(std::size_t const &spp) {
    this->sce.to_camera_space(this->cam);
    for (std::size_t i = 0; i < this->w; ++i) {
        for (std::size_t j = 0; j < this->h; ++j) {
            flt x = i + 0.5;
            flt y = j + 0.5;
        }
    }
}

/* Private */

void Screen::_init() { this->img.init(this->w, this->h); }

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 01 2021, 17:17 [CST]
