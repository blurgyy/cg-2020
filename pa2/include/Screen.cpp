#include "Ray.hpp"
#include "Screen.hpp"

#include <omp.h>

Screen::Screen() {}
Screen::Screen(std::size_t const &width, std::size_t const &height,
               Scene const &world, Camera const &cam)
    : w(width), h(height), sce(world), cam(cam) {
    this->_init();
}

/* Public */

void Screen::attach_scene(Scene const &world) { this->sce = world; }
void Screen::set_cam(Camera const &cam) { this->cam = cam; }

void Screen::render(std::size_t const &spp, flt const &rr) {
    this->sce.to_camera_space(this->cam);
    if (!this->sce.has_skybox() && this->sce.emissives().size() == 0) {
        // There is no light source or skybox in the scene whatsoever, abort
        // meaningless rendering.
        msg("Scene has no light source or skybox, returning dark image.\n");
        return;
    }

    flt yscale = std::tan(this->cam.fovy() / 2 * degree);
    flt xscale = yscale * this->cam.aspect_ratio();

    flt pixel_w = 1.0 / this->w * xscale;
    flt pixel_h = 1.0 / this->h * yscale;

    this->sce.build_BVH();
    for (std::size_t i = 0; i < this->w; ++i) {
#pragma omp parallel for
        for (std::size_t j = 0; j < this->h; ++j) {
            flt x = (2 * (i + 0.5) / this->w - 1) * xscale;
            flt y = (2 * (j + 0.5) / this->h - 1) * yscale;
            // debugm("pixel(%lu, %lu): shoot ray at (%.3f, %.3f, -1.0)\n", i,
            // j, x, y);
            vec3 colorvec{0};
            for (std::size_t s = 0; s < spp; ++s) {
                flt nx = x + (uniform() - 1) * pixel_w;
                flt ny = y + (uniform() - 1) * pixel_h;
                Ray ray{vec3{0}, vec3{nx, ny, -1}};
                colorvec += this->sce.shoot(ray, rr);
            }
            this->img(i, j) = Color{colorvec / static_cast<flt>(spp)};
        }
        msg("Progress: [%zu/%zu]\r", i + 1, this->w);
    }
    msg("\n");
}

Image const &Screen::image() const { return this->img; }

/* Private */

void Screen::_init() { this->img.init(this->w, this->h); }

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 01 2021, 17:17 [CST]
