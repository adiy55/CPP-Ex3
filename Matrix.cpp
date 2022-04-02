#include "Matrix.hpp"

using namespace zich;


Matrix::Matrix(std::vector<double> matrix, int rows, int cols)
        : _matrix(std::move(matrix)), _rows(rows), _cols(cols) {}

Matrix Matrix::operator-() const {
    return Matrix{{}, 0, 0};
}

Matrix &Matrix::operator*=(const int scalar) {
    return *this;
}

Matrix Matrix::operator+(const Matrix &other) const {
    return Matrix{{}, 0, 0};
}

Matrix Matrix::operator-(const Matrix &other) const {
    return Matrix{{}, 0, 0};
}

// friend functions

std::ostream &zich::operator<<(std::ostream &out, const Matrix &matrix) {
    return out;
}

Matrix zich::operator*(const int scalar, const Matrix &matrix) {
    return Matrix{{}, 0, 0};
}













