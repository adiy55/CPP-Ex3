#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include "Matrix.hpp"

typedef unsigned int uint;

using std::string;
using std::vector;

namespace zich {

    /*
     * About constructors:
     * Matrix matrix{*this}; calls copy constructor
     * Matrix matrix{_matrix, _rows, _cols}; calls lvalue constructor
     * Matrix matrix{{...}, (rows), (cols)}; calls move constructor
     */

    /**
     * Lvalue constructor.
     */
    Matrix::Matrix(const std::vector<double> &matrix, int rows, int cols)
            : _matrix(matrix), _rows(rows), _cols(cols) { checkInput(_matrix.size(), _rows, _cols); }

    /**
     * Move constructor for rvalue vectors.
     */
    Matrix::Matrix(std::vector<double> &&matrix, int rows, int cols) // rvalue reference // move constructor
            : _matrix(std::move(matrix)), _rows(rows), _cols(cols) { checkInput(_matrix.size(), _rows, _cols); }

    /**
     * @return new matrix with flipped signs
     */
    Matrix Matrix::operator-() const {
        Matrix matrix{*this};
        matrix.operator*=(-1);
        return matrix;
    }

    /**
     *
     * @param other matrix of the same dimensions
     * @return new matrix with the calculated values
     */
    Matrix Matrix::operator+(const Matrix &other) const {
        Matrix res_matrix{*this};
        res_matrix.operator+=(other);
        return res_matrix;
    }

    /**
     * @param other matrix of the same dimensions
     * @return new matrix with the calculated values
     */
    Matrix Matrix::operator-(const Matrix &other) const {
        Matrix res_matrix{*this};
        res_matrix.operator-=(other);
        return res_matrix;
    }

    /**
     * @return copy of the matrix with same values
     */
    Matrix Matrix::operator+() const {
        return Matrix{*this};
    }

    /**
     * @param other matrix of the same dimensions
     * @return reference of the matrix with the calculated values
     */
    Matrix &Matrix::operator+=(const Matrix &other) {
        checkDimensionsEq(_rows, _cols, other._rows, other._cols);
        for (uint i = 0; i < _matrix.size(); ++i) {
            _matrix[i] += other._matrix[i];
        }
        return *this;
    }

    /**
     * @param other matrix of the same dimensions
     * @return reference of the matrix with the subtracted entries
     */
    Matrix &Matrix::operator-=(const Matrix &other) {
        checkDimensionsEq(_rows, _cols, other._rows, other._cols);
        for (uint i = 0; i < _matrix.size(); ++i) {
            _matrix[i] -= other._matrix[i];
        }
        return *this;
    }

    /**
     * @param other matrix of the same dimensions
     * @return true if the sum of the entries is greater
     */
    bool Matrix::operator>(const Matrix &other) const {
        checkDimensionsEq(_rows, _cols, other._rows, other._cols);
        return (this->calculateSum() > other.calculateSum()); // helper function calculates the sums
    }

    /**
     * @param other matrix of the same dimensions
     * @return true if the sum of entries is greater or equal
     */
    bool Matrix::operator>=(const Matrix &other) const {
        return (*this > other || *this == other);
    }

    /**
     * @param other matrix of the same dimensions
     * @return true if the sum of entries is smaller
     */
    bool Matrix::operator<(const Matrix &other) const {
        checkDimensionsEq(_rows, _cols, other._rows, other._cols);
        return (this->calculateSum() < other.calculateSum());
    }

    /**
     * @param other matrix of the same dimensions
     * @return true if the sum of entries is smaller or equal
     */
    bool Matrix::operator<=(const Matrix &other) const {
        return (*this < other || *this == other);
    }

    /**
     * @param other matrix of the same dimensions
     * @return true if all entries are equal
     */
    bool Matrix::operator==(const Matrix &other) const {
        checkDimensionsEq(_rows, _cols, other._rows, other._cols);
        for (uint i = 0; i < _matrix.size(); ++i) {
            if (_matrix[i] != other._matrix[i]) {
                return false;
            }
        }
        return true;
    }

    /**
     * @param other matrix of the same dimensions
     * @return true if there exists an entry with different values
     */
    bool Matrix::operator!=(const Matrix &other) const {
        return !((*this) == other);
    }

    /**
     * Prefix increment.
     * @return matrix reference with incremented values
     */
    Matrix &Matrix::operator++() {
        std::for_each(_matrix.begin(), _matrix.end(), [](double &val) { ++val; });
        return *this;
    }

    /**
     * Prefix decrement.
     * @return matrix reference with decremented entries
     */
    Matrix &Matrix::operator--() {
        std::for_each(_matrix.begin(), _matrix.end(), [](double &val) { --val; });
        return *this;
    }

    /**
     * Postfix increment.
     * @return new matrix with old values (actual matrix is incremented)
     */
    Matrix Matrix::operator++(int) {
        Matrix mat_copy{*this};
        ++(*this);
        return mat_copy;
    }

