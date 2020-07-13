#include <iostream>
#include "matrix.hpp"

using namespace std;

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

    return 0;
}

