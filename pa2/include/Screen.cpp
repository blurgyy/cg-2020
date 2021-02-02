#include "Ray.hpp"
#include "Screen.hpp"

Screen::Screen() {}
Screen::Screen(std::size_t const &width, std::size_t const &height,
               Scene const &world, Camera const &cam)
    : w(width), h(height), sce(world), cam(cam) {
    this->_init();
}

/* Public */

void Screen::attach_scene(Scene const &world) { this->sce = world; }
void Screen::set_cam(Camera const &cam) { this->cam = cam; }

void Screen::render(std::size_t const &spp) {
    flt yscale = std::tan(this->cam.fovy() / 2);
    flt xscale = yscale * this->cam.aspect_ratio();

    this->sce.to_camera_space(this->cam);
    for (std::size_t i = 0; i < this->w; ++i) {
        for (std::size_t j = 0; j < this->h; ++j) {
            flt x = (2 * (i + 0.5) / this->w - 1) * xscale;
            flt y = (2 * (j + 0.5) / this->h - 1) * yscale;
            msg("pixel(%lu, %lu): shoot ray at (%.3f, %.3f, -1.0)\n", i, j, x,
                y);
            Ray          ray(vec3{0}, vec3{x, y, -1});
            Intersection isect = this->sce.intersect(ray);
            if (isect.occurred) {
                this->img(i, j) =
                    Color{.5 + .5 * (isect.normal.x + 1.0) * 255,
                          .5 + .5 * (isect.normal.y + 1.0) * 255,
                          .5 + .5 * (isect.normal.z + 1.0) * 255};
                // output(isect.normal);
            } else {
                this->img(i, j) = Color{0};
            }
        }
    }
}

Image const &Screen::image() const { return this->img; }

/* Private */

void Screen::_init() { this->img.init(this->w, this->h); }

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 01 2021, 17:17 [CST]
