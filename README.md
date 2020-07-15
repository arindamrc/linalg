## Matrix and vector implementation in C++.

It supports the following operations:
1. Typed matrices and vectors
2. Addition, subtraction and element-wise multiplication of matrices and vectors.
3. Scalar addition and multiplication for both matrix and vector.
3. Multiplication of two matrices or two vectors or a vector and a matrix (both pre and post).

Build system is qmake.

To build, navigate to project folder and type:

$ qmake -o Makefile linalg.pro

This generates the Makefile. To generate the executable target, again on the command prompt type:

$ make

This generates the executable binary call `linalg`. 

Execute it by typing:

$ ./linalg