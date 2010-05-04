
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    See http://www.boost.org/libs/xint for library home page.
*/

/*! \file
    \brief Declares the \c xint::nothrow_integer class.
*/

#ifndef BOOST_INCLUDED_XINT_NOTHROW_INTEGER_HPP
#define BOOST_INCLUDED_XINT_NOTHROW_INTEGER_HPP

#include "integer.hpp"

namespace boost {
namespace xint {

/*! \brief The %nothrow_integer class.

    Functions using this class do not throw exceptions, they return special
    values (most often \ref nan "Not-a-Number") to indicate failures.

    There are only a few member functions; most of the functionality is
    implemented using standalone functions.
*/
class nothrow_integer: public detail::base_variable_length_integer {
    public:

    //! \name Constructors & Destructors
    //!@{
    nothrow_integer();
    nothrow_integer(const nothrow_integer& b);
    nothrow_integer(BOOST_XINT_RV_REF(nothrow_integer) b) { _swap(b); }
    explicit nothrow_integer(const integer& b);
    explicit nothrow_integer(const fixed_integer_any& b);
    explicit nothrow_integer(const std::string& str, size_t base=10);
    template <typename T> nothrow_integer(const T& n,
        typename boost::enable_if<boost::is_integral<T> >::type* = 0,
        typename boost::enable_if<boost::is_signed<T> >::type* = 0);
    template <typename T> nothrow_integer(const T& n,
        typename boost::enable_if<boost::is_integral<T> >::type* = 0,
        typename boost::enable_if<boost::is_unsigned<T> >::type* = 0);
    //!@}

    /*! \name Mathematical and Bitwise Operators
        \brief These operate exactly like their counterparts for the built-in
               %integer types.

        Note that there is no \c operator~. Since the nothrow_integer type
        doesn't have a fixed maximum size, there is no logical way to implement
        it.
    */
    //@{
    nothrow_integer& operator=(BOOST_XINT_COPY_ASSIGN_REF(nothrow_integer) c);
    nothrow_integer& operator=(BOOST_XINT_RV_REF(nothrow_integer) c) { _swap(c);
        return *this; }
    template <typename T> nothrow_integer& operator=(const T& n) {
        nothrow_integer nn(n); _swap(nn); return *this; }

    bool operator!() const { return _is_zero(); }
    nothrow_integer operator-() const;
    nothrow_integer& operator+() { return *this; }
    const nothrow_integer& operator+() const { return *this; }

    nothrow_integer& operator+=(const nothrow_integer& b);
    nothrow_integer& operator-=(const nothrow_integer& b);
    nothrow_integer& operator*=(const nothrow_integer& b);
    nothrow_integer& operator/=(const nothrow_integer& b);
    nothrow_integer& operator%=(const nothrow_integer& b);

    nothrow_integer& operator++(); //!< \details Preincrement operator
    nothrow_integer& operator--(); //!< \details Predecrement operator
    nothrow_integer  operator++(int); //!< \details Postincrement operator -- not recommended, inefficient
    nothrow_integer  operator--(int); //!< \details Postdecrement operator -- not recommended, inefficient

    nothrow_integer& operator&=(const nothrow_integer& n);
    nothrow_integer& operator|=(const nothrow_integer& n);
    nothrow_integer& operator^=(const nothrow_integer& n);
    nothrow_integer  operator<<(size_t shift) const;
    nothrow_integer  operator>>(size_t shift) const;
    nothrow_integer& operator<<=(size_t shift);
    nothrow_integer& operator>>=(size_t shift);
    //@}

    //! \name Miscellaneous Functions
    //!@{
    bool odd() const;
    bool even() const;
    int  sign(bool signed_zero=false) const;
    bool is_nan() const;

    size_t hex_digits() const;
    //!@}

    /*! \name Preconstructed integers

        These return preconstructed integers with the values named by the
        functions. They are used internally, so that the most common values
        don't have to be constructed each time they're needed.
    */
    //@{
    static const nothrow_integer& zero();
    static const nothrow_integer& one();
    static const nothrow_integer& nan();
    //@}

    //! \brief For internal use only.
    void _make_unique(bool);

    typedef base_divide_t<nothrow_integer> divide_t;

    private:
    BOOST_XINT_COPYABLE_AND_MOVABLE(nothrow_integer)

