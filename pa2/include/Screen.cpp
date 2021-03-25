#include "Ray.hpp"
#include "Screen.hpp"

#include <atomic>

#include <omp.h>

Screen::Screen() : iter(0), gamma(1) {}
Screen::Screen(std::size_t const &width, std::size_t const &height,
               Scene const &world, Camera const &cam)
    : w(width), h(height), iter(0), gamma(1), sce(world), cam(cam) {
    this->_init();
}

/* Public */

void Screen::attach_scene(Scene const &world) { this->sce = world; }
void Screen::set_cam(Camera const &cam) { this->cam = cam; }
void Screen::set_gamma(flt const &gamma) { this->gamma = gamma; }

void Screen::render(flt const &rr, std::string const &outputfile) {
    this->sce.to_camera_space(this->cam);

    flt yscale = std::tan(this->cam.fovy() / 2 * degree);
    flt xscale = yscale * this->cam.aspect_ratio();

    flt pixel_w = 1.0 / this->w * xscale;
    flt pixel_h = 1.0 / this->h * yscale;

    this->sce.build_BVH();
    this->iter = 0;
    while (true) {
        std::atomic<std::size_t> progress{0};
#pragma omp parallel for schedule(dynamic)
        for (std::size_t i = 0; i < this->w; ++i) {
            for (std::size_t j = 0; j < this->h; ++j) {
                flt x = (2 * (i + 0.5) / this->w - 1) * xscale;
                flt y = (2 * (j + 0.5) / this->h - 1) * yscale;
                // debugm("pixel(%lu, %lu): shoot ray at (%.3f, %.3f,
                // -1.0)\n", i, j, x, y);
                vec3 colorvec{0};
                flt  nx = x + (uniform() - 0.5) * pixel_w;
                flt  ny = y + (uniform() - 0.5) * pixel_h;
                Ray  ray{vec3{0}, vec3{nx, ny, -1}};
                vec3 color = this->sce.shoot(ray, rr);
                color      = clamp(color); // Is this needed?
                this->img(i, j) += color;
            }
            msg("Iteration#%d - Progress: [%zu/%zu]\r", this->iter + 1,
                ++progress, this->w);
        }
        ++this->iter;
        write_ppm(outputfile, this->image(), this->gamma);
    }
}

Image Screen::image() const {
    Image ret(this->w, this->h);
    for (int i = 0; i < this->w; ++i) {
        for (int j = 0; j < this->h; ++j) {
            ret(i, j) = Color(this->img(i, j) / static_cast<flt>(this->iter));
        }
    }
    return ret;
}

/* Private */

void Screen::_init() { this->img.init(this->w, this->h); }

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Feb 01 2021, 17:17 [CST]
