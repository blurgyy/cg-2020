#pragma once

#include "global.hpp"

namespace linalg {

template <size_t const _rows, size_t const _cols, typename value_type = flt>
class matrix {
  private:
    value_type data[_rows * _cols];

  private:
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
    matrix() : data{0} {}

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

  public:
    // Returns an identity matrix, aborts if matrix is not squrare.
    static matrix<_rows, _cols, value_type> identity() {
        matrix<_rows, _cols, value_type> ret;
        ret._make_identity();
        return ret;
    }
};

}; // namespace linalg

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Dec 12 2020, 16:44 [CST]
