#include "Matrix.hpp"

typedef unsigned int uint;

using std::string;
using std::vector;

namespace zich {

    Matrix::Matrix(const std::vector<double> &matrix, int rows, int cols)
            : _matrix(matrix), _rows(rows), _cols(cols) { checkInput(_matrix.size(), _rows, _cols); }

    Matrix::Matrix(std::vector<double> &&matrix, int rows, int cols) // rvalue reference // move constructor
            : _matrix(std::move(matrix)), _rows(rows), _cols(cols) { checkInput(_matrix.size(), _rows, _cols); }

    Matrix Matrix::operator-() const {
        Matrix matrix{*this};
        std::for_each(matrix._matrix.begin(), matrix._matrix.end(), [](double &val) { if (val != 0) { val = -val; }});
        return matrix;
    }

    Matrix &Matrix::operator*=(double scalar) {
        std::for_each(_matrix.begin(), _matrix.end(), [scalar](double &val) { val *= scalar; });
        return *this;
    }

    Matrix Matrix::operator+(const Matrix &other) const {
        Matrix res_matrix{*this};
        res_matrix.operator+=(other);
        return res_matrix;
    }

    Matrix Matrix::operator-(const Matrix &other) const {
        Matrix res_matrix{*this};
        res_matrix.operator-=(other); // todo: check
        return res_matrix;
    }

    Matrix Matrix::operator+() const {
        return Matrix{*this};
    }

    Matrix &Matrix::operator+=(const Matrix &other) {
        checkDimensionsEq(_rows, _cols, other._rows, other._cols);
        for (uint i = 0; i < _matrix.size(); ++i) {
            _matrix[i] += other._matrix[i];
        }
        return *this;
    }

    Matrix &Matrix::operator-=(const Matrix &other) {
        checkDimensionsEq(_rows, _cols, other._rows, other._cols);
        for (uint i = 0; i < _matrix.size(); ++i) {
            _matrix[i] -= other._matrix[i];
        }
        return *this;
    }

    bool Matrix::operator>(const Matrix &other) const {
        checkDimensionsEq(_rows, _cols, other._rows, other._cols);
        return (this->calculateSum() > other.calculateSum());
    }

    bool Matrix::operator>=(const Matrix &other) const {
        return (*this > other || *this == other);
    }

    bool Matrix::operator<(const Matrix &other) const {
        checkDimensionsEq(_rows, _cols, other._rows, other._cols);
        return (this->calculateSum() < other.calculateSum());
    }

    bool Matrix::operator<=(const Matrix &other) const {
        return (*this < other || *this == other);
    }

    bool Matrix::operator==(const Matrix &other) const {
        checkDimensionsEq(_rows, _cols, other._rows, other._cols);
        for (uint i = 0; i < _matrix.size(); ++i) {
            if (_matrix[i] != other._matrix[i]) {
                return false;
            }
        }
        return true;
    }

    bool Matrix::operator!=(const Matrix &other) const {
        return !((*this) == other);
    }

// prefix (++i)

    Matrix &Matrix::operator++() {
        std::for_each(_matrix.begin(), _matrix.end(), [](double &val) { val++; });
        return *this;
    }

    Matrix &Matrix::operator--() {
        std::for_each(_matrix.begin(), _matrix.end(), [](double &val) { val--; });
        return *this;
    }

// postfix (i++)

    Matrix Matrix::operator++(int) {
        Matrix mat_copy{*this};
        ++(*this);
        return mat_copy;
    }

    Matrix Matrix::operator--(int) {
        Matrix mat_copy{*this};
        --(*this);
        return mat_copy;
    }

    Matrix Matrix::operator*(const Matrix &other) const {
        checkDimensionsMul(_cols, other._rows);
        /*
         * 0 1 2
         * 3 4 5
         *
         * 0 1 2
         * 3 4 5
         * 6 7 8
         *
         *
         * 2x3 3x3
         *
         * cols left
         *      cols -skip right += row
         *
         */
        vector<double> mat_mul;
        uint curr_index = 0;
        uint step = static_cast<uint>(other._cols);
        uint curr_col = 0;
        double curr_sum = 0;
        for (uint i = 0; i < _matrix.size(); i += static_cast<uint>(_cols)) {
            curr_col = 0;
            for (uint j = 0; j < other._matrix.size(); ++j) {
                curr_sum += (_matrix[curr_index + i] * other._matrix[curr_col + (curr_index * step)]);
                ++curr_index;
                if (curr_index == _cols) {
                    curr_index = 0;
                    mat_mul.push_back(curr_sum);
                    curr_sum = 0;
                    ++curr_col;
                }
            }
        }
        return Matrix{mat_mul, _rows, other._cols};
    }

    Matrix &Matrix::operator*=(const Matrix &other) {
        checkDimensionsMul(_cols, other._rows);
        return *this;
    }

// friend functions

    /*
     * https://2019.cppconf-piter.ru/en/2019/spb/talks/45r2fxppvo0iabreclznd/
     * https://www.codesynthesis.com/~boris/blog/2012/07/24/const-rvalue-references/#:~:text=Note%20the%20asymmetry%3A%20while%20a,const%20rvalue%20references%20pretty%20useless.
     */
    std::ostream &operator<<(std::ostream &out, const Matrix &matrix) {
        double tmp = 0;
        for (uint i = 0; i < matrix._rows; ++i) {
            out << "[";
            uint start_index = i * static_cast<uint>(matrix._cols);
            uint end_of_row_index = start_index + static_cast<uint>(matrix._cols);
            for (uint j = start_index; j < end_of_row_index; ++j) {
                tmp = matrix._matrix[j] == 0 ? 0 : matrix._matrix[j];
                out << tmp;
                if (j < end_of_row_index - 1) {
                    out << " ";
                }
            }
            out << "]\n";
        }
        return out;
    }

    std::istream &operator>>(std::istream &in, const Matrix &matrix) {
        return in;
    }

    Matrix operator*(double scalar, const Matrix &matrix) {
        Matrix res_matrix{matrix};
        res_matrix *= scalar;
        return res_matrix;
    }

// class methods and helper functions

    void Matrix::checkInput(uint mat_size, int rows, int cols) {
        if (rows < 1 || cols < 1 || mat_size != (rows * cols)) {
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

    double Matrix::calculateSum() const {
        double mat_sum = 0;
        for (const double &val: _matrix) {
            mat_sum += val;
        }
        return mat_sum;
    }

}