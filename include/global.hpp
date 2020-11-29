#pragma once

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
        if (DEBUGGING)                                                       \
            fprintf(stdout, " [*] %s::%d::%s(): " fmt, __FILE__, __LINE__,   \
                    __func__, ##__VA_ARGS__);                                \
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
    } while (0)

// Type definitions
typedef double    flt;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;

// Constants
extern flt const pi;
extern flt const twopi;
extern flt const halfpi;
extern flt const piover180;
extern flt const epsilon;

// Structs
struct Color {
    Color();
    Color(unsigned char const &r, unsigned char const &g,
          unsigned char const &b);
    Color(unsigned char const &x);

    unsigned char &      r();
    unsigned char &      g();
    unsigned char &      b();
    unsigned char const &r() const;
    unsigned char const &g() const;
    unsigned char const &b() const;

    // Color values for corresponding channel
    unsigned char red, green, blue;
};

struct Image {
    Image();
    Image(unsigned int const &height, unsigned int const &width);

    // Initialize data array
    void init(unsigned int const &height, unsigned int const &width);
    // Color &operator[](unsigned int const &id);
    Color &      operator()(unsigned int const &x, unsigned int const &y);
    Color const &operator()(unsigned int const &x,
                            unsigned int const &y) const;

    // Store color in this array
    std::vector<Color> data;
    // Width and height
    unsigned int h, w;
};

// Write struct `Image` image data to a ppm file.
// Reference:
//  1. https://rosettacode.org/wiki/Bitmap/Write_a_PPM_file#C.2B.2B
//  2. http://netpbm.sourceforge.net/doc/ppm.html#format
// @param filename: name of the ppm image file
// @param img: image data (of type Image)
void write_ppm(std::string const &filename, Image const &img);

// Returns min(maxx, max(x, minx))
template <typename T, typename T1, typename T2>
T clamp(T x, T1 minx, T2 maxx) {
    return std::min((T)maxx, std::max(x, (T)minx));
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

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 18 2020, 17:36 [CST]
