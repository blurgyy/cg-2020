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

void Screen::render(std::size_t const &spp) {}

/* Private */

void Screen::_init() { this->img.init(this->w, this->h); }

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 01 2021, 17:17 [CST]
