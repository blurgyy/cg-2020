#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <string>

#include <opencv2/opencv.hpp>

// message functions `debugm` (debug messages), `errorm` (error messages)
// Reference: https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html
#ifndef NDEBUG
#define DEBUGGING 0
#else
#define DEBUGGING -1
#endif
#define debugm(fmt, ...)                                                     \
    do {                                                                     \
        if (~DEBUGGING)                                                      \
            fprintf(stdout, " [*] %s::%d::%s(): " fmt, __FILE__, __LINE__,   \
                    __func__, ##__VA_ARGS__);                                \
    } while (0)
#define errorm(fmt, ...)                                                     \
    do {                                                                     \
        if (~DEBUGGING)                                                      \
            fprintf(stderr, " [X] %s::%d::%s(): " fmt, __FILE__, __LINE__,   \
                    __func__, ##__VA_ARGS__);                                \
        else                                                                 \
            fprintf(stderr, " [X] " fmt, ##__VA_ARGS__);                     \
        exit(-1);                                                            \
    } while (0)

// Type definitions
typedef double flt;

// Constants
extern flt const pi;
extern flt const twopi;
extern flt const halfpi;
extern flt const piover180;

struct Color {
    Color();
    Color(unsigned char const &r, unsigned char const &g,
          unsigned char const &b);
    Color(unsigned char const &x);

    unsigned char &r();
    unsigned char &g();
    unsigned char &b();
    unsigned char const &r() const;
    unsigned char const &g() const;
    unsigned char const &b() const;

    // Color values for corresponding channel
    unsigned char red, green, blue;
};

struct Image {
    Image();
    Image(unsigned int const &width, unsigned int const &height);

    // Initialize data array
    void init(unsigned int const &width, unsigned int const &height);
    // Color &operator[](unsigned int const &id);
    Color &operator()(unsigned int const &i, unsigned int const &j);

    // Store color in this array
    std::vector<Color> data;
    // Width and height
    unsigned int w, h;
};

// Write cv::Mat image data to a ppm file.
// Reference:
//  1. https://rosettacode.org/wiki/Bitmap/Write_a_PPM_file#C.2B.2B
//  2. http://netpbm.sourceforge.net/doc/ppm.html#format
// @param filename: name of the ppm image file
// @param data: image data (of type CV_8UC3)
void write_ppm(std::string const &filename, cv::Mat const &data);

// Write struct `Image` image data to a ppm file.
// Reference:
//  1. https://rosettacode.org/wiki/Bitmap/Write_a_PPM_file#C.2B.2B
//  2. http://netpbm.sourceforge.net/doc/ppm.html#format
// @param filename: name of the ppm image file
// @param img: image data (of type Image)
void write_ppm(std::string const &filename, Image const &img);

#endif

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Nov 18 2020, 17:36 [CST]
