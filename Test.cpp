#include <random>
#include <sstream>
#include "doctest.h"
#include "Matrix.hpp"

using namespace zich;

std::vector<double> identity = {1, 0, 0, 0, 1, 0, 0, 0, 1};

Matrix generateZeroMatrix(int rows, int cols) {
    std::vector<double> matrix(static_cast<uint>(rows * cols), 0);
    return Matrix{matrix, rows, cols};
}

// https://stackoverflow.com/questions/14638739/generating-a-random-double-between-a-range-of-values
Matrix generateRandomMatrix(int rows, int cols) {
    std::vector<double> vector;
    vector.reserve(static_cast<uint>(rows * cols));
    std::uniform_real_distribution<double> dist(-500, 500);  // (min, max)
    // Mersenne Twister: Good quality random number generator
    std::mt19937 rng;
    // Initialize with non-deterministic seeds
    rng.seed(std::random_device{}());
    // generate rows*cols amount of random numbers
    for (int i = 0; i < (rows * cols); i++) {
        vector.push_back(dist(rng));
    }
    return Matrix{vector, rows, cols};
}

TEST_CASE ("Negative Dimensions") {
            CHECK_THROWS(Matrix({}, 0, 0););
            CHECK_THROWS(Matrix({0, 1}, -2, -1););
            CHECK_NOTHROW((Matrix{identity, 3, 3}));
            CHECK_THROWS((Matrix{identity, -3, 3}));
            CHECK_THROWS((Matrix{identity, 3, -3}));
            CHECK_THROWS((Matrix{identity, -3, -3}));
}

TEST_CASE ("Dimensions do not match vector size") {
            CHECK_THROWS((Matrix{identity, 3, 1}));
            CHECK_THROWS((Matrix{identity, 0, 0}));
            CHECK_THROWS((Matrix{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 2, 9}));
            CHECK_THROWS((Matrix{{11, 22, 33, 44, 55}, 4, 1}));
}

TEST_CASE ("Matrix multiplication with wrong dimensions") {
    Matrix mat1(Matrix{identity, 3, 3});
    Matrix mat2 = generateZeroMatrix(20, 3);
    Matrix mat3 = generateRandomMatrix(5, 1);
    Matrix mat4 = generateRandomMatrix(1, 5);

            CHECK_THROWS(mat1 * mat2);
            CHECK_NOTHROW(mat2 * mat1);

            CHECK_THROWS(mat4 * mat3);
            CHECK_NOTHROW(mat3 * mat4);

}

TEST_CASE ("Compare") {
    Matrix mat1(Matrix{identity, 3, 3});
    Matrix mat2 = generateZeroMatrix(3, 3);
    Matrix mat3 = generateZeroMatrix(9, 1);
    Matrix mat4(Matrix{{16, 0, 0, 0, 16, 0, 0, 0, 16}, 3, 3});
    Matrix mat5 = generateRandomMatrix(25, 9);

            CHECK(mat1 != mat2);
            CHECK_THROWS(bool check(mat2 == mat3));
            CHECK_THROWS(if (mat2 == mat3));

            CHECK_THROWS(bool check(mat2 == mat3));
            CHECK_THROWS(bool check(mat2 == mat3));
            CHECK_THROWS(if (mat2 == mat3));
            CHECK(mat4 == 16 * mat1);

            CHECK(mat2 < mat1);
            CHECK(mat2 <= mat1);
            CHECK(mat5 <= mat5);
            CHECK_THROWS(bool check(mat2 < mat3)); // different dimensions

            CHECK(mat4 > mat1);
            CHECK(mat5 >= mat5);

//            CHECK_THROWS(mat2 <= mat3);

}

TEST_CASE ("+ - operator") {
    Matrix mat1(Matrix{identity, 3, 3});
    Matrix mat2(Matrix{{2, 0, 0, 0, 2, 0, 0, 0, 2}, 3, 3});
    Matrix mat3(Matrix{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 1, 9});
    Matrix mat4(Matrix{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 1, 9});
    Matrix mat5(Matrix{{11.5, 22.6, 33.7, 44.8, 55.9}, 5, 1});
    Matrix mat6(Matrix{{12.5, 23.6, 34.7, 45.8, 56.9}, 5, 1});

            SUBCASE("+= operator") {
        Matrix mat7(Matrix{{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 9, 1});
        Matrix mat8(Matrix{{1, 1, 1, 1, 1}, 5, 1});
                CHECK((mat1 += mat1) == mat2);
                CHECK_THROWS(mat3 += mat7);
                CHECK((mat5 += mat8) == mat6);
                CHECK_THROWS(mat7 += mat1);

    }
            SUBCASE("++ prefix") {
                CHECK(++mat1 == mat2);
                CHECK(++mat3 == mat4);
                CHECK(++mat5 == mat6);
    }
            SUBCASE("++ postfix") {
                CHECK(mat1++ == mat2);
                CHECK(mat3++ == mat4);
                CHECK(mat5++ == mat6);
    }
            SUBCASE("-- prefix") {
                CHECK(mat1 == --mat2);
                CHECK(mat3 == --mat4);
                CHECK(mat5 == --mat6);
    }
            SUBCASE("-- postfix") {
                CHECK(mat1 == mat2--);
                CHECK(mat3 == mat4--);
                CHECK(mat5 == mat6--);
    }
            SUBCASE("Operators with zero matrix") {
        Matrix zero_mat = generateZeroMatrix(3, 3);
        Matrix ones_mat({{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 3, 3});
                CHECK((mat1 += zero_mat) == mat1);
                CHECK((mat2 -= 2 * mat1) == zero_mat);
                CHECK_THROWS(zero_mat + mat3);
                CHECK_THROWS(zero_mat * mat4);
                CHECK(zero_mat++ == ones_mat);
    }

}

TEST_CASE ("Matrix multiplication") {
    Matrix mat1(Matrix{identity, 3, 3});
    Matrix mat2(Matrix{{16.4, 0, 0, 0, 16.4, 0, 0, 0, 16.4}, 3, 3});
    Matrix mat3 = generateRandomMatrix(9, 1);
    Matrix mat4 = generateZeroMatrix(9, 1);

            CHECK((mat1 * mat2) == (16.4 * mat1));
            CHECK_THROWS(mat1 * mat3);

            CHECK((mat3 * mat4) == mat4);

}


TEST_CASE ("Deep copy check") {
    Matrix mat1 = generateRandomMatrix(12, 5);
    Matrix mat2(mat1);
    Matrix mat3 = mat1;

            CHECK(mat1 == mat2);
            CHECK(&mat1 != &mat2);

            CHECK(mat1 == mat3);
            CHECK(&mat1 != &mat3);

}

TEST_CASE ("Output stream") {
    Matrix mat1{identity, 3, 3};
    std::stringstream stream;
    stream << mat1;
            CHECK(stream.str() == "[1 0 0]\n"
                                  "[0 1 0]\n"
                                  "[0 0 1]\n");

    Matrix mat2(Matrix{{11.5, 22.6, 33.7, 44.8, 55.9}, 5, 1});
    stream << mat2;
            CHECK(stream.str() == "[11.5 22.6 33.7 44.8 55.9]\n");

    Matrix mat3(Matrix{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 1, 9});
    stream << mat3;
            CHECK(stream.str() == "[1]\n"
                                  "[2]\n"
                                  "[3]\n"
                                  "[4]\n"
                                  "[5]\n"
                                  "[6]\n"
                                  "[7]\n"
                                  "[8]\n"
                                  "[9]\n"
                                  "[10]\n");
}
// todo: output input stream tests
