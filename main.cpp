#include <iostream>
#include "matrix.hpp"

using namespace std;

int main()
{
    Matrix<double> m1(2,2);
    m1 = {1, 2, 3, 4};
    std::cout << m1;

    Matrix<double> m2(2,2);
    m2 = {1, 2, 3, 4};
    std::cout << m2;

    Matrix<double> m3 = m1 + m2 - m2;
    std::cout << m3;


    return 0;
}

