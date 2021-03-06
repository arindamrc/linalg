#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <assert.h>
#include <limits>
#include <iostream>
#include <algorithm>
#include <functional>

#define MAX_SZ (std::size_t)-1

/*
 * An expression template for element-wise matrix operations
 * like addition and subtractions. The expression template
 * allows addition of more than two matrices without creating
 * intermediate temporaries.
 *
 * See: https://en.wikipedia.org/wiki/Expression_templates
*/
template <typename E>
class MatOp {

public:

    // use 'auto' as the return type since the type
    // of the subclass 'Matrix' isn't known yet.
    auto operator()(std::size_t i, std::size_t j) const {
        return static_cast<E const&>(*this)(i,j);
    }

    std::size_t size() const {
        return static_cast<E const&>(*this).size();
    }

    std::size_t rows() const {
        return static_cast<E const&>(*this).rows();
    }

    std::size_t cols() const {
        return static_cast<E const&>(*this).cols();
    }

};


///////////////////////Matrix implementation///////////////////////


/*
 * A two dimensional single channel templated matrix implementaion.
 * This class supports template arguments that are arithmetic types
 * i.e. integers or floats.
*/
template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class Matrix : public MatOp<Matrix<T>>
{

    // make sure we instatiate the correct template types.
//    static_assert (std::is_arithmetic<T>::value, "Matrix can only contain elements that support arithmetic operations!");

protected:

    /* The rows and columns of the 2D matrix. */
    std::size_t rows_, cols_, size_;

    /*
     * Underlying container that holds the matrix elements.
     * 'vector' is chosen because it's easy to work with.
     * We make the assumption that the size of the matrix is
     * known only at runtime.
    */
    std::vector<T> data_;

public:

    /*
     * Construct a matrix with the given dimensions r and c
     * and fill it with zeros.
    */
    Matrix(const std::size_t r, const std::size_t c) : Matrix(r, c, T(0)){
    }

    /*
     * Construct a matrix with the given dimensions r and c
     * and fill it with val.
    */
    template<typename U, typename std::enable_if<std::is_convertible<T, U>::value>::type* = nullptr>
    Matrix(const std::size_t r, const std::size_t c, const U val){
        assert (r > 0 && c > 0 && r < MAX_SZ && c < MAX_SZ);
        rows_ = r;
        cols_ = c;
        size_ = r * c;
        data_.resize(r * c, val);
    }

    /*
     * Copy constructor.
    */
    template<typename U, typename std::enable_if<std::is_convertible<T, U>::value>::type* = nullptr>
    Matrix(const Matrix<U> &other){
        rows_ = other.rows();
        cols_ = other.cols();
        size_ = other.size();
        data_.resize(size_);

        #pragma omp parallel for collapse(2)
        for (std::size_t i = 0; i < rows_; i++) {
            for (std::size_t j = 0; j < cols_; j++) {
                (*this)(i,j) = other(i,j);
            }
        }
    }

    /*
     * Construct a matrix with the given dimensions r and c
     * and fill it with valarray.
    */
    template<typename U, typename std::enable_if<std::is_convertible<T, U>::value>::type* = nullptr>
    Matrix(const std::size_t r, const std::size_t c, const U* valarray){
        assert (r > 0 && c > 0 && r < MAX_SZ && c < MAX_SZ);
        rows_ = r;
        cols_ = c;
        size_ = r * c;
        data_.resize(r * c);

        for (std::size_t i = 0; i < size_; i++) {
            data_ = valarray[i];
        }
    }

    /*
     * Construct a matrix with the given values.
     * The matrix is assumed to have a single row.
    */
    template<typename U, typename std::enable_if<std::is_convertible<T, U>::value>::type* = nullptr>
    Matrix(const std::initializer_list<U> vals){
        data_.resize(vals.size());

        int i = 0;
        for(const auto& elem : vals)
        {
            data_[i++] = elem;
        }

        size_ = vals.size();
        rows_ = 1;
        cols_ = vals.size();
    }

    /*
     * Constructor for a matrix operation.
    */
    template<typename E>
    Matrix(MatOp<E> const& op) : Matrix(op.rows(), op.cols()) {

        #pragma omp parallel for collapse(2)
        for (std::size_t i = 0; i < op.rows(); i++) {
            for (std::size_t j = 0; j < op.cols(); j++) {
                (*this)(i,j) = op(i,j);
            }
        }
    }

    /*
     * Overloaded assignment operator.
    */
    template<typename U, typename std::enable_if<std::is_convertible<T, U>::value>::type* = nullptr>
    Matrix<T>& operator=(const Matrix<U>& other){
        if (&other == this) {
            return *this;
        }

        rows_ = other.rows();
        cols_ = other.cols();
        size_ = other.size();

        data_.resize(size_);

        #pragma omp parallel for collapse(2)
        for (std::size_t i = 0; i < rows_; i++) {
            for (std::size_t j = 0; j < cols_; j++) {
                (*this)(i,j) = other(i,j);
            }
        }

        return *this;
    }

    /*
     * Overload assignment operator to allow initialization.
    */
    template<typename U, typename std::enable_if<std::is_convertible<T, U>::value>::type* = nullptr>
    Matrix<T>& operator=(std::initializer_list<U> vals){
        assert(size_ == vals.size());

        int i = 0;
        for(const auto& elem : vals)
        {
            data_[i++] = elem;
        }

        return *this;
    }

    /*
     * The default destructor (empty).
    */
    virtual ~Matrix() {};

    /*
     * Size of the matrix or the total number of elements in it.
    */
    std::size_t size() const{
        return size_;
    }

    /*
     * Resize the matrix.
    */
    void resize(std::size_t r, std::size_t c) {
        assert(r * c == size_);
        rows_ = r;
        cols_ = c;
    }

    /*
     * Get the transpose of this matrix.
    */
    Matrix<T> t() const {
        Matrix<T> result(cols_, rows_, T(0));

        #pragma omp parallel for collapse(2)
        for (std::size_t i = 0; i < result.rows(); i++) {
            for (std::size_t j = 0; j < result.cols(); j++) {
                result(i,j) = (*this)(j,i);
            }
        }

        return result;
    }

    /*
     * Element access operator: mat(i,j).
    */
    const T& operator()(std::size_t i, std::size_t j) const {
        assert (i < rows_ && j < cols_);
        return data_[i * cols_ + j];
    }

    /*
     * Element mutation operator: mat(i,j).
    */
    T& operator()(std::size_t i, std::size_t j) {
        assert (i < rows_ && j < cols_);
        return data_[i * cols_ + j];
    }

    /*
     * The number of rows in the matrix.
    */
    std::size_t rows() const {
        return rows_;
    }

    /*
     * The number of columns in the matrix.
    */
    std::size_t cols() const {
        return cols_;
    }

};



