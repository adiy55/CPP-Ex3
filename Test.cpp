#include <random>
#include "doctest.h"
#include "Matrix.hpp"

using namespace zich;

std::vector<double> identity = {1, 0, 0, 0, 1, 0, 0, 0, 1};

//TEST_CASE ("Good input") {
//    std::vector<double> identity = {1, 0, 0, 0, 1, 0, 0, 0, 1};
//    std::vector<double> arr = {3, 0, 0, 0, 3, 0, 0, 0, 3};
//    Matrix a{identity, 3, 3};
//    Matrix b{arr, 3, 3};
//    Matrix c{{4, 0, 0, 0, 4, 0, 0, 0, 4}, 3, 3};
//            SUBCASE("+= matrix") {
//        a += b;
//                CHECK(a == c);
//    }
//
//}

TEST_CASE ("Bad Input- Dimensions") {
            SUBCASE("Negative Dimensions") {

    }
            SUBCASE("Dimensions that do not match vector size") {

    }


            SUBCASE("Operators between matrices with mismatching dimensions") {

    }


}

TEST_CASE ("Operators") {


            SUBCASE("Equals") {

    }

}

//TEST_CASE("Binary Operators")

// mat multiplication dimensions

// binary operations dimensions

// to string


// todo: deep copy test


Matrix generateZeroMatrix(int rows, int cols) {
    std::vector<double> matrix(0, rows * cols);
    return Matrix{matrix, rows, cols};
}

// https://stackoverflow.com/questions/14638739/generating-a-random-double-between-a-range-of-values
Matrix generateRandomMatrix(int rows, int cols) {
    std::vector<double> vector;
    vector.reserve(static_cast<uint>(rows * cols));
    std::uniform_real_distribution<double> dist(-500, 500);  //(min, max)
    // Mersenne Twister: Good quality random number generator
    std::mt19937 rng;
    // Initialize with non-deterministic seeds
    rng.seed(std::random_device{}());
    // generate 10 random numbers.
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


}

TEST_CASE ("Matrix multiplication with wrong dimensions") {
    Matrix mat1 = generateRandomMatrix(5, 1);
    Matrix mat2 = generateRandomMatrix(1, 5);
            CHECK_THROWS(mat2 * mat1);
            CHECK_NOTHROW(mat1 * mat2);

}

TEST_CASE ("Compare") {
    Matrix mat1(Matrix{identity, 3, 3});
    Matrix mat2 = generateZeroMatrix(3, 3);
    Matrix mat3 = generateZeroMatrix(9, 1);
    Matrix mat4(Matrix{{16, 0, 0, 0, 16, 0, 0, 0, 16}, 3, 3});
            CHECK(mat1 != mat2);
            CHECK(mat2 != mat3);
            CHECK(mat4 != 16 * mat1);
            CHECK(mat2 < mat1);
            CHECK(mat2 <= mat1);
            CHECK_THROWS(if (mat2 == mat3));
//            CHECK_THROWS(mat2 <= mat3);

}

TEST_CASE ("+ - operator") {
    Matrix mat1(Matrix{identity, 3, 3});
    Matrix mat4(Matrix{{2, 0, 0, 0, 2, 0, 0, 0, 2}, 3, 3});

//            CHECK((mat1 += 1) =1= mat4);
            SUBCASE("++") {
                CHECK(++mat1 == mat4);
    }
            SUBCASE("++ postfix") {
                CHECK(mat1++ == mat4);
    }
            SUBCASE("-- prefix") {
                CHECK(mat1 == --mat4);
    }
            SUBCASE("-- postfix") {
                CHECK(mat1 == mat4--);
    }

}

// todo: output input stream tests