    /**
     * Postfix decrement.
     * @return new matrix with old values (actual matrix is decremented)
     */
    Matrix Matrix::operator--(int) {
        Matrix mat_copy{*this};
        --(*this);
        return mat_copy;
    }

    /**
     * @param scalar double
     * @return matrix reference with the multiplied entries
     */
    /*
     * https://www.cprogramming.com/c++11/c++11-lambda-closures.html
     * Explains lambda and specifically captures.
     * In the lambda below, only scalar is captured by making a copy.
     * The lambda in for_each defines what function to apply to each element in _matrix.
     */
    Matrix &Matrix::operator*=(double scalar) {
        std::for_each(_matrix.begin(), _matrix.end(), [&scalar](double &val) { val *= scalar; });
        return *this;
    }

    /**
     * @param scalar double
     * @return new matrix with the multiplied entries
     */
    Matrix Matrix::operator*(double scalar) const {
        Matrix res_mat{*this};
        res_mat.operator*=(scalar);
        return res_mat;
    }

    /**
     * @param other matrix with valid dimensions for matrix multiplication (_cols = other._rows)
     * @return new matrix with dimensions (_rows x other._cols) and matrix multiplication values
     */
    Matrix Matrix::operator*(const Matrix &other) const {
        Matrix mat_copy{*this};
        mat_copy.operator*=(other);
        return mat_copy;
    }

    /*
     * 0 1 2    0 1 2
     * 3 4 5    3 4 5
     *          6 7 8
     */
    /**
     * @param other matrix with valid dimensions for matrix multiplication (_cols = other._rows)
     * @return matrix reference with updated dimensions (_rows x other._cols) and matrix multiplication values
     */
    Matrix &Matrix::operator*=(const Matrix &other) {
        checkDimensionsMul(_cols, other._rows);
        vector<double> mat_mul; // init new result vector
        uint shared_index = 0; // shared_index -> left mat col = right mat row
        uint right_mat_step = static_cast<uint>(other._cols); // number of indices to skip to next row in column
        uint right_mat_col = 0; // current column number of right matrix (for inner loop)
        double curr_sum = 0; // sum of current entry
        for (uint i = 0; i < _matrix.size(); i += static_cast<uint>(_cols)) { // loop left matrix, i->skips to next row
            right_mat_col = 0; // start from first column in right matrix
            for (uint j = 0; j < other._matrix.size(); ++j) { // loop right matrix (multiply the ith row by all columns)
                curr_sum += (_matrix[shared_index + i] * // i = start of current row + shared_index = current column
                             // shared_index * right_mat_step = skip to the row below in the current column (right_mat_col)
                             other._matrix[right_mat_col + (shared_index * right_mat_step)]);
                ++shared_index; // increment to next value in left mat row / right mat column
                if (shared_index == _cols) { // _cols == other._rows (finished current entry)
                    shared_index = 0;
                    mat_mul.push_back(curr_sum); // insert sum of entry
                    curr_sum = 0;
                    ++right_mat_col;
                }
            }
        }
        _matrix.swap(mat_mul); // swaps the contents (addresses) of the vectors, avoids copying (swap is O(1))
        // vector must be of the same data type, size can differ
        _cols = other._cols;
        return *this;
    }

// ******************
// friend functions
// ******************

    /**
     * friend function for scalar on left side
     * @param scalar double
     * @param matrix matrix to be multiplied
     * @return new matrix with multiplied entries
     */
    Matrix operator*(double scalar, const Matrix &matrix) {
        Matrix res_matrix{matrix};
        res_matrix.operator*=(scalar);
        return res_matrix;
    }

    /*
     * https://2019.cppconf-piter.ru/en/2019/spb/talks/45r2fxppvo0iabreclznd/
     * https://www.codesynthesis.com/~boris/blog/2012/07/24/const-rvalue-references/#:~:text=Note%20the%20asymmetry%3A%20while%20a,const%20rvalue%20references%20pretty%20useless.
     */
    /**
     * Print matrix. Each row is in brackets and there is a newline in between rows.
     */
    std::ostream &operator<<(std::ostream &out, const Matrix &matrix) {
        double curr_val = 0;
        for (uint i = 0; i < matrix._rows; ++i) { // loop rows
            out << "[";
            uint start_index = i * static_cast<uint>(matrix._cols); // start index of the ith row
            uint end_of_row_index = start_index + static_cast<uint>(matrix._cols); // end of the ith row index
            for (uint j = start_index; j < end_of_row_index; ++j) { // loop row according to indices calculated above
                // floating point signbit could be negative and print -0 (even though 0 == -0)
                curr_val = matrix._matrix[j] == 0 ? 0 : matrix._matrix[j];
                out << curr_val;
                if (j < end_of_row_index - 1) { // space in between numbers (not including first and last values)
                    out << " ";
                }
            }
            out << "]";
            if (i < (matrix._rows - 1)) { // avoid newline after last row
                out << '\n';
            }
        }
        return out;
    }

