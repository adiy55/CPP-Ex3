#include <sstream>
#include "doctest.h"
#include "sources/Matrix.hpp"

typedef unsigned int uint;

using namespace zich;

const std::vector<double> identity{1, 0, 0, 0, 1, 0, 0, 0, 1}; // global because this is used frequently

/**
 * Helper function for tests.
 * @return zero matrix of the given size
 */
Matrix generateZeroMatrix(int rows, int cols) {
    std::vector<double> matrix(static_cast<uint>(rows * cols), 0);
    return Matrix{matrix, rows, cols};
}

TEST_CASE ("Bad Input- initializing matrix with negative dimensions") {
            CHECK_THROWS((Matrix{{}, 0, 0}););
            CHECK_THROWS((Matrix{{0, 1}, -2, -1}););
            CHECK_NOTHROW((Matrix{identity, 3, 3}));
            CHECK_THROWS((Matrix{identity, -3, 3}));
            CHECK_THROWS((Matrix{identity, 3, -3}));
            CHECK_THROWS((Matrix{identity, -3, -3})); // rows*cols is positive here
}

TEST_CASE ("Bad Input- initializing matrix with dimensions that do not match vector size") {
            CHECK_THROWS((Matrix{identity, 3, 1})); // identity vector contains 9 values
            CHECK_THROWS((Matrix{identity, 0, 0})); // zero is invalid
            CHECK_THROWS((Matrix{{-0.0}, 0, 1})); // zero is invalid
            CHECK_THROWS((Matrix{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 2, 10}));
            CHECK_THROWS((Matrix{{11, 22, 33, 44, 55}, 4, 1}));
}

TEST_CASE ("Matrix Multiplication") {
    Matrix mat1{identity, 3, 3};
    Matrix mat2{generateZeroMatrix(20, 3)};
    Matrix mat3{{45.75, 242.333, -67.43, 656, 3}, 5, 1};
    Matrix mat4{{454.24, -205, 5, -35, 22}, 1, 5};
    Matrix mat5{{16.4, 0, 0, 0, 16.4, 0, 0, 0, 16.4}, 3, 3};
    Matrix mat6{{302.931, -194.562, 194.668, -292.798, 372.674, 59.7573, -149.028, 82.8862, -302.947}, 9, 1};
    Matrix mat7{generateZeroMatrix(1, 9)};
    Matrix mat8{generateZeroMatrix(9, 9)};

            SUBCASE("Bad Input- wrong dimensions") {
        Matrix mat9{{5}, 1, 1};
                CHECK_THROWS(mat1 * mat2);
                CHECK_THROWS(mat4 * mat9);
                CHECK_THROWS(mat1 * mat6); // vector size is the same, different dimensions
    }

            SUBCASE("Good Input- valid dimensions") {
        Matrix matrix9{{0.0}, 1, 1};
        Matrix matrix10{{-0.0}, 1, 1};
        Matrix res{matrix9 * matrix10};
                CHECK(bool ((res == matrix9) && (res == matrix10))); // 0.0 == -0.0
                CHECK_NOTHROW(mat2 * mat1);
                CHECK_NOTHROW(mat3 * mat4);
                CHECK((mat1 * mat5) == (16.4 * mat1));
                CHECK((mat6 * mat7) == mat8);

    }

            SUBCASE("*= matrix operator") {
                CHECK_THROWS(mat1 *= mat2);
                CHECK_NOTHROW(mat4 *= mat3);
    }

}

/*
 * trying all three initializations
 * preferred is curly brackets (more info in header)
 */
TEST_CASE ("Deep Copy- check if matrices are equal and have different memory addresses") {
    Matrix mat1{{454.24, -205, 5, -35, 22, -0}, 2, 3};
    Matrix mat2{mat1};
    Matrix mat3 = mat1;
    Matrix mat4(mat1);

            CHECK(bool ((mat1 == mat2) && (&mat1 != &mat2)));
            CHECK(bool ((mat1 == mat3) && (&mat1 != &mat3)));
            CHECK(bool ((mat1 == mat4) && (&mat1 != &mat4)));

}

