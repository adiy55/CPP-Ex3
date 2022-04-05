#ifndef CPP_EX3_MATRIX_HPP
#define CPP_EX3_MATRIX_HPP

#include <iostream>
#include <vector>
#include <string>

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

    public:

        // https://www.reddit.com/r/cpp_questions/comments/swaxw2/passing_a_vector_to_constructor/
        // https://stackoverflow.com/questions/46513507/c-copy-constructor-vs-move-constructor-for-stdvector
        Matrix(const std::vector<double> &matrix, int rows, int cols); // constructor

        Matrix(std::vector<double> &&matrix, int rows, int cols); // rvalue constructor

        Matrix operator-() const;

        // https://clang.llvm.org/extra/clang-tidy/checks/readability-avoid-const-params-in-decls.html
        // https://abseil.io/tips/109
        Matrix &operator*=(double scalar);

        Matrix operator+(const Matrix &other) const;

        Matrix operator-(const Matrix &other) const;

        Matrix operator+() const; // todo: check

        Matrix &operator+=(const Matrix &other);

        Matrix &operator-=(const Matrix &other);

        bool operator>(const Matrix &other) const;

        bool operator>=(const Matrix &other) const;

        bool operator<(const Matrix &other) const;

        bool operator<=(const Matrix &other) const;

        bool operator==(const Matrix &other) const;

        bool operator!=(const Matrix &other) const;

        // todo: = operator

        // prefix (++i)

        Matrix &operator++();

        Matrix &operator--();

        // postfix (i++)

        Matrix operator++(int);

        Matrix operator--(int);


        Matrix operator*(const Matrix &other) const;

        Matrix &operator*=(const Matrix &other);

        // friend functions

        friend std::ostream &operator<<(std::ostream &out, const Matrix &matrix);

        friend std::istream &operator>>(std::istream &in, const Matrix &matrix);

        friend Matrix operator*(double scalar, const Matrix &matrix);

//        friend Matrix operator*(const Matrix &matrix, double scalar); // todo???

    };
}
#endif //CPP_EX3_MATRIX_HPP
