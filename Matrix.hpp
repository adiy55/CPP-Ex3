#ifndef CPP_EX3_MATRIX_HPP
#define CPP_EX3_MATRIX_HPP

#include <iostream>
#include <vector>
#include <string>

namespace zich {

    class Matrix {
        std::vector<double> _matrix;
        int _rows;
        int _cols;

    public:

        Matrix(std::vector<double> matrix, int rows, int cols); // constructor
        // todo: default? destructor? copy constructor?

        Matrix operator-() const;

        Matrix &operator*=(int scalar);

        Matrix operator+(const Matrix &other) const;

        Matrix operator-(const Matrix &other) const;

        // friend functions

        friend std::ostream &operator<<(std::ostream &out, const Matrix &matrix); // todo: does this need to be friend?

        friend Matrix operator*(int scalar, const Matrix &matrix); // todo: make const int?

    };
}
#endif //CPP_EX3_MATRIX_HPP
