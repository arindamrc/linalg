#include <iostream>
//#include "matrix.hpp"
#include "vector.hpp"

int main()
{
    // does not work with mixed matrix types!
    Matrix<double> m1(2,2);
    m1 = {1, 2, 3, 4};
    std::cout << "m1:" << m1;

    Matrix<double> m2(2,2);
    m2 = {1, 2, 3, 4};
    std::cout << "m2:" << m2;

    Matrix<double> m3 = m1 + m2;
    std::cout << "m3 = m1 + m2:" << m3;

    Matrix<double> m4 = m1 & m2;
    std::cout << "m4 = m1 * m2 (Element-wise):" << m4;

    Matrix<double> m5 = m1 - m2;
    std::cout << "m5 = m1 - m2:" << m5;

    Matrix<double> m6 = 2.0 * m2;
    std::cout << "m6 = 2.0 * m2:" << m6;

    Matrix<double> m7 = m2 * 3.0;
    std::cout << "m7 = m2 * 3.0:" << m7;

    Matrix<double> m1t = m1.t();
    std::cout << "m1t = m1.t()" << m1t;

    Matrix<double> m8 = m1 * m2;
    std::cout << "m8 = m1 * m2" << m8;

    Vector<double> v1(2);
    v1 = {1,2};
    std::cout << "v1:" << v1;

    Vector<double> v2 = {3,4};
    std::cout << "v2:" << v2;

    Vector<double> v3 = v1 + v2;
    std::cout << "v3:" << v3;

    Vector<double> v4 = v1 & v2;
    std::cout << "v4:" << v4;

    Vector<double> v5 = v2 - v1 + v2;
    std::cout << "v5:" << v5;

    Matrix<double> m9 = v1 * m1;
    std::cout << "m9:" << m9;

    Matrix<double> m10 = m1 * v1.t();
    std::cout << "m10:" << m10;

    Vector<double> m11 = 3.0 * v1;
    std::cout << "m11:" << m11;

    Vector<double> m12 = v1 * 3.0;
    std::cout << "m12:" << m12;

    Vector<double> m13 = 3.0 * v1 + v2;
    std::cout << "m13:" << m13;

    Matrix<double> m14 = m1 + 10.0;
    std::cout << "m14:" << m14;

    Matrix<double> m15 = 10.0 + m2 + m1;
    std::cout << "m15:" << m15;

    Vector<double> v6 = v1 * v2.t();
    std::cout << "v6:" << v6;

    std::cout << "v1(0):" << v1(0) << std::endl;

    std::cout << "m1(0,1):" << m1(0,1) << std::endl;

    return 0;
}