    /**
     * Parse user input into a matrix object.
     * Valid format example: [1 0 0], [0 1 0], [0 0 1]
     * @param matrix reference of matrix to parse input into
     */
    std::istream &operator>>(std::istream &in, Matrix &matrix) {
        string str_input;
        getline(in, str_input);

        vector<double> new_mat;
        vector<string> input_rows;

        Matrix::cinSplitRows(str_input, input_rows); // if no exception was thrown: rows are valid
        int col_size = Matrix::cinColumnsCheck(input_rows); // if no exception was thrown: columns are valid
        Matrix::cinInsertNumbers(new_mat, input_rows); // if passed functions above: ready for parsing into matrix
        int row_size = static_cast<int>(input_rows.size());

        // check if parsing was successful
        if (new_mat.empty() || new_mat.size() != row_size * col_size) {
            throw std::runtime_error{"Error: Matrix size does not match dimensions after parsing!"};
        }

        matrix._matrix.swap(new_mat);
        matrix._rows = row_size;
        matrix._cols = col_size;

        return in;
    }

// *******************************************
// private class methods and helper functions
// *******************************************

    /**
        * Split input string by rows and insert into a vector.
        */
    void Matrix::cinSplitRows(const string &str_input, vector<string> &input_rows) {
        std::regex extract_rows{R"(\,\s)"};
        // when sub-match index parameter is -1 -> performs splitting
        std::sregex_token_iterator iter_start{str_input.begin(), str_input.end(), extract_rows, -1};
        std::sregex_token_iterator iter_end;
        while (iter_start != iter_end) {
            input_rows.push_back(iter_start->str());
            ++iter_start;
        }
        Matrix::cinRowsCheck(input_rows); // call helper function
    }

    /**
     * Check that each row is in the valid format.
     */
    void Matrix::cinRowsCheck(vector<string> &input_rows) {
        // these websites were useful for testing the regex: https://www.regextester.com/97722, https://regex101.com/
        std::regex check_rows{R"(\[(\-?\d+(\.\d+)?)(\s\-?\d+(\.\d+)?)*\])"};
        for (const string &row: input_rows) {
            if (!std::regex_match(row, check_rows)) {
                throw std::runtime_error{"Could not parse input!"};
            }
        }
    }

    /**
     * Check that user input rows have the same number of columns.
     * @return number of columns
     */
    int Matrix::cinColumnsCheck(vector<string> &input_rows) {
        int prev_num_count = -1;
        int num_count = 1; // num_count starts from 1 (one value does not have spaces in between)
        for (const string &row: input_rows) {
            for (const char &c: row) {
                if (c == ' ') {
                    ++num_count;
                }
            }
            if (prev_num_count != -1 && num_count != prev_num_count) { // check from second iteration
                throw std::runtime_error{"Invalid column dimensions!"};
            }
            prev_num_count = num_count;
            num_count = 1;
        }
        return prev_num_count; // return number of columns to update the matrix
    }

    /**
     * Split input rows by spaces and append to the matrix vector.
     */
    /*
     * std::regex_token_iterator: https://en.cppreference.com/w/cpp/regex/regex_token_iterator
     * "accesses the individual sub-matches of every match of a regular expression"
     */
    void Matrix::cinInsertNumbers(vector<double> &new_mat, vector<string> &input_rows) {
        std::regex get_nums{R"(\s)"};
        for (string &row: input_rows) {
            // start iterator from +1 and end at -1 to ignore the brackets
            // safe to assume since this helper function is called after the rows are split and input is validated
            // iter_end is the end of sequence (default constructor)
            std::sregex_token_iterator iter_start{row.begin() + 1, row.end() - 1, get_nums, -1};
            std::sregex_token_iterator iter_end;
            while (iter_start != iter_end) {
                new_mat.push_back(std::stod(iter_start->str()));
                ++iter_start;
            }
        }
    }

    /**
     * Checks that the matrix has valid dimensions.
     * This is called in the constructors.
     * @param mat_size vector size
     */
    void Matrix::checkInput(uint mat_size, int rows, int cols) {
        if (rows < 1 || cols < 1 || mat_size != (rows * cols)) {
            throw std::invalid_argument{"Invalid matrix size!"};
        }
    }

    /**
     * Checks that the dimensions are valid for matrix multiplication.
     */
    void Matrix::checkDimensionsMul(int mat1_cols, int mat2_rows) {
        if (mat1_cols != mat2_rows) {
            throw std::invalid_argument{"Invalid dimensions for matrix multiplication!"};
        }
    }

    /**
     * Checks that the dimensions are equal for addition, subtraction, and comparison operators.
     */
    void Matrix::checkDimensionsEq(int rows1, int cols1, int rows2, int cols2) {
        if (rows1 != rows2 || cols1 != cols2) {
            throw std::invalid_argument{"Invalid dimensions for matrix addition or subtraction!"};
        }
    }

    /**
     * Used in comparison functions.
     * @return sum of matrix entries
     */
    double Matrix::calculateSum() const {
        double mat_sum = 0;
        for (const double &val: _matrix) {
            mat_sum += val;
        }
        return mat_sum;
    }

}