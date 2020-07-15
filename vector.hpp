#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "matrix.hpp"

/*
 * Vector is implemented as a matric with
 * only one column but n rows.
*/
template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
class Vector : public Matrix<T>
{

    // make sure we instatiate the correct template types.
    static_assert (std::is_arithmetic<T>::value, "Matrix can only contain elements that support arithmetic operations!");

    typedef Matrix<T> super_type;

public:

    /*
     * Construct a vector with the given size s
     * and fill it with zeros.
    */
    Vector(const std::size_t s) : Matrix<T>(1, s) {}

    /*
     * Construct a vector with the given size s
     * and fill it with val.
    */
    template<typename U, typename std::enable_if<std::is_convertible<T, U>::value>::type* = nullptr>
    Vector(const std::size_t s, const U val) : Matrix<T>(1, s, val) {}

    /*
     * Construct a vector with the given size s
     * and fill it with valarray.
    */
    template<typename U, typename std::enable_if<std::is_convertible<T, U>::value>::type* = nullptr>
    Vector(const std::size_t s, const U* valarray) : Matrix<T>(1, s, valarray) {}

    /*
     * Construct a vector with the given values.
    */
    template<typename U, typename std::enable_if<std::is_convertible<T, U>::value>::type* = nullptr>
    Vector(const std::initializer_list<U> vals) : Matrix<T>(vals) {}

    /*
     * Constructor for a vector operation.
    */
    template<typename E>
    Vector(MatOp<E, T> const& op) : Matrix<T>(op) {}

    /*
     * Overloaded assignment operator.
    */
    template<typename U, typename std::enable_if<std::is_convertible<T, U>::value>::type* = nullptr>
    Vector<T>& operator=(const Vector<U>& other){
        return static_cast<Vector<T>&>(super_type::operator=(other));
    }

    /*
     * Overload assignment operator to allow initialization.
    */
    template<typename U, typename std::enable_if<std::is_convertible<T, U>::value>::type* = nullptr>
    Vector<T>& operator=(std::initializer_list<U> vals){
        return static_cast<Vector<T>&>(super_type::operator=(vals));
    }

    /*
     * The default destructor (empty).
    */
    ~Vector() {};

    /*
     * Resize the vector.
    */
    void resize(__attribute__((unused)) std::size_t r, __attribute__((unused)) std::size_t c) {
        // Vector doesn't support resizing operation!
        return;
    }

    /*
     * Element access operator: vec(i).
    */
    const T& operator()(std::size_t i) const {
        assert (i < super_type::size_);
        return super_type::data_[i];
    }

    /*
     * The size of the vector.
    */
    std::size_t rows() const {
        return 1;
    }

    /*
     * The vector has only one column.
    */
    std::size_t cols() const {
        return super_type::size_;
    }
};



/* Helper function to print matrix to output stream. */
template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vec){
    os << std::endl;
    for (std::size_t i=0; i < vec.size(); i++) {
        os << vec(i) << ' ';
    }
    os << std::endl << std::endl;
    return os;
}





#endif // VECTOR_HPP
