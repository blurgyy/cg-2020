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

typedef std::pair<int, int>       pii;
typedef std::pair<size_t, size_t> pss;

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

// NOTE: Image data array has origin at lower left.
struct Image {
    Image();
    Image(size_t const &height, size_t const &width);

    // Initialize data array
    void         init(size_t const &height, size_t const &width);
    Color &      operator()(size_t const &x, size_t const &y);
    Color const &operator()(size_t const &x, size_t const &y) const;

    // Store color in this array
    std::vector<Color> data;
    // Width and height
    size_t h, w;
};

template <size_t _order> struct Node {
    Node() : fa(nullptr){};

    Node *fa; // Father

    /* Pointer array to children should be declared in inherited structs,
     * because data members are declared in inherited structs, and children
     * have to have same data members as ancestor.
     * */
    // std::array<Node<_order> *, _order> children; // Children
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
T constexpr clamp(T x, T1 minx, T2 maxx) {
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