///////////////Matrix Operations///////////////


/* Element-wise addition. */
template <typename E1, typename E2>
class MatSum : public MatOp<MatSum<E1, E2>> {

private:

    E1 const& _u;
    E2 const& _v;

public:

    MatSum(E1 const& u, E2 const& v) : _u(u), _v(v) {
        assert(u.size() == v.size());
        assert(u.rows() == v.rows());
        assert(u.cols() == v.cols());
    }

    auto operator()(std::size_t i, std::size_t j) const {
        return _u(i,j) + _v(i,j);
    }

    std::size_t size() const {
        return _u.size();
    }

    std::size_t rows() const {
        return static_cast<E1 const&>(_u).rows();
    }

    std::size_t cols() const {
        return static_cast<E1 const&>(_u).cols();
    }
};

template <typename E1, typename E2>
MatSum<E1, E2> operator+(MatOp<E1> const& u, MatOp<E2> const& v) {
   return MatSum<E1, E2>(*static_cast<const E1*>(&u), *static_cast<const E2*>(&v));
}



/* Element-wise subtraction. */
template <typename E1, typename E2>
class MatSub : public MatOp<MatSub<E1, E2>> {

private:

    E1 const& _u;
    E2 const& _v;

public:

    MatSub(E1 const& u, E2 const& v) : _u(u), _v(v) {
        assert(u.size() == v.size());
        assert(u.rows() == v.rows());
        assert(u.cols() == v.cols());
    }

    auto operator()(std::size_t i, std::size_t j) const {
        return _u(i,j) - _v(i,j);
    }

    std::size_t size() const {
        return _u.size();
    }

    std::size_t rows() const {
        return static_cast<E1 const&>(_u).rows();
    }

    std::size_t cols() const {
        return static_cast<E1 const&>(_u).cols();
    }
};

template <typename E1, typename E2>
MatSub<E1, E2> operator-(MatOp<E1> const& u, MatOp<E2> const& v) {
   return MatSub<E1, E2>(*static_cast<const E1*>(&u), *static_cast<const E2*>(&v));
}



/* Element-wise multiplication. */
template <typename E1, typename E2>
class MatElMul : public MatOp<MatElMul<E1, E2>> {

private:

    E1 const& _u;
    E2 const& _v;

public:

