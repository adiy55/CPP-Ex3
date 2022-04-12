#ifndef CPP_EX3_MATRIX_HPP
#define CPP_EX3_MATRIX_HPP

#include <iostream>
#include <vector>
#include <string>

/*
 * Why the {}-initializer (list initialization) syntax is preferred:
 * https://tinyurl.com/5f4rw4xb
 * https://isocpp.org/blog/2016/05/quick-q-why-is-list-initialization-using-curly-braces-better-than-the-alter
 * Most vexing parse: (another reason curly brackets should be used)
 * https://www.fluentcpp.com/2018/01/30/most-vexing-parse/
 */
namespace zich {

    class Matrix {
    private:
        std::vector<double> _matrix;
        int _rows;
        int _cols;

        static void checkInput(unsigned int mat_size, int rows, int cols);

        static void checkDimensionsMul(int mat1_cols, int mat2_rows);

        static void checkDimensionsEq(int rows1, int cols1, int rows2, int cols2);

        double calculateSum() const;

        static void cinSplitRows(const std::string &str_input, std::vector<std::string> &input_rows);

        static void cinRowsCheck(std::vector<std::string> &input_rows);

        static int cinColumnsCheck(std::vector<std::string> &input_rows);

        static void cinInsertNumbers(std::vector<double> &new_mat, std::vector<std::string> &input_rows);

    public:

        // https://www.reddit.com/r/cpp_questions/comments/swaxw2/passing_a_vector_to_constructor/
        // https://stackoverflow.com/questions/46513507/c-copy-constructor-vs-move-constructor-for-stdvector
        Matrix(const std::vector<double> &matrix, int rows, int cols); // constructor

        Matrix(std::vector<double> &&matrix, int rows, int cols); // rvalue constructor

        Matrix operator-() const;

        Matrix operator+(const Matrix &other) const;

        Matrix operator-(const Matrix &other) const;

        Matrix operator+() const;

        Matrix &operator+=(const Matrix &other);

        Matrix &operator-=(const Matrix &other);

        bool operator>(const Matrix &other) const;

        bool operator>=(const Matrix &other) const;

        bool operator<(const Matrix &other) const;

        bool operator<=(const Matrix &other) const;

        bool operator==(const Matrix &other) const;

        bool operator!=(const Matrix &other) const;

        // prefix (++i)

        Matrix &operator++();

        Matrix &operator--();

        // postfix (i++)

        Matrix operator++(int);

        Matrix operator--(int);


        // https://clang.llvm.org/extra/clang-tidy/checks/readability-avoid-const-params-in-decls.html
        // https://abseil.io/tips/109
        Matrix &operator*=(double scalar);

        Matrix operator*(double scalar) const;

        Matrix operator*(const Matrix &other) const;

        Matrix &operator*=(const Matrix &other);

        // friend functions

        friend Matrix operator*(double scalar, const Matrix &matrix);

        friend std::ostream &operator<<(std::ostream &out, const Matrix &matrix);

        friend std::istream &operator>>(std::istream &in, Matrix &matrix);

    };
}
#endif //CPP_EX3_MATRIX_HPP