    static std::auto_ptr<nothrow_integer> cZero, cOne, cNaN;
};

//! \name Mathematical primitives
//!@{
nothrow_integer abs(const nothrow_integer& n);
nothrow_integer negate(const nothrow_integer& n);
nothrow_integer add(const nothrow_integer& n, const nothrow_integer& addend);
nothrow_integer subtract(const nothrow_integer& n, const nothrow_integer&
    subtrahend);
nothrow_integer multiply(const nothrow_integer& n, const nothrow_integer&
    multiplicand);
nothrow_integer divide(const nothrow_integer& dividend, const nothrow_integer&
    divisor);
nothrow_integer::divide_t divide_r(const nothrow_integer& dividend, const
    nothrow_integer& divisor);
//!@}

////////////////////////////////////////////////////////////////////////////////

//! \name Powers and roots
//!@{
nothrow_integer sqr(const nothrow_integer& n);
nothrow_integer pow(const nothrow_integer& n, const nothrow_integer& exponent);
nothrow_integer nothrow_pow2(size_t exponent);
nothrow_integer nothrow_factorial(size_t n);
nothrow_integer sqrt(const nothrow_integer& n);
//!@}

////////////////////////////////////////////////////////////////////////////////

//! \name Conversion functions
//!@{
template <typename T> T to(const nothrow_integer& n);
std::string to_string(const nothrow_integer& n, size_t base=10, bool upperCase =
    false);
nothrow_integer nothrow_from_string(const std::string& str, size_t base=10);
xint::binary_t to_binary(const nothrow_integer& n, size_t bits=0);
nothrow_integer nothrow_from_binary(const xint::binary_t& s, size_t bits=0);
//!@}

////////////////////////////////////////////////////////////////////////////////

//! \name Bit-manipulation functions
//!@{
bool getbit(const nothrow_integer& n, size_t bit);
void setbit(nothrow_integer& n, size_t bit);
void clearbit(nothrow_integer& n, size_t bit);
size_t lowestbit(const nothrow_integer& n, size_t valueIfZero=0);
size_t highestbit(const nothrow_integer& n, size_t valueIfZero=0);
nothrow_integer bitwise_and(const nothrow_integer& n1, const nothrow_integer&
    n2);
nothrow_integer bitwise_or (const nothrow_integer& n1, const nothrow_integer&
    n2);
nothrow_integer bitwise_xor(const nothrow_integer& n1, const nothrow_integer&
    n2);
nothrow_integer shift(const nothrow_integer& n, int byBits);
nothrow_integer shift_left(const nothrow_integer& n, size_t byBits);
nothrow_integer shift_right(const nothrow_integer& n, size_t byBits);
//!@}

////////////////////////////////////////////////////////////////////////////////

/*! \name Modular math functions

A common reason to use a large-integer library is to implement public-key
encryption, and the algorithms for such encryption often use modular math.
*/
//!@{
nothrow_integer mod(const nothrow_integer& n, const nothrow_integer& modBy);
nothrow_integer mulmod(const nothrow_integer& n, const nothrow_integer& by,
    const nothrow_integer& modulus);
nothrow_integer sqrmod(const nothrow_integer& n, const nothrow_integer&
    modulus);
nothrow_integer powmod(const nothrow_integer& n, const nothrow_integer&
    exponent, const nothrow_integer& modulus, bool noMontgomery=false);
nothrow_integer invmod(const nothrow_integer& n, const nothrow_integer&
    modulus);
//!@}

////////////////////////////////////////////////////////////////////////////////

//! \name Random number functions
//!@{
template <class T>
nothrow_integer nothrow_random_by_size(T& gen, size_t sizeInBits, bool
    highBitOn = false, bool lowBitOn = false, bool canBeNegative = false);
//!@}

////////////////////////////////////////////////////////////////////////////////

//! \name Prime number functions
//!@{
int is_prime(const nothrow_integer& n, callback_t callback=no_callback);
template <class T>
nothrow_integer nothrow_random_prime(T& gen, size_t sizeInBits, callback_t
    callback = no_callback);
//!@}

////////////////////////////////////////////////////////////////////////////////

/*! \name Operators

    These act exactly the same as for the built-in %integer types.
*/
//!@{
bool operator<(const nothrow_integer& n1, const nothrow_integer& n2);
bool operator>(const nothrow_integer& n1, const nothrow_integer& n2);
bool operator<=(const nothrow_integer& n1, const nothrow_integer& n2);
bool operator>=(const nothrow_integer& n1, const nothrow_integer& n2);
bool operator==(const nothrow_integer& n1, const nothrow_integer& n2);
bool operator!=(const nothrow_integer& n1, const nothrow_integer& n2);

nothrow_integer operator+(const nothrow_integer& n1, const nothrow_integer& n2);
nothrow_integer operator-(const nothrow_integer& n1, const nothrow_integer& n2);
nothrow_integer operator*(const nothrow_integer& n1, const nothrow_integer& n2);
nothrow_integer operator/(const nothrow_integer& n1, const nothrow_integer& n2);
nothrow_integer operator%(const nothrow_integer& n1, const nothrow_integer& n2);
nothrow_integer operator&(const nothrow_integer& n1, const nothrow_integer& n2);
nothrow_integer operator|(const nothrow_integer& n1, const nothrow_integer& n2);
nothrow_integer operator^(const nothrow_integer& n1, const nothrow_integer& n2);
//!@}

////////////////////////////////////////////////////////////////////////////////

//! \name Miscellaneous functions
//!@{
int compare(const nothrow_integer &n1, const nothrow_integer &n2, bool
    ignoresign=false);
size_t log2(const nothrow_integer& n);
nothrow_integer gcd(const nothrow_integer& num1, const nothrow_integer& num2);
nothrow_integer lcm(const nothrow_integer& num1, const nothrow_integer& num2);
//!@}

////////////////////////////////////////////////////////////////////////////////
// Template function definitions

/*!
    Constructs a new nothrow_integer object with the value of the built-in
    signed %integer passed to it.

    \param[in] n A number of type \c T.
    \tparam T The type of \c n.

    You can ignore the \c enable_if parameters, they are there only to tell
    the compiler what types it applies to.

    \overload
*/
template <typename T> nothrow_integer::nothrow_integer(const T& n,
    typename boost::enable_if<boost::is_integral<T> >::type*,
    typename boost::enable_if<boost::is_signed<T> >::type*)
{
    try {
        _set_signed(n);
    } catch (std::exception&) {
        _base_attach(0);
    }
}

/*!
    Constructs a new nothrow_integer object with the value of the built-in
    unsigned %integer passed to it.

    \param[in] n A number of type \c T.
    \tparam T The type of \c n.

    You can ignore the \c enable_if parameters, they are there only to tell
    the compiler what types it applies to.

    \overload
*/
template <typename T> nothrow_integer::nothrow_integer(const T& n,
    typename boost::enable_if<boost::is_integral<T> >::type*,
    typename boost::enable_if<boost::is_unsigned<T> >::type*)
{
    try {
        _set_unsigned(n);
    } catch (std::exception&) {
        _base_attach(0);
    }
}

/*! \copydoc xint::to

\note Returns T(0) instead of throwing.
*/
template <typename T>
T to(const nothrow_integer& n) {
    try {
        return detail::to<T>(n);
    } catch (std::exception&) {
        return 0;
    }
}

//! \name Stream input/output functions
//!@{
template <typename charT, typename traits>
inline std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,
    traits>& out, const nothrow_integer& n)
{
    if (n.is_nan()) {
        out << detail::nan_text;
        return out;
    } else {
        return detail::operator<<(out, n);
    }
}

template <typename charT, typename traits>
inline std::basic_istream<charT,traits>& operator>>(std::basic_istream<charT,
    traits>& in, nothrow_integer& n)
{
    if (in.peek()=='#') {
        // Must be either #NaN# or an error
        char buffer[6];

        // These are efficient and safe, but MSVC complains about them anyway.
        //std::streamsize size=in.readsome(buffer, 5);
        //buffer[size]=0;

        // Replacing them with these.
        char *p = buffer, *pe = p + 5;
        while (p != pe) in >> *p++;
        *p = 0;

        std::string str(buffer);

        if (str==detail::nan_text) n=nothrow_integer::nan();
        else in.setstate(std::ios::failbit);
        return in;
    } else {
        return detail::operator>>(in, n);
    }
}
//!@}

//! \copydoc xint::random_by_size
template <class T>
nothrow_integer nothrow_random_by_size(T& gen, size_t bits, bool high_bit_on,
    bool low_bit_on, bool can_be_negative)
{
    try {
        return nothrow_integer(xint::random_by_size(gen, bits, high_bit_on,
            low_bit_on, can_be_negative));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::random_prime
template <class T>
nothrow_integer nothrow_random_prime(T& gen, size_t size_in_bits, callback_t
    callback)
{
    try {
        nothrow_integer r(xint::random_prime(gen, size_in_bits, callback));
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

inline void swap(nothrow_integer& left, nothrow_integer& right) {
    left._swap(right);
}

} // namespace xint
} // namespace boost

namespace std {
template<>
class numeric_limits<boost::xint::nothrow_integer> {
    public:
    static const bool is_specialized = true;

    static boost::xint::nothrow_integer min() throw() { return 0; } // Not applicable
    static boost::xint::nothrow_integer max() throw() { return 0; } // Not applicable

    static const int digits = 0; // Not applicable
    static const int digits10 = 0; // Not applicable
    static const bool is_signed = true;
    static const bool is_integer = true;
    static const bool is_exact = true;
    static const int radix = 2;
    static boost::xint::nothrow_integer epsilon() throw() { return 0; }
    static boost::xint::nothrow_integer round_error() throw() { return 0; }

    static const int min_exponent = 0; // Not applicable
    static const int min_exponent10 = 0; // Not applicable
    static const int max_exponent = 0; // Not applicable
    static const int max_exponent10 = 0; // Not applicable

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = true;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent; // Not applicable
    static const bool has_denorm_loss = false; // Not applicable

    static boost::xint::nothrow_integer infinity() throw() { return 0; } // Not applicable
    static boost::xint::nothrow_integer quiet_NaN() throw()
        { return boost::xint::nothrow_integer::nan(); }
    static boost::xint::nothrow_integer signaling_NaN() throw() { return 0; } // Not applicable
    static boost::xint::nothrow_integer denorm_min() throw() { return 0; } // Not applicable

    static const bool is_iec559 = false;
    static const bool is_bounded = false;
    static const bool is_modulo = false;

    static const bool traps = false; // I assume this means exceptions?
    static const bool tinyness_before = false; // Not applicable
    static const float_round_style round_style = round_indeterminate; // Not applicable
};
} // namespace std

#endif // BOOST_INCLUDED_XINT_NOTHROW_INTEGER_HPP
