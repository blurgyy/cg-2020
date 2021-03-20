#pragma once

#include <array>
#include <random>
#include <string>
#include <vector>

#include <glm/glm.hpp>

// message functions `debugm` (debug messages), `errorm` (error messages)
// Reference: https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html
#ifndef NDEBUG
#define DEBUGGING -1
#else
#define DEBUGGING 0
#endif
#define debugm(fmt, ...)                                                     \
    do {                                                                     \
        if (DEBUGGING) {                                                     \
            fprintf(stdout, " [*] %s::%d::%s(): " fmt, __FILE__, __LINE__,   \
                    __func__, ##__VA_ARGS__);                                \
            fflush(stdout);                                                  \
        }                                                                    \
    } while (0)
#define errorm(fmt, ...)                                                     \
    do {                                                                     \
        if (DEBUGGING)                                                       \
            fprintf(stderr, " [X] %s::%d::%s(): " fmt, __FILE__, __LINE__,   \
                    __func__, ##__VA_ARGS__);                                \
        else                                                                 \
            fprintf(stderr, " [X] " fmt, ##__VA_ARGS__);                     \
        exit(-1);                                                            \
    } while (0)
#define msg(fmt, ...)                                                        \
    do {                                                                     \
        fprintf(stdout, " [v] " fmt, ##__VA_ARGS__);                         \
        fflush(stdout);                                                      \
    } while (0)

// Type definitions
typedef double                             flt;
typedef glm::vec<2, flt, glm::defaultp>    vec2;
typedef glm::vec<3, flt, glm::defaultp>    vec3;
typedef glm::vec<4, flt, glm::defaultp>    vec4;
typedef glm::mat<3, 3, flt, glm::defaultp> mat3;
typedef glm::mat<4, 4, flt, glm::defaultp> mat4;

typedef std::pair<int, int>       pii;
typedef std::pair<size_t, size_t> pss;

// Constants
extern flt const pi;
extern flt const twopi;
extern flt const halfpi;
extern flt const degree;
extern flt const epsilon;

// Structs
struct Color {
    Color();
    Color(unsigned char const &r, unsigned char const &g,
          unsigned char const &b);
    Color(unsigned char const &x);
    Color(vec3 const &values);

    Color correction(flt const &gamma) const;

    Color operator+=(Color const &rhs);

  public:
    // Color values for corresponding channel
    unsigned char r, g, b;
};

// NOTE: Image data array has origin at lower left.
struct Image {
    Image();
    Image(size_t const &width, size_t const &height);

    // Initialize data array
    void         init(size_t const &width, size_t const &height);
    void         fill(Color const &value = Color{0});
    Color &      operator()(size_t const &x, size_t const &y);
    Color const &operator()(size_t const &x, size_t const &y) const;

    // Store color in this array
    std::vector<Color> data;
    // Width and height
    size_t w, h;
};

template <size_t _order> struct Node {
    Node() : tdep{0}, isleaf{false} {};

    // Depth in the pyramid
    int tdep;
    // If this node is a leaf node
    bool isleaf;

    /* Pointer array to children should be declared in inherited structs,
     * because data members are declared in inherited structs, and children
     * have to have same data members as ancestor.
     * */
    // std::array<Node<_order> *, _order> children; // Children
};

// Axis-aligned bounding box (AABB).
struct BBox {
  public:
    vec3 minp, maxp;

  public:
    BBox();
    BBox(vec3 const &p);
    BBox(vec3 const &p1, vec3 const &p2);

    vec3 constexpr centroid() const {
        return this->minp + (this->maxp - this->minp) * 0.5;
    }
    vec3 constexpr extent() const { return this->maxp - this->minp; }
    flt constexpr area() const {
        vec3 e = this->extent();
        return 2 * (e.x * e.x + e.y * e.y + e.z * e.z);
    }
    std::size_t constexpr max_dir() const {
        vec3 e = this->extent();
        return e.x > e.y   ? e.x > e.z ? 0 : 2
               : e.x > e.z ? 1
               : e.y > e.z ? 1
                           : 2;
    }

    // Merged Bbox.
    BBox operator|(BBox const &rhs) const;
    BBox operator|(vec3 const &rhs) const;
    BBox operator|=(BBox const &rhs);
    BBox operator|=(vec3 const &rhs);
};

// Write struct `Image` image data to a ppm file.
// Reference:
//  1. https://rosettacode.org/wiki/Bitmap/Write_a_PPM_file#C.2B.2B
//  2. http://netpbm.sourceforge.net/doc/ppm.html#format
// @param filename: name of the ppm image file
// @param img: image data (of type Image)
void write_ppm(std::string const &filename, Image const &img,
               flt const &gamma = 0.6);

// Returns min(maxx, max(x, minx))
template <typename T, typename T1, typename T2>
T constexpr clamp(T const &x, T1 const &minx, T2 const &maxx) {
    return std::min(static_cast<T>(maxx), std::max(x, static_cast<T>(minx)));
}

// Barycentric interpolation.
template <typename value_type>
value_type berp(std::array<value_type, 3> const &values,
                std::array<flt, 3> const &       b) {
    value_type ret{0};
    for (std::size_t i = 0; i < 3; ++i) {
        ret += b[i] * values[i];
    }
    return ret;
}

// @param `phi` angle between returned vector and `+z` direction.
// @param `theta` angle between returned vector and `+x` direction.
inline vec3 polar_to_cartesian(flt const &phi, flt const &theta) {
    flt x = std::sin(phi) * std::cos(theta);
    flt y = std::sin(phi) * std::sin(theta);
    flt z = std::cos(phi);
    return vec3(x, y, z);
}

// Fast sign function.  Returns `1` when `x` is positive; returns `0` when `x`
// is `0`, returns `-1` when x is negative.
template <typename T> int constexpr sign(T const &x) {
    return ((T)0 < x) - (x < (T)0);
}
template <typename T> T constexpr sq(T const &x) { return x * x; }

inline bool equal(flt const &lhs, flt const &rhs) {
    return fabs(lhs - rhs) < epsilon;
}

// Generate a random number with uniform probability in range [0, 1].
inline flt uniform() {
    thread_local std::random_device     dev;
    thread_local std::mt19937           rngdev{dev()};
    std::uniform_real_distribution<flt> rng{0, 1};
    return rng(rngdev);
}

/*** debugging ***/
inline void output(mat4 const &x) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("%f ", x[i][j]);
        }
        printf("\n");
    }
}
inline void output(mat3 const &x) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            printf("%f ", x[i][j]);
        }
        printf("\n");
    }
}
inline void output(vec4 const &x) {
    for (int i = 0; i < 4; ++i) {
        printf("%f ", x[i]);
    }
    printf("\n");
}
inline void output(vec3 const &x) {
    for (int i = 0; i < 3; ++i) {
        printf("%f ", x[i]);
    }
    printf("\n");
}
inline void output(vec2 const &x) {
    for (int i = 0; i < 2; ++i) {
        printf("%f ", x[i]);
    }
    printf("\n");
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 18 2020, 17:36 [CST]
