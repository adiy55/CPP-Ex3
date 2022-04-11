#include "Matrix.hpp"
#include <regex>

typedef unsigned int uint;

using std::string;
using std::vector;

namespace zich {

    /**
     * Lvalue constructor.
     */
    Matrix::Matrix(const std::vector<double> &matrix, int rows, int cols) // todo: default ctr?
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
        Matrix matrix{*this}; // todo: check which constructor this uses
        matrix.operator*=(-1);
//        std::for_each(matrix._matrix.begin(), matrix._matrix.end(), [](double &val) { if (val != 0) { val = -val; }});
        return matrix;
    }

    /**
     * @param scalar double
     * @return matrix reference with the multiplied entries
     */
    Matrix &Matrix::operator*=(double scalar) {
        std::for_each(_matrix.begin(), _matrix.end(), [scalar](double &val) { val *= scalar; });
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
        res_matrix.operator-=(other); // todo: check
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
    bool Matrix::operator!=(const Matrix &other) const { // todo: test case with 0 and -0
        return !((*this) == other);
    }

// prefix (++i)

    /**
     * @return matrix reference with incremented values
     */
    Matrix &Matrix::operator++() {
        std::for_each(_matrix.begin(), _matrix.end(), [](double &val) { ++val; });
        return *this;
    }

    /**
     * @return matrix reference with decremented entries
     */
    Matrix &Matrix::operator--() {
        std::for_each(_matrix.begin(), _matrix.end(), [](double &val) { --val; });
        return *this;
    }

// postfix (i++)

    /**
     * @return new matrix with old values (actual matrix is incremented)
     */
    Matrix Matrix::operator++(int) {
        Matrix mat_copy{*this};
        ++(*this);
        return mat_copy;
    }

    /**
     * @return new matrix with old values (actual matrix is decremented)
     */
    Matrix Matrix::operator--(int) {
        Matrix mat_copy{*this};
        --(*this);
        return mat_copy;
    }

    /**
     * @param other matrix of the valid dimensions for matrix multiplication (cols = other.rows)
     * @return new matrix with dimensions rows x other.cols and matrix multiplication valuesl
     */
    Matrix Matrix::operator*(const Matrix &other) const {
        Matrix mat_copy{*this};
        mat_copy *= other;
        return mat_copy;
    }

    Matrix &Matrix::operator*=(const Matrix &other) {
        checkDimensionsMul(_cols, other._rows);
        /*
         * 0 1 2
         * 3 4 5
         *
         * 0 1 2
         * 3 4 5
         * 6 7 8
         *
         */
        vector<double> mat_mul; // init new result vector
        uint curr_index = 0; //
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
        _matrix.swap(mat_mul);
        _cols = other._cols;
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
            out << "]";
            if (i < matrix._rows - 1) {
                out << '\n';
            }
        }
        return out;
    }

    void Matrix::cinSplitRows(const string &str_input, vector<string> &input_rows) {
        std::regex extract_rows{R"(\,\s)"};
        // passing -1 as the submatch index parameter performs splitting
        std::sregex_token_iterator iter_start{str_input.begin(), str_input.end(), extract_rows, -1}, iter_end;
        while (iter_start != iter_end) {
            input_rows.push_back(iter_start->str());
            ++iter_start;
        }
    }

    int Matrix::cinColumnsCheck(vector<string> &input_rows) {
        int prev_num_count = -1, num_count = 0;
        for (string &row: input_rows) {
            for (char &c: row) {
                if (c == ' ') {
                    ++num_count;
                }
            }
            if (prev_num_count != -1 && num_count != prev_num_count) {
                throw std::runtime_error("Invalid column dimensions!");
            }
            prev_num_count = num_count;
            num_count = 0;
        }
        return prev_num_count;
    }

    // these websites were useful for testing the regex: https://www.regextester.com/97722, https://regex101.com/
    void Matrix::cinRowsCheck(vector<string> &input_rows) {
        std::regex check_rows{R"(\[(\-?\d+(\.\d+)?)(\s\-?\d+(\.\d+)?)*\])"};

        for (const string &row: input_rows) {
            if (!std::regex_match(row, check_rows)) {
                throw std::runtime_error("Could not parse input!");
            }
        }
    }

    void Matrix::cinInsertNumbers(Matrix &matrix, vector<string> &input_rows) {
        std::regex get_nums{R"(\s)"};
        for (string &row: input_rows) {
            std::sregex_token_iterator iter_start{row.begin() + 1, row.end() - 1, get_nums, -1}, iter_end;
            while (iter_start != iter_end) {
                matrix._matrix.push_back(std::stod(iter_start->str()));
                ++iter_start;
            }
        }
    }

    std::istream &operator>>(std::istream &in, Matrix &matrix) {
        matrix._matrix.clear();
        matrix._rows = 0;
        matrix._cols = 0;

        string str_input;

        in.ignore(); // todo: explain
        getline(in, str_input);

        vector<string> input_rows;

        Matrix::cinSplitRows(str_input, input_rows);
        int col_size = Matrix::cinColumnsCheck(input_rows);
        Matrix::cinRowsCheck(input_rows);
        Matrix::cinInsertNumbers(matrix, input_rows);

        matrix._rows = static_cast<int>(input_rows.size());
        matrix._cols = col_size;

        if (matrix._matrix.empty() || matrix._matrix.size() != matrix._rows * matrix._cols) {
            throw std::runtime_error("Could not parse input!");
        }

        return in;
    }


    /**
     * friend function for scalar on left side
     * @param scalar double
     * @param matrix matrix to be multiplied
     * @return new matrix with multiplied entries
     */
    Matrix operator*(double scalar, const Matrix &matrix) {
        Matrix res_matrix{matrix};
        res_matrix *= scalar;
        return res_matrix;
    }

// class methods and helper functions

    /**
     * Checks that the matrix has valid dimensions.
     * This is called in the constructors.
     * @param mat_size
     * @param rows
     * @param cols
     */
    void Matrix::checkInput(uint mat_size, int rows, int cols) {
        if (rows < 1 || cols < 1 || mat_size != (rows * cols)) {
            throw std::invalid_argument("Invalid matrix size!");
        }
    }

    /**
     * Checks that the dimensions are valid for matrix multiplication.
     * @param mat1_cols
     * @param mat2_rows
     */
    void Matrix::checkDimensionsMul(int mat1_cols, int mat2_rows) {
        if (mat1_cols != mat2_rows) {
            throw std::invalid_argument("Invalid dimensions for matrix multiplication!");
        }
    }

    /**
     * Checks that the dimensions are equal for addition, subtraction, and comparison operators.
     * @param rows1
     * @param cols1
     * @param rows2
     * @param cols2
     */
    void Matrix::checkDimensionsEq(int rows1, int cols1, int rows2, int cols2) {
        if (rows1 != rows2 || cols1 != cols2) {
            throw std::invalid_argument("Invalid dimensions for matrix addition or subtraction!");
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