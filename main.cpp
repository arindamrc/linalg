#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <chrono>

#include "vector.hpp"

int main()
{
    // Matrix creation
    Matrix<double> m1(2,2);
    m1 = {1, 2, 3, 4};
    std::cout << "m1:" << m1;

    Matrix<double> m2(2,2);
    m2 = {1, 2, 3, 4};
    std::cout << "m2:" << m2;

    // Matrix addition
    Matrix<double> m3 = m1 + m2;
    std::cout << "m3 = m1 + m2:" << m3;

    // Element-wise multiplication
    Matrix<double> m4 = m1 & m2;
    std::cout << "m4 = m1 * m2 (Element-wise):" << m4;

    // Matrix subtraction
    Matrix<double> m5 = m1 - m2;
    std::cout << "m5 = m1 - m2:" << m5;

    // Scalar multiplication (pre)
    Matrix<double> m6 = 2.0 * m2;
    std::cout << "m6 = 2.0 * m2:" << m6;

    // Scalar multiplication (post)
    Matrix<double> m7 = m2 * 3.0;
    std::cout << "m7 = m2 * 3.0:" << m7;

    // Transpose
    Matrix<double> m1t = m1.t();
    std::cout << "m1t = m1.t()" << m1t;

    // Matrix multiplication
    Matrix<double> m8 = m1 * m2;
    std::cout << "m8 = m1 * m2" << m8;

    // Vector creation
    Vector<double> v1(2);
    v1 = {1,2};
    std::cout << "v1:" << v1;

    Vector<double> v2 = {3,4};
    std::cout << "v2:" << v2;

    // Vector addition
    Vector<double> v3 = v1 + v2;
    std::cout << "v3:" << v3;

    // Vector element-wise multiplication
    Vector<double> v4 = v1 & v2;
    std::cout << "v4:" << v4;

    // Vector chained operations (also supported for matrices)
    Vector<double> v5 = v2 - v1 + v2;
    std::cout << "v5:" << v5;

    // Vector-matrix multiplication
    Matrix<double> m9 = v1 * m1;
    std::cout << "m9:" << m9;

    // Matrix-vector multiplication
    Matrix<double> m10 = m1 * v1.t();
    std::cout << "m10:" << m10;

    // Vector scalar multiplication (pre)
    Vector<double> m11 = 3.0 * v1;
    std::cout << "m11:" << m11;

    // Vector scalar multiplication (post)
    Vector<double> m12 = v1 * 3.0;
    std::cout << "m12:" << m12;

    // Vector mixed operations
    Vector<double> m13 = 3.0 * v1 + v2;
    std::cout << "m13:" << m13;

    // Matrix scalar addition (post)
    Matrix<double> m14 = m1 + 10.0;
    std::cout << "m14:" << m14;

    // Matrix scalar addition (pre)
    Matrix<double> m15 = 10.0 + m2 + m1;
    std::cout << "m15:" << m15;

    // Vector-vector multiplication
    Vector<double> v6 = v1 * v2.t();
    std::cout << "v6:" << v6;

    // Access operations
    std::cout << "v1(0):" << v1(0) << std::endl;
    std::cout << "m1(0,1):" << m1(0,1) << std::endl;

    // Mixed type assignment
    Vector<int> d1 = {1.0, 2.0};
    std::cout << "d1:" << d1;

    // Matrix resize operation
    Matrix<int> m16 = {1,2,3,4,5,6};
    std::cout << "m16:" << m16 << std::endl;
    m16.resize(2,3);
    std::cout << "m16:" << m16 << std::endl;

    Matrix<double> m17 = m16;
    std::cout << "m17:" << m17 << std::endl;
    m17.resize(3,2);
    std::cout << "m17:" << m17 << std::endl;

    // Mixed type matrix multiplication
    Matrix<double> m18 = m16 * m17;
    std::cout << "m18:" << m18 << std::endl;

    // Large matrix operation chains
    srand(42);
    Matrix<double> l1(500, 500);
    Matrix<double> l2(500, 500);

    for (std::size_t i = 0; i < 500; i++) {
        for (std::size_t j=0; j < 500; j++) {
            l1(i,j) = (rand() % 1000) + 1;
            l2(i,j) = (rand() % 1000) + 1;
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    Matrix<double> l3 = (l1 + l2) - (0.5 * l1) + (l1 + 10);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Chained operation time:" << duration.count() / 1000 << " milliseconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    Matrix<double> l4 = l1 * l3;
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Matrix multiplication time:" << duration.count() / 1000 << " milliseconds" <<  std::endl;

    return 0;
}