TEST_CASE ("Comparison Operators") {
    Matrix mat1{Matrix{identity, 3, 3}};
    Matrix mat2{generateZeroMatrix(3, 3)};
    Matrix mat3{generateZeroMatrix(9, 1)};
    Matrix mat4{Matrix{{16, 0, 0, 0, 16, 0, 0, 0, 16}, 3, 3}};

            SUBCASE("== operator") {
                CHECK(mat1 == mat1);
                CHECK(mat1 == Matrix{identity, 3, 3});
                CHECK(mat4 == 16 * mat1);
                CHECK_THROWS(mat3.operator==(Matrix{{-1, 0, 0, 0, -1, 0, 0, 0, -1}, 1, 9}));
                CHECK_THROWS(mat2.operator==(mat3));
    }

            SUBCASE("!= operator") {
                CHECK(mat1 != mat2);
                CHECK(mat1 != mat4);
                CHECK_THROWS(mat2.operator!=(mat3));
                CHECK(Matrix{{0.0}, 1, 1} == Matrix{{-0.0}, 1, 1}); // -0.0 == 0.0
    }

            SUBCASE("< operator") {
                CHECK(mat1 < mat4);
                CHECK_FALSE(mat1 < mat1);
                CHECK(mat2 < mat1);
                CHECK_THROWS(mat2.operator<(mat3));
    }

            SUBCASE("<= operator") {
                CHECK(mat3 <= mat3);
                CHECK(mat1 <= mat4);
                CHECK(mat2 <= mat1);
                CHECK_THROWS(mat2.operator<=(mat3)); // different dimensions
    }

            SUBCASE("> operator") {
                CHECK_FALSE(mat3 > mat3);
                CHECK(mat4 > mat1);
                CHECK_THROWS(mat2.operator>(mat3));
    }

            SUBCASE(">= operator") {
                CHECK_FALSE(mat2 >= mat1);
                CHECK(mat4 >= mat2);
                CHECK_THROWS(mat2.operator>=(mat3));
    }

}