    MatElMul(E1 const& u, E2 const& v) : _u(u), _v(v) {
        assert(u.size() == v.size());
        assert(u.rows() == v.rows());
        assert(u.cols() == v.cols());
    }

    auto operator()(std::size_t i, std::size_t j) const {
        return _u(i,j) * _v(i,j);
    }

    std::size_t size() const {
        return _u.size();
    }

    std::size_t rows() const {
        return static_cast<E1 const&>(_u).rows();
    }

    std::size_t cols() const {
        return static_cast<E1 const&>(_u).cols();
    }
};

/* The element wise multiplication is defined to be & instead of *, which is reserved
 * for matrix multiplication.
*/
template <typename E1, typename E2>
MatElMul<E1, E2> operator&(MatOp<E1> const& u, MatOp<E2> const& v) {
   return MatElMul<E1, E2>(*static_cast<const E1*>(&u), *static_cast<const E2*>(&v));
}


/* Scalar multiplication. */
template <typename E, typename T>
class MatScale : public MatOp<MatScale<E, T>> {

private:

    E const& _u;
    T const& _v;

public:

    MatScale(E const& u, T const& v) : _u(u), _v(v) {
    }

    auto operator()(std::size_t i, std::size_t j) const {
        return _u(i,j) * _v;
    }

    std::size_t size() const {
        return _u.size();
    }

    std::size_t rows() const {
        return static_cast<E const&>(_u).rows();
    }

    std::size_t cols() const {
        return static_cast<E const&>(_u).cols();
    }
};

/* Overload the scalar multiplication so that it can be done on both sides. */

template <typename E, typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
MatScale<E, T> operator*(MatOp<E> const& u, T const& v) {
   return MatScale<E, T>(*static_cast<const E*>(&u), *static_cast<const T*>(&v));
}

template <typename E, typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
MatScale<E, T> operator*(T const& v, MatOp<E> const& u) {
   return MatScale<E, T>(*static_cast<const E*>(&u), *static_cast<const T*>(&v));
}



/* Scalar Addition. */
template <typename E, typename T>
class MatShift : public MatOp<MatShift<E, T>> {

private:

    E const& _u;
    T const& _v;

public:

    MatShift(E const& u, T const& v) : _u(u), _v(v) {
    }

    auto operator()(std::size_t i, std::size_t j) const {
        return _u(i,j) + _v;
    }

    std::size_t size() const {
        return _u.size();
    }

    std::size_t rows() const {
        return static_cast<E const&>(_u).rows();
    }

    std::size_t cols() const {
        return static_cast<E const&>(_u).cols();
    }
};

/* Overload the scalar addition so that it can be done on both sides. */

template <typename E, typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
MatShift<E, T> operator+(MatOp<E> const& u, T const& v) {
   return MatShift<E, T>(*static_cast<const E*>(&u), *static_cast<const T*>(&v));
}

template <typename E, typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
MatShift<E, T> operator+(T const& v, MatOp<E> const& u) {
   return MatShift<E, T>(*static_cast<const E*>(&u), *static_cast<const T*>(&v));
}




/* Matrix multiplication. */
template <typename E1, typename E2>
class MatMul : public MatOp<MatMul<E1, E2>> {

private:

    E1 const& _u;
    E2 const& _v;
    E2 const _vt;

public:

    MatMul(E1 const& u, E2 const& v) : _u(u), _v(v), _vt(v.t()) {
        // pre-condition for matrix multiplication.
        assert(u.cols() == v.rows());
    }

    auto operator()(std::size_t i, std::size_t j) const {
        auto sum = 0;
        for (std::size_t c = 0; c < _u.cols(); c++) {
            sum += _u(i,c) * _vt(j,c);
        }
        return sum;
    }

    std::size_t size() const {
        return _u.rows() * _v.cols();
    }

    std::size_t rows() const {
        return static_cast<E1 const&>(_u).rows();
    }

    std::size_t cols() const {
        return static_cast<E2 const&>(_v).cols();
    }
};

/* For matrix multiplication, the standard * operator should be used. */
template <typename E1, typename E2>
MatMul<E1, E2> operator*(MatOp<E1> const& u, MatOp<E2> const& v) {
   return MatMul<E1, E2>(*static_cast<const E1*>(&u), *static_cast<const E2*>(&v));
}




/* Helper function to print matrix to output stream. */
template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& mat){
    os << std::endl;
    for (std::size_t i=0; i < mat.rows(); i++) {
        for (std::size_t j = 0; j < mat.cols(); j++) {
            os << mat(i,j) << ' ';
        }
        os << std::endl;
    }
    os << std::endl;
    return os;
}

#endif // MATRIX_H
