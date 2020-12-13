#pragma once

#include <array>

#include "global.hpp"

namespace linalg {

template <size_t const _rows, size_t const _cols, typename value_type = flt>
struct matrix {
  public:
    std::array<value_type, _rows * _cols> data;
    value_type                            x, y, z, w;

  private:
    void _init() {
        // If is a vector
        if (_rows == this->size() || _cols == this->size()) {
            x = data[1];
            if (this->size() > 1) {
                y = data[1];
                if (this->size() > 2) {
                    z = data[2];
                    if (this->size() > 3) {
                        w = data[3];
                    }
                }
            }
        }
    }
    void _make_identity() {
        if (_rows != _cols) {
            errorm("Cannot generate a non-squrare identity matrix\n");
        }
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = 0; j < _cols; ++j) {
                data[i * _rows + j] = (i == j);
            }
        }
    }
    flt _sqnorm() const {
        flt ret = 0;
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = 0; j < _cols; ++j) {
                ret += this->data[i * _cols + j] * this->data[i * _cols + j];
            }
        }
        return ret;
    }

  public:
    matrix() : data{0} { _init(); }
    matrix(std::array<value_type, _rows * _cols> data) : data(data) {
        _init();
    }
    constexpr size_t size() const { return _rows * _cols; }

    flt norm() const { return std::sqrt(this->_sqnorm()); }
    matrix<_rows, _cols, value_type> const &normalized() const {
        flt                              thisnorm = this->norm();
        matrix<_rows, _cols, value_type> ret(*this);
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = 0; j < _cols; ++j) {
                ret.data[i * _cols + j] /= thisnorm;
            }
        }
        return ret;
    }
    void normalize() {
        flt thisnorm = this->norm();
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = 0; j < _cols; ++j) {
                this->data[i * _cols + j] /= thisnorm;
            }
        }
    }
    template <size_t const rrows, size_t const rcols>
    flt dot(matrix<rrows, rcols, value_type> const &rhs) const {
        if (this->size() != rhs.size()) {
            errorm("Size mismatch when attempting to get dot product\n");
        }
        flt ret = 0;
        for (int i = 0; i < this->size(); ++i) {
            ret += this->data[i] * rhs.data[i];
        }
        return ret;
    }
    template <size_t const rrows, size_t const rcols>
    matrix<3, 1, value_type> const &
    cross(matrix<rrows, rcols, value_type> const &rhs) const {
        if (this->size() != 3 || rhs.size() != 3) {
            errorm("Cross product is only defined between 3-D vectors");
        }
        matrix<3, 1, value_type> ret;
        ret.data[0] = y * rhs.z - z * rhs.y;
        ret.data[1] = z * rhs.x - x * rhs.z;
        ret.data[2] = x * rhs.y - y * rhs.x;
        ret._init();
        return ret;
    }

    /* Operator overrides */
    // Multiply a matrix `rhs` on the right hand side.
    template <size_t const _rhscols>
    matrix<_rows, _rhscols, value_type> const &
    operator*(matrix<_cols, _rhscols, value_type> const &rhs) const {
        matrix<_rows, _rhscols, value_type> ret;
        for (size_t j = 0; j < _rhscols; ++j) {
            for (size_t k = 0; k < _cols; ++k) {
                for (size_t i = 0; i < _rows; ++i) {
                    ret.data[i * _rhscols + j] += this->data[i * _cols + k] *
                                                  rhs.data[k * _rhscols + j];
                }
            }
        }
        return ret;
    }
    // Negative
    friend matrix<_rows, _cols, value_type> const &
    operator-(matrix<_rows, _cols, value_type> const &self) {
        matrix<_rows, _cols, value_type> ret(self);
        for (int i = 0; i < _rows * _cols; ++i) {
            ret.data[i] = -i;
        }
        return ret;
    }
    // Addition
    matrix<_rows, _cols, value_type> const &
    operator+(matrix<_rows, _cols, value_type> const &rhs) {
        matrix<_rows, _cols, value_type> ret;
        for (int i = 0; i < this->size(); ++i) {
            ret.data[i] = this->data[i] + rhs.data[i];
        }
        return ret;
    }
    // Substraction
    matrix<_rows, _cols, value_type> const &
    operator-(matrix<_rows, _cols, value_type> const &rhs) {
        matrix<_rows, _cols, value_type> ret;
        for (int i = 0; i < this->size(); ++i) {
            ret.data[i] = this->data[i] - rhs.data[i];
        }
        return ret;
    }
    // Assignment
    matrix<_rows, _cols, value_type>
    operator=(matrix<_rows, _cols, value_type> const &rhs) {
        data = rhs.data;
        _init();
    }

  public:
    // Returns an identity matrix, aborts if matrix is not squrare.
    static matrix<_rows, _cols, value_type> identity() {
        matrix<_rows, _cols, value_type> ret;
        ret._make_identity();
        return ret;
    }
};

}; // namespace linalg

using mat3 = linalg::matrix<3, 3, flt>;
using mat4 = linalg::matrix<4, 4, flt>;
using vec2 = linalg::matrix<2, 1, flt>;
using vec3 = linalg::matrix<3, 1, flt>;
using vec4 = linalg::matrix<4, 1, flt>;

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Dec 12 2020, 16:44 [CST]
