#include "Matrix.hpp"

typedef unsigned int uint;

using namespace zich;
using std::string;
using std::vector;


Matrix::Matrix(const std::vector<double> &matrix, int rows, int cols)
        : _matrix(matrix), _rows(rows), _cols(cols) { checkInput(_matrix.size(), _rows, _cols); }

Matrix::Matrix(std::vector<double> &&matrix, int rows, int cols) // rvalue reference // move constructor
        : _matrix(std::move(matrix)), _rows(rows), _cols(cols) { checkInput(_matrix.size(), _rows, _cols); }

Matrix Matrix::operator-() const {
    vector<double> res;
    res.reserve(_matrix.size());
    std::for_each(_matrix.begin(), _matrix.end(), [&res](double val) { res.push_back(-val); });
    return Matrix{res, _rows, _cols};
}

Matrix &Matrix::operator*=(const double scalar) {
    std::for_each(_matrix.begin(), _matrix.end(), [scalar](double &val) { val *= scalar; });
    return *this;
}

Matrix Matrix::operator+(const Matrix &other) const {
    checkDimensionsEq(_rows, _cols, other._rows, other._cols);
    vector<double> res;
    res.reserve(_matrix.size());
    for (uint i = 0; i < res.size(); ++i) {
        res[i] = _matrix[i] + other._matrix[i];
    }
    return Matrix{res, _rows, _cols};
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

// friend functions

std::ostream &zich::operator<<(std::ostream &out, const Matrix &matrix) {
    out << matrix.toString();
    return out;
}

Matrix zich::operator*(const double scalar, const Matrix &matrix) {
    vector<double> res;
    res.reserve(matrix._matrix.size());
    for (const double &val: matrix._matrix) {
        res.push_back(scalar * val);
    }
    return Matrix{res, matrix._rows, matrix._cols};
}

std::istream &zich::operator>>(std::istream &in, const Matrix &matrix) {
    return in;
}

// class methods and helper functions

void Matrix::checkInput(uint mat_size, int rows, int cols) {
    if (rows < 0 || cols < 0 || mat_size != (rows * cols)) {
        throw std::invalid_argument("Invalid matrix size!");
    }
}

void Matrix::checkDimensionsMul(int mat1_cols, int mat2_rows) {
    if (mat1_cols != mat2_rows) {
        throw std::invalid_argument("Invalid dimensions for matrix multiplication!");
    }
}

void Matrix::checkDimensionsEq(int rows1, int cols1, int rows2, int cols2) {
    if (rows1 != rows2 || cols1 != cols2) {
        throw std::invalid_argument("Invalid dimensions for matrix addition or subtraction!");
    }
}

std::string Matrix::toString() const {
    string mat_str;
    std::for_each(_matrix.begin(), _matrix.end(),
                  [&mat_str](const double &val) { mat_str += std::to_string(std::float_round_style(val)) + " "; });
    return mat_str;
}

