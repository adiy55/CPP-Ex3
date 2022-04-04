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
    Matrix matrix{_matrix, _rows, _cols};
    std::for_each(matrix._matrix.begin(), matrix._matrix.end(), [](double &val) { if (val != 0) val = -val; });
    return matrix;
}

Matrix &Matrix::operator*=(const double scalar) {
    std::for_each(_matrix.begin(), _matrix.end(), [scalar](double &val) { val *= scalar; });
    return *this;
}

Matrix Matrix::operator+(const Matrix &other) const {
    checkDimensionsEq(_rows, _cols, other._rows, other._cols);
    Matrix res_matrix{_matrix, _rows, _cols};
    for (uint i = 0; i < _matrix.size(); ++i) {
        res_matrix._matrix[i] += other._matrix[i];
    }
    return res_matrix;
}

Matrix Matrix::operator-(const Matrix &other) const {
    checkDimensionsEq(_rows, _cols, other._rows, other._cols);
    Matrix res_matrix{_matrix, _rows, _cols};
    for (uint i = 0; i < res_matrix._matrix.size(); ++i) {
        res_matrix._matrix[i] -= other._matrix[i];
    }
    return res_matrix;
}

Matrix Matrix::operator+() const {
    return Matrix{{}, 0, 0};
}

Matrix &Matrix::operator+=(const Matrix &other) {
    checkDimensionsEq(_rows, _cols, other._rows, other._cols);
    for (uint i = 0; i < _matrix.size(); ++i) {
        _matrix[i] += other._matrix[i];
    }
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
    uint curr_row;
    uint curr_col;
    for (uint i = 0; i < matrix._rows; ++i) {
        out << "[";
        curr_row = i * static_cast<uint>(matrix._rows);
        curr_col = curr_row + static_cast<uint>(matrix._cols);
        for (uint j = curr_row; j < curr_col; ++j) {
            out << matrix._matrix[j];
            if (j < curr_col - 1) {
                out << " ";
            }
        }
        out << "]\n";
    }
    return out;
}

Matrix zich::operator*(const double scalar, const Matrix &matrix) {
    Matrix res_matrix{matrix._matrix, matrix._rows, matrix._cols};
    std::for_each(res_matrix._matrix.begin(), res_matrix._matrix.end(), [scalar](double &val) { val *= scalar; });
    return res_matrix;
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