TEST_CASE ("Unary and Binary Operators (between matrices)") {
    // setup (runs before each subcase)
    Matrix mat1{Matrix{identity, 3, 3}};
    Matrix mat2{Matrix{{2, 0, 0, 0, 2, 0, 0, 0, 2}, 3, 3}};
    Matrix mat3{Matrix{{2, 1, 1, 1, 2, 1, 1, 1, 2}, 3, 3}};
    Matrix mat4{Matrix{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 1, 10}};
    Matrix mat5{Matrix{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 1, 10}};
    Matrix mat6{Matrix{{11.5, 22.6, 33.7, 44.8, 55.9}, 5, 1}};
    Matrix mat7{Matrix{{12.5, 23.6, 34.7, 45.8, 56.9}, 5, 1}};
    Matrix mat8{{2, 4, 6}, 3, 1};
    Matrix mat9{{1, 2, 3}, 3, 1};
    Matrix mat10{{1, 2, 3}, 1, 3};

            SUBCASE("- unary operator") {
                CHECK(-mat1 == Matrix{{-1, 0, 0, 0, -1, 0, 0, 0, -1}, 3, 3});
                CHECK(Matrix{{-12.5, -23.6, -34.7, -45.8, -56.9}, 5, 1} == -mat7);
    }

            SUBCASE("+ unary operator") {
        Matrix mat11{{-1, 0, 0, 0, -1, 0, 0, 0, -1}, 3, 3};
        Matrix mat12{{-0.0}, 1, 1};
                CHECK(mat1 == +mat1);
                CHECK(mat11 == +mat11);
                CHECK(mat12 == +mat12);
    }

            SUBCASE("+= operator") {
        Matrix mat11(Matrix{{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 10, 1});
        Matrix mat12(Matrix{{1, 1, 1, 1, 1}, 5, 1});
                CHECK((mat1 += mat1) == mat2);
                CHECK_THROWS(mat4 += mat11);
                CHECK((mat6 += mat12) == mat7);
                CHECK_THROWS(mat11 += mat1);
    }

            SUBCASE("-= operator") {
        mat8 -= mat9;
                CHECK(mat8 == mat9);
                CHECK_THROWS(mat8 -= mat10);
        Matrix zeros_mat{generateZeroMatrix(3, 3)};
        mat1 -= zeros_mat;
                CHECK(mat1 == Matrix{identity, 3, 3});
    }

            SUBCASE("*= scalar operator") {
                CHECK(mat2 == 2 * mat1);
                CHECK(mat8 == 2 * mat9);
                CHECK_THROWS(mat10.operator==(2 * mat8));
    }

            SUBCASE("++ prefix") {
                CHECK(++mat1 == mat3);
                CHECK(++mat4 == mat5);
                CHECK(++mat6 == mat7);
    }

            SUBCASE("++ postfix") {
                CHECK(mat1++ != mat2);
                CHECK(mat1 == mat3);
                CHECK(mat4++ != mat5);
                CHECK(mat4 == mat5);
                CHECK(mat6++ != mat6); // left is before increment, right is after increment
                CHECK(mat6 == mat7);
    }

            SUBCASE("-- prefix") {
                CHECK(mat1 == --mat3);
                CHECK(mat4 == --mat5);
                CHECK(mat6 == --mat7);
    }

            SUBCASE("-- postfix") {
                CHECK(mat1 != mat3--);
                CHECK(mat1 == mat3);
                CHECK(mat4 != mat5--);
                CHECK(mat4 == mat5);
                CHECK(mat7-- != mat7); // left is before decrement, right is after decrement
                CHECK(mat6 == mat7);
    }

            SUBCASE("Operators with zero matrix") {
        Matrix zero_mat{generateZeroMatrix(3, 3)};
        Matrix ones_mat{{1, 1, 1, 1, 1, 1, 1, 1, 1}, 3, 3};
                CHECK((mat1 += zero_mat) == mat1);
                CHECK((mat2 -= 2 * mat1) == zero_mat);
                CHECK_THROWS(zero_mat + mat4);
                CHECK_THROWS(zero_mat * mat5);
                CHECK(zero_mat++ != ones_mat);
                CHECK(zero_mat-- == ones_mat);
                CHECK(zero_mat == --ones_mat);
    }
}

TEST_CASE ("Output stream") {
    /*
     * stringstream allows a string object to be treated as a stream (both input and output).
     * insertion << and extraction >> operators, work like i/ostream.
     *
     * I used this to get and validate the output of a matrix object.
     */
    std::stringstream stream;

            SUBCASE("Output 1") {
        Matrix mat{identity, 3, 3};
        stream << mat;
                CHECK(stream.str() == "[1 0 0]\n"
                                      "[0 1 0]\n"
                                      "[0 0 1]");
    }

            SUBCASE("Output 2") {
        Matrix mat{{11.5, 22.6, 33.7, 44.8, 55.9}, 1, 5};
        stream << mat;
                CHECK(stream.str() == "[11.5 22.6 33.7 44.8 55.9]");
    }

            SUBCASE("Output 3") {
        Matrix mat{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 10, 1};
        stream << mat;
                CHECK(stream.str() == "[1]\n"
                                      "[2]\n"
                                      "[3]\n"
                                      "[4]\n"
                                      "[5]\n"
                                      "[6]\n"
                                      "[7]\n"
                                      "[8]\n"
                                      "[9]\n"
                                      "[10]");
    }

            SUBCASE("Output 4") {
        Matrix mat{{5, 8, 24, 30, 23, 45, 16, -5.7, 0.0, 0, -4, 7}, 4, 3};
        stream << mat;
                CHECK(stream.str() == "[5 8 24]\n"
                                      "[30 23 45]\n"
                                      "[16 -5.7 0]\n"
                                      "[0 -4 7]");
    }

            SUBCASE("Output 5") {
        Matrix mat{{-0.0, 0, 0, 0, -0, 0, 0, 0, -0.00}, 3, 3}; // should print zeros without negative sign
        stream << mat;
                CHECK(stream.str() == "[0 0 0]\n"
                                      "[0 0 0]\n"
                                      "[0 0 0]");
    }

            SUBCASE("Output 6") {
        Matrix mat{{-0.0}, 1, 1};
        stream << mat;
                CHECK(stream.str() == "[0]\n");
    }
}


TEST_CASE ("Input Stream") {
    std::stringstream stream;
    Matrix matrix{{0}, 1, 1};

            SUBCASE("Input 1") {
        stream << "[-0.0]\n";
                CHECK_NOTHROW(stream >> matrix);
    }

            SUBCASE("Input 2") {
        stream << "[1.1.55]\n";
                CHECK_THROWS(stream >> matrix);
    }

            SUBCASE("Input 3") {
        stream << "[-1 0 0], [-10 0]\n";
                CHECK_THROWS(stream >> matrix);
    }

            SUBCASE("Input 4") {
        stream << "[1,]\n";
                CHECK_THROWS(stream >> matrix);
    }
}