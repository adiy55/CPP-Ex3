#include "Matrix.hpp"

namespace zich {

    Matrix::Matrix(const std::vector<double> &matrix, int rows, int cols)
            : _matrix(matrix), _rows(rows), _cols(cols) {}

    Matrix::Matrix(std::vector<double> &&matrix, int rows, int cols) // rvalue reference
            : _matrix(std::move(matrix)), _rows(rows), _cols(cols) {}

    Matrix Matrix::operator-() const {
        return Matrix{{}, 0, 0};
    }

    Matrix &Matrix::operator*=(const double scalar) {
        return *this;
    }

    Matrix Matrix::operator+(const Matrix &other) const {
        return Matrix{{}, 0, 0};
    }

    Matrix Matrix::operator-(const Matrix &other) const {
        return Matrix{{}, 0, 0};
    }

    Matrix Matrix::operator+() const {
        return Matrix{{}, 0, 0};
    }

    Matrix &Matrix::operator+=(const Matrix &other) {
        return *this;
    }

    Matrix &Matrix::operator-=(const Matrix &other) {
        return *this;
    }

    bool Matrix::operator>(const Matrix &other) const {
        return false;
    }

    bool Matrix::operator>=(const Matrix &other) const {
        return false;
    }

    bool Matrix::operator<(const Matrix &other) const {
        return false;
    }

    bool Matrix::operator<=(const Matrix &other) const {
        return false;
    }

    bool Matrix::operator==(const Matrix &other) const {
        return false;
    }

    bool Matrix::operator!=(const Matrix &other) const {
        return false;
    }

// prefix (++i)

    Matrix &Matrix::operator++() {
        return *this;
    }

    Matrix &Matrix::operator--() {
        return *this;
    }

// postfix (i++)

    Matrix Matrix::operator++(int) {
        return Matrix{{}, 0, 0};
    }

    Matrix Matrix::operator--(int) {
        return Matrix{{}, 0, 0};
    }

    Matrix Matrix::operator*(const Matrix &other) const {
        return Matrix{{}, 0, 0};
    }

    Matrix &Matrix::operator*=(const Matrix &other) {
        return *this;
    }

// friend functions

    std::ostream &operator<<(std::ostream &out, const Matrix &matrix) {
        return out;
    }

    std::istream &operator>>(std::istream &in, const Matrix &matrix) {
        return in;
    }

    Matrix operator*(const double scalar, const Matrix &matrix) {
        return Matrix{{}, 0, 0};
    }

}