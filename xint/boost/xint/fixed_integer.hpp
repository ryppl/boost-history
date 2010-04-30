
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
    \brief Declares the fixed-size integer type.
*/

#ifndef BOOST_INCLUDED_XINT_FIXED_INTEGER_HPP
#define BOOST_INCLUDED_XINT_FIXED_INTEGER_HPP

#include <boost/shared_ptr.hpp>
#include "internals.hpp"
#include "exceptions.hpp"

namespace boost {
namespace xint {

/*! \brief A fixed-size integer type.

\tparam Bits The number of bits that the magnitude of the %fixed_integer type
should be limited to. This can be any number greater than zero.

%fixed_integer types are more processor-efficient than variable-sized %integers,
because they seldom need to do any allocation once they're created. However,
they can be less memory-efficient for holding smaller values. They also silently
discard the upper bits on any value that is too big for them, which can bite the
unwary.

\note The number of bits refers only to the unsigned magnitude of the number.
The sign is stored separately. This means that, for example, a \c
fixed_integer<8> has a range of -255 to +255, not -128 to +127 like an \c
int8_t.

\par
Also note that when calculating with %fixed_integer types, the number does not
"wrap around" when it exceeds the type's size like the built-in signed %integer
types do. It is truncated to the lower bits instead. For example,
<code>fixed_integer<8>(-255) - 1</code> and <code>fixed_integer<8>(255) +
1</code> will both be zero. However, the sign (on non-zero answers) is
preserved, so <code>fixed_integer<8>(-255) - 2</code> will be -1.
*/
template <size_t Bits>
class fixed_integer: public fixed_integer_any {
    public:
    //! \name Constructors & Destructors
    //!@{
    fixed_integer();
    fixed_integer(const fixed_integer<Bits>& b);
    explicit fixed_integer(const base_integer& b);
    explicit fixed_integer(const std::string& str, size_t base=10);
    template <typename T> fixed_integer(const T& n,
        typename boost::enable_if<boost::is_integral<T> >::type* = 0,
        typename boost::enable_if<boost::is_signed<T> >::type* = 0);
    template <typename T> fixed_integer(const T& n,
        typename boost::enable_if<boost::is_integral<T> >::type* = 0,
        typename boost::enable_if<boost::is_unsigned<T> >::type* = 0);
    //!@}

    //! \name Miscellaneous Functions
    //!@{
    bool odd() const;
    bool even() const;
    int  sign(bool signed_zero=false) const;
    size_t hex_digits() const;
    //!@}

    /*! \name Mathematical and Bitwise Operators
        \brief These operate exactly like their counterparts for the built-in
               %integer types.
    */
    //!@{
    fixed_integer<Bits>& operator=(const fixed_integer<Bits>& c) { _attach(c);
		return *this; }

    bool operator!() const { return _is_zero(); }
    fixed_integer<Bits> operator-() const { return negate(*this); }
    fixed_integer<Bits>& operator+() { return *this; }
    const fixed_integer<Bits>& operator+() const { return *this; }
    fixed_integer<Bits> operator~() const;

    fixed_integer<Bits>& operator+=(const fixed_integer_any& b);
    fixed_integer<Bits>& operator-=(const fixed_integer_any& b);
    fixed_integer<Bits>& operator*=(const fixed_integer_any& b);
    fixed_integer<Bits>& operator/=(const fixed_integer_any& b);
    fixed_integer<Bits>& operator%=(const fixed_integer_any& b);

    fixed_integer<Bits>& operator++(); //!< \details Preincrement operator
    fixed_integer<Bits>& operator--(); //!< \details Predecrement operator
    fixed_integer<Bits>  operator++(int); //!< \details Postincrement operator -- not recommended, inefficient
    fixed_integer<Bits>  operator--(int); //!< \details Postdecrement operator -- not recommended, inefficient

    fixed_integer<Bits>& operator&=(const fixed_integer_any& n);
    fixed_integer<Bits>& operator|=(const fixed_integer_any& n);
    fixed_integer<Bits>& operator^=(const fixed_integer_any& n);
    fixed_integer<Bits>  operator<<(size_t shift) const;
    fixed_integer<Bits>  operator>>(size_t shift) const;
    fixed_integer<Bits>& operator<<=(size_t shift);
    fixed_integer<Bits>& operator>>=(size_t shift);
    //!@}

    typedef base_divide_t<fixed_integer<Bits> > divide_t;

    private:
    static const size_t _c_digits;
    static const detail::digit_t _c_mask;
    static boost::shared_ptr<detail::data_t> preallocated_zero;
};

template <size_t Bits> const size_t fixed_integer<Bits>::_c_digits =
    (Bits + detail::bits_per_digit - 1) / detail::bits_per_digit;
template <size_t Bits> const detail::digit_t fixed_integer<Bits>::_c_mask =
    ((Bits % detail::bits_per_digit) == 0) ? detail::digit_mask
    : ((detail::digit_t(1) << (Bits % detail::bits_per_digit)) - 1);
template <size_t Bits> boost::shared_ptr<detail::data_t>
    fixed_integer<Bits>::preallocated_zero(detail::allocate_fixed_zero(
    fixed_integer<Bits>::_c_mask, fixed_integer<Bits>::_c_digits),
    detail::deallocate);

//! \name Mathematical primitives
//!@{
template <size_t Bits> fixed_integer<Bits> abs(const fixed_integer<Bits>& n);
template <size_t Bits> fixed_integer<Bits> negate(const fixed_integer<Bits>& n);
template <size_t Bits> fixed_integer<Bits> add(const fixed_integer<Bits>& n,
    const fixed_integer<Bits>& addend);
template <size_t Bits> fixed_integer<Bits> subtract(const fixed_integer<Bits>&
    n, const fixed_integer<Bits>& subtrahend);
template <size_t Bits> fixed_integer<Bits> multiply(const fixed_integer<Bits>&
    n, const fixed_integer<Bits>& multiplicand);
template <size_t Bits> fixed_integer<Bits> divide(const fixed_integer<Bits>&
    dividend, const fixed_integer<Bits>& divisor);
template <size_t Bits> typename fixed_integer<Bits>::divide_t divide_r(const
    fixed_integer<Bits>& dividend, const fixed_integer<Bits>& divisor);
//!@}

//! \name Powers and roots
//!@{
template <size_t Bits>
fixed_integer<Bits> sqr(const fixed_integer<Bits>& n);
template <size_t Bits>
fixed_integer<Bits> pow(const fixed_integer<Bits>& n, const
    fixed_integer_any& exponent);
template <size_t Bits>
fixed_integer<Bits> fixed_pow2(size_t exponent);
template <size_t Bits>
fixed_integer<Bits> fixed_factorial(size_t n);
template <size_t Bits>
fixed_integer<Bits> sqrt(const fixed_integer<Bits>& n);
//!@}

//! \name Conversion functions
//!@{
template <size_t Bits, typename T> T to(const fixed_integer<Bits>& n);
std::string to_string(const fixed_integer_any& n, size_t base=10, bool
    uppercase=false);
template <size_t Bits> fixed_integer<Bits> fixed_from_string(const
    std::string& str, size_t base=10);
template <size_t Bits> xint::binary_t to_binary(const fixed_integer_any& n,
    size_t bits=0);
template <size_t Bits> fixed_integer<Bits> fixed_from_binary(const
    xint::binary_t& s, size_t bits=0);
//!@}

//! \name Bit-manipulation functions
//!@{
template <size_t Bits>
bool getbit(const fixed_integer<Bits>& n, size_t bit);
template <size_t Bits>
void setbit(fixed_integer<Bits>& n, size_t bit);
template <size_t Bits>
void clearbit(fixed_integer<Bits>& n, size_t bit);
template <size_t Bits>
size_t lowestbit(const fixed_integer<Bits>& n, size_t valueIfZero=0);
template <size_t Bits>
size_t highestbit(const fixed_integer<Bits>& n, size_t valueIfZero=0);
template <size_t Bits>
fixed_integer<Bits> bitwise_and(const fixed_integer<Bits>& n1, const
    fixed_integer<Bits>& n2);
template <size_t Bits>
fixed_integer<Bits> bitwise_or (const fixed_integer<Bits>& n1, const
    fixed_integer<Bits>& n2);
template <size_t Bits>
fixed_integer<Bits> bitwise_xor(const fixed_integer<Bits>& n1, const
    fixed_integer<Bits>& n2);
template <size_t Bits>
fixed_integer<Bits> shift(const fixed_integer<Bits>& n, int byBits);
template <size_t Bits>
fixed_integer<Bits> shift_left(const fixed_integer<Bits>& n, size_t byBits);
template <size_t Bits>
fixed_integer<Bits> shift_right(const fixed_integer<Bits>& n, size_t byBits);
//!@}

/*! \name Modular math functions

A common reason to use a large-integer library is to implement public-key
encryption, and the algorithms for such encryption often use modular math.
*/
//!@{
template <size_t Bits>
fixed_integer<Bits> mod(const fixed_integer<Bits>& n, const fixed_integer<Bits>&
    modBy);
template <size_t Bits>
fixed_integer<Bits> mulmod(const fixed_integer<Bits>& n, const
    fixed_integer<Bits>& by, const fixed_integer<Bits>& modulus);
template <size_t Bits>
fixed_integer<Bits> sqrmod(const fixed_integer<Bits>& n, const
    fixed_integer<Bits>& modulus);
template <size_t Bits>
fixed_integer<Bits> powmod(const fixed_integer<Bits>& n, const
    fixed_integer<Bits>& exponent, const fixed_integer<Bits>& modulus,
    bool noMontgomery=false);
template <size_t Bits>
fixed_integer<Bits> invmod(const fixed_integer<Bits>& n, const
    fixed_integer<Bits>& modulus);
//!@}

//! \name Random number functions
//!@{
template <size_t Bits>
fixed_integer<Bits> fixed_random_by_size(size_t sizeInBits, bool highBitOn =
    false, bool lowBitOn = false, bool canBeNegative = false);
//!@}

//! \name Prime number functions
//!@{
int is_prime(const fixed_integer_any& n, callback_t callback = no_callback);
template <size_t Bits>
fixed_integer<Bits> fixed_random_prime(size_t sizeInBits, callback_t callback =
    no_callback);
//!@}

/*! \name Operators

    These act exactly the same as for the built-in %integer types.
*/
//!@{
#define BOOST_XINT_NFIXED typename boost::disable_if<boost::is_base_of<\
    fixed_integer_any, T>, fixed_integer<Bits> >::type

#define BOOST_XINT_FIXED_BINARY_FN(name, body) \
template <size_t Bits> \
fixed_integer<Bits> name(const fixed_integer<Bits>& op1, \
    const fixed_integer<Bits>& op2) \
{ \
    fixed_integer<Bits> r; \
    body(r, op1, op2); \
    return BOOST_XINT_MOVE(r); \
} \
\
template <size_t Bits, typename T> \
BOOST_XINT_NFIXED name(const fixed_integer<Bits>& n1, const T& n2) { \
    fixed_integer<Bits> r; \
    body(r, n1, fixed_integer_any(n2)); \
    return BOOST_XINT_MOVE(r); \
} \
\
template <typename T, size_t Bits> \
BOOST_XINT_NFIXED name(const T& n1, const fixed_integer<Bits>& n2) { \
    fixed_integer<Bits> r; \
    body(r, fixed_integer_any(n1), n2); \
    return BOOST_XINT_MOVE(r); \
}

BOOST_XINT_FIXED_BINARY_FN(operator+, detail::add)
BOOST_XINT_FIXED_BINARY_FN(operator-, detail::subtract)
BOOST_XINT_FIXED_BINARY_FN(operator*, detail::multiply)
BOOST_XINT_FIXED_BINARY_FN(operator/, detail::divide)
BOOST_XINT_FIXED_BINARY_FN(operator%, detail::mod)
BOOST_XINT_FIXED_BINARY_FN(operator&, detail::bitwise_and)
BOOST_XINT_FIXED_BINARY_FN(operator|, detail::bitwise_or)
BOOST_XINT_FIXED_BINARY_FN(operator^, detail::bitwise_xor)

bool operator==(const fixed_integer_any &num1, const fixed_integer_any &num2);
bool operator!=(const fixed_integer_any& num1, const fixed_integer_any& num2);
bool operator<(const fixed_integer_any& num1, const fixed_integer_any& num2);
bool operator>(const fixed_integer_any& num1, const fixed_integer_any& num2);
bool operator<=(const fixed_integer_any& num1, const fixed_integer_any& num2);
bool operator>=(const fixed_integer_any& num1, const fixed_integer_any& num2);
//!@}

//! \name Miscellaneous functions
//!@{
int compare(const fixed_integer_any& c1, const fixed_integer_any& c2, bool
    ignoresign=false);
size_t log2(const fixed_integer_any& n);
template <size_t Bits>
fixed_integer<Bits> gcd(const fixed_integer<Bits>& num1, const
    fixed_integer<Bits>& num2);
template <size_t Bits>
fixed_integer<Bits> lcm(const fixed_integer<Bits>& num1, const
    fixed_integer<Bits>& num2);
//!@}

////////////////////////////////////////////////////////////////////////////////
// fixed_integer member function definitions

//! \brief Creates a new fixed_integer with an initial value of zero.
template <size_t Bits>
fixed_integer<Bits>::fixed_integer(): fixed_integer_any(preallocated_zero.get())
{ }

/*! \brief Creates a copy of an existing fixed_integer with the same size.

\param[in] b An existing integer of the same size.

\overload
*/
template <size_t Bits>
fixed_integer<Bits>::fixed_integer(const fixed_integer<Bits>& b):
    fixed_integer_any(b) { }

/*! \brief Converts an existing xint type.

\param[in] b An existing integer, nothrow_integer, or fixed_integer of a
different size.

\overload

\note If the number represented by the parameter is too large to fit into the
fixed_integer, it is silently truncated, and only the lower bits are preserved.
*/
template <size_t Bits>
fixed_integer<Bits>::fixed_integer(const base_integer& b):
    fixed_integer_any(preallocated_zero.get())
{
    if (b._is_nan()) throw exceptions::not_a_number();
    _attach(b);
}

/*! \copydoc integer::integer(const std::string&, size_t)

\note If the number represented by the parameter is too large to fit into the
fixed_integer, it is silently truncated, and only the lower bits are preserved.
*/
template <size_t Bits>
fixed_integer<Bits>::fixed_integer(const std::string& str, size_t base):
    fixed_integer_any(_c_digits, _c_mask)
{
    detail::from_string(*this, str, base);
}

/*!
Constructs a new fixed_integer object with the value of the built-in signed
%integer passed to it.

\param[in] n A number of type \c T.
\tparam T The type of \c n.

You can ignore the \c enable_if parameters, they are there only to tell
the compiler what types it applies to.

\overload

\note If the number represented by the parameter is too large to fit into the
fixed_integer, it is silently truncated, and only the lower bits are preserved.
*/
template <size_t Bits>
template <typename T>
fixed_integer<Bits>::fixed_integer(const T& n,
    typename boost::enable_if<boost::is_integral<T> >::type*,
    typename boost::enable_if<boost::is_signed<T> >::type*):
        fixed_integer_any(_c_digits, _c_mask)
{
    _set_signed(n);
}

/*!
Constructs a new integer object with the value of the built-in unsigned
%integer passed to it.

\param[in] n A number of type \c T.
\tparam T The type of \c n.

You can ignore the \c enable_if parameters, they are there only to tell
the compiler what types it applies to.

\overload

\note If the number represented by the parameter is too large to fit into the
fixed_integer, it is silently truncated, and only the lower bits are preserved.
*/
template <size_t Bits>
template <typename T>
fixed_integer<Bits>::fixed_integer(const T& n,
    typename boost::enable_if<boost::is_integral<T> >::type*,
    typename boost::enable_if<boost::is_unsigned<T> >::type*):
        fixed_integer_any(_c_digits, _c_mask)
{
    _set_unsigned(n);
}

//! \copydoc integer::odd
template <size_t Bits>
bool fixed_integer<Bits>::odd() const {
    return (_get_digit(0) & 0x01) != 0;
}

//! \copydoc integer::even
template <size_t Bits>
bool fixed_integer<Bits>::even() const {
    return (_get_digit(0) & 0x01) == 0;
}

//! \copydoc integer::sign
template <size_t Bits>
int fixed_integer<Bits>::sign(bool signed_zero) const {
    if (!signed_zero && _is_zero()) return 0;
    else return (_get_negative() ? -1 : 1);
}

//! \copydoc integer::hex_digits
template <size_t Bits>
size_t fixed_integer<Bits>::hex_digits() const {
    size_t bits=_log2();
    return (bits+3)/4; // Four bits per hex digit, rounded up
}

/*! \brief The Bitwise Ones' Complement (i.e. bitwise NOT) operator.

This operator does not exist in the integer and nothrow_integer classes, because
as their lengths are not limited, it would result in an infinitely long number.
*/
template <size_t Bits>
fixed_integer<Bits> fixed_integer<Bits>::operator~() const {
    fixed_integer<Bits> r(*this);
    r._make_unique();
    detail::digit_t *p = r._get_writable_digits(), *pe1 = p + r._get_length(),
        *pe2 = p + _c_digits;
    while (p != pe1) { *p = ~(*p); ++p; }
    while (p != pe2) *p++ = detail::digit_mask;
    r._set_length(_c_digits);
    r._cleanup();
    return BOOST_XINT_MOVE(r);
}

#define BOOST_XINT_FIXED_OPEQ_FN(name, body) \
template <size_t Bits> \
fixed_integer<Bits>& fixed_integer<Bits>::name(const fixed_integer_any& n) { \
    body(*this, *this, n); \
    return *this; \
}

BOOST_XINT_FIXED_OPEQ_FN(operator+=, detail::add)
BOOST_XINT_FIXED_OPEQ_FN(operator-=, detail::subtract)
BOOST_XINT_FIXED_OPEQ_FN(operator*=, detail::multiply)
BOOST_XINT_FIXED_OPEQ_FN(operator/=, detail::divide)
BOOST_XINT_FIXED_OPEQ_FN(operator%=, detail::mod)

template <size_t Bits>
fixed_integer<Bits>& fixed_integer<Bits>::operator<<=(size_t shift) {
    detail::shift_left(*this, *this, shift);
    return *this;
}

template <size_t Bits>
fixed_integer<Bits>& fixed_integer<Bits>::operator>>=(size_t shift) {
    detail::shift_right(*this, *this, shift);
    return *this;
}

template <size_t Bits>
fixed_integer<Bits> fixed_integer<Bits>::operator<<(size_t shift) const {
    fixed_integer<Bits> r;
    detail::shift_left(r, *this, shift);
    return BOOST_XINT_MOVE(r);
}

template <size_t Bits>
fixed_integer<Bits> fixed_integer<Bits>::operator>>(size_t shift) const {
    fixed_integer<Bits> r;
    detail::shift_right(r, *this, shift);
    return BOOST_XINT_MOVE(r);
}

template <size_t Bits>
fixed_integer<Bits>& fixed_integer<Bits>::operator++() {
    _increment();
    return *this;
}

template <size_t Bits>
fixed_integer<Bits>& fixed_integer<Bits>::operator--() {
    _decrement();
    return *this;
}

template <size_t Bits>
fixed_integer<Bits>  fixed_integer<Bits>::operator++(int) {
    fixed_integer<Bits> old(*this);
    _increment();
    return BOOST_XINT_MOVE(old);
}

template <size_t Bits>
fixed_integer<Bits>  fixed_integer<Bits>::operator--(int) {
    fixed_integer<Bits> old(*this);
    _decrement();
    return BOOST_XINT_MOVE(old);
}

////////////////////////////////////////////////////////////////////////////////
// Non-member fixed_integer function definitions

//! \copydoc abs(const integer&)
template <size_t Bits>
fixed_integer<Bits> abs(const fixed_integer<Bits>& n) {
    if (n._get_negative()) {
        fixed_integer<Bits> r(n);
        r._set_negative(false);
        return BOOST_XINT_MOVE(r);
    } else return n;
}

//! \copydoc negate(const integer&)
template <size_t Bits>
fixed_integer<Bits> negate(const fixed_integer<Bits>& n) {
    fixed_integer<Bits> r(n);
    r._set_negative(!r._get_negative());
    return BOOST_XINT_MOVE(r);
}

//! \copydoc add(const integer&, const integer&)
template <size_t Bits>
fixed_integer<Bits> add(const fixed_integer<Bits>& n1, const
    fixed_integer<Bits>& n2)
{
    fixed_integer<Bits> r;
    detail::add(r, n1, n2);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc subtract(const integer&, const integer&)
template <size_t Bits>
fixed_integer<Bits> subtract(const fixed_integer<Bits>& n1, const
    fixed_integer<Bits>& n2)
{
    fixed_integer<Bits> r;
    detail::subtract(r, n1, n2);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc multiply(const integer&, const integer&)
template <size_t Bits>
fixed_integer<Bits> multiply(const fixed_integer<Bits>& n, const
    fixed_integer<Bits>& by)
{
    fixed_integer<Bits> r;
    detail::multiply(r, n, by);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc divide(const integer&, const integer&)
template <size_t Bits>
fixed_integer<Bits> divide(const fixed_integer<Bits>& dividend,
    const fixed_integer<Bits>& divisor)
{
    fixed_integer<Bits> q, r;
    detail::divide(q, r, dividend, divisor);
    return BOOST_XINT_MOVE(q);
}

//! \copydoc divide(const integer&, const integer&)
template <size_t Bits>
typename fixed_integer<Bits>::divide_t divide_r(const fixed_integer<Bits>&
    dividend, const fixed_integer<Bits>& divisor)
{
    fixed_integer<Bits> q, r;
    detail::divide(q, r, dividend, divisor);
    return BOOST_XINT_FORWARD(fixed_integer<Bits>::divide_t,
        fixed_integer<Bits>::divide_t(BOOST_XINT_MOVE(q), BOOST_XINT_MOVE(r)));
}

//! \copydoc sqr(const integer&)
template <size_t Bits>
fixed_integer<Bits> sqr(const fixed_integer<Bits>& n) {
    fixed_integer<Bits> r;
    detail::sqr(r, n);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc pow(const integer&, const integer&)
template <size_t Bits>
fixed_integer<Bits> pow(const fixed_integer<Bits>& n, const
    fixed_integer_any& e)
{
    fixed_integer<Bits> r;
    detail::pow(r, n, e);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc pow2(size_t)
template <size_t Bits>
fixed_integer<Bits> fixed_pow2(size_t e) {
    fixed_integer<Bits> r;
    setbit(r, e);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc factorial(size_t)
template <size_t Bits>
fixed_integer<Bits> fixed_factorial(size_t n) {
    fixed_integer<Bits> r;
    detail::factorial(r, n);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc sqrt(const integer&)
template <size_t Bits>
fixed_integer<Bits> sqrt(const fixed_integer<Bits>& n) {
    fixed_integer<Bits> r;
    detail::sqrt(r, n);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc to(const integer&)
template <size_t Bits, typename T>
T to(const fixed_integer<Bits>& n) {
    return detail::to<T>(n);
}

//! \copydoc from_string
template <size_t Bits>
fixed_integer<Bits> fixed_from_string(const std::string& str, size_t base) {
    fixed_integer<Bits> r;
    detail::from_string(r, str, base);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc to_binary(const integer&, size_t bits)
template <size_t Bits>
xint::binary_t to_binary(const fixed_integer_any& n, size_t bits) {
    xint::binary_t r;
    detail::to_binary(r, n, bits);
    return r;
}

//! \copydoc from_binary
template <size_t Bits>
fixed_integer<Bits> fixed_from_binary(const xint::binary_t& b, size_t bits) {
    fixed_integer<Bits> r;
    detail::from_binary(r, b, bits);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc getbit(const integer&, size_t)
template <size_t Bits>
bool getbit(const fixed_integer<Bits>& n, size_t bit) {
    return detail::getbit(n, bit);
}

//! \copydoc setbit(integer&, size_t)
template <size_t Bits>
void setbit(fixed_integer<Bits>& n, size_t bit) {
    return detail::setbit(n, bit);
}

//! \copydoc clearbit(integer&, size_t)
template <size_t Bits>
void clearbit(fixed_integer<Bits>& n, size_t bit) {
    return detail::clearbit(n, bit);
}

//! \copydoc lowestbit(const integer&, size_t)
template <size_t Bits>
size_t lowestbit(const fixed_integer<Bits>& n, size_t valueIfZero) {
    return detail::lowestbit(n, valueIfZero);
}

//! \copydoc highestbit(const integer&, size_t)
template <size_t Bits>
size_t highestbit(const fixed_integer<Bits>& n, size_t valueIfZero) {
    return detail::highestbit(n, valueIfZero);
}

//! \copydoc bitwise_and(const integer&, const integer&)
template <size_t Bits>
fixed_integer<Bits> bitwise_and(const fixed_integer<Bits>& n1, const
    fixed_integer<Bits>& n2)
{
    fixed_integer<Bits> r;
    detail::bitwise_and(r, n1, n2);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc bitwise_or(const integer&, const integer&)
template <size_t Bits>
fixed_integer<Bits> bitwise_or (const fixed_integer<Bits>& n1, const
    fixed_integer<Bits>& n2)
{
    fixed_integer<Bits> r;
    detail::bitwise_or(r, n1, n2);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc bitwise_xor(const integer&, const integer&)
template <size_t Bits>
fixed_integer<Bits> bitwise_xor(const fixed_integer<Bits>& n1, const
    fixed_integer<Bits>& n2)
{
    fixed_integer<Bits> r;
    detail::bitwise_xor(r, n1, n2);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc shift(const integer&, int)
template <size_t Bits>
fixed_integer<Bits> shift(const fixed_integer<Bits>& n, int byBits) {
    return (byBits < 0 ? shift_right(n, -byBits) : shift_left(n, byBits));
}

//! \copydoc shift_left(const integer&, size_t)
template <size_t Bits>
fixed_integer<Bits> shift_left(const fixed_integer<Bits>& n, size_t byBits) {
    fixed_integer<Bits> r;
    detail::shift_left(r, n, byBits);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc shift_right(const integer&, size_t)
template <size_t Bits>
fixed_integer<Bits> shift_right(const fixed_integer<Bits>& n, size_t byBits) {
    fixed_integer<Bits> r;
    detail::shift_right(r, n, byBits);
    return BOOST_XINT_MOVE(r);
}

template <size_t Bits>
fixed_integer<Bits> mod(const fixed_integer<Bits>& n, const fixed_integer<Bits>&
    m)
{
    fixed_integer<Bits> r;
    detail::mod(r, n, m);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc mulmod(const integer&, const integer&, const integer&)
template <size_t Bits>
fixed_integer<Bits> mulmod(const fixed_integer<Bits>& n, const
    fixed_integer<Bits>& by, const fixed_integer<Bits>& m)
{
    return mod(n * by, m);
}

//! \copydoc sqrmod(const integer&, const integer&)
template <size_t Bits>
fixed_integer<Bits> sqrmod(const fixed_integer<Bits>& n, const
    fixed_integer<Bits>& m)
{
    return mod(sqr(n), m);
}

//! \copydoc powmod(const integer&, const integer&, const integer&, bool)
template <size_t Bits>
fixed_integer<Bits> powmod(const fixed_integer<Bits>& n, const
    fixed_integer<Bits>& e, const fixed_integer<Bits>& m, bool noMontgomery)
{
    fixed_integer<Bits> r;
    detail::powmod(r, n, e, m, noMontgomery);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc invmod(const integer&, const integer&)
template <size_t Bits>
fixed_integer<Bits> invmod(const fixed_integer<Bits>& n, const
    fixed_integer<Bits>& m)
{
    fixed_integer<Bits> r;
    detail::invmod(r, n, m);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc random_by_size
template <size_t Bits>
fixed_integer<Bits> fixed_random_by_size(size_t bits, bool high_bit_on, bool
    low_bit_on, bool can_be_negative)
{
    return fixed_integer<Bits>(random_by_size((std::min)(Bits, bits),
        high_bit_on, low_bit_on, can_be_negative));
}

//! \copydoc random_prime
template <size_t Bits>
fixed_integer<Bits> fixed_random_prime(size_t size_in_bits, callback_t callback)
{
    fixed_integer<Bits> r(random_prime((std::min)(Bits, size_in_bits), callback));
    return BOOST_XINT_MOVE(r);
}

//! \copydoc gcd(const integer&, const integer&)
template <size_t Bits>
fixed_integer<Bits> gcd(const fixed_integer<Bits>& num1, const
    fixed_integer<Bits>& num2)
{
    fixed_integer<Bits> r;
    detail::gcd(r, num1, num2);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc lcm(const integer&, const integer&)
template <size_t Bits>
fixed_integer<Bits> lcm(const fixed_integer<Bits>& num1, const
    fixed_integer<Bits>& num2)
{
    fixed_integer<Bits> r;
    detail::lcm(r, num1, num2);
    return BOOST_XINT_MOVE(r);
}

//! \name Stream input/output functions
//!@{
template <typename charT, typename traits, size_t Bits>
inline std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,
    traits>& out, const fixed_integer<Bits>& n)
{
    return detail::operator<<(out, n);
}

template <typename charT, typename traits, size_t Bits>
inline std::basic_istream<charT,traits>& operator>>(std::basic_istream<charT,
    traits>& in, fixed_integer<Bits>& n)
{
    return detail::operator>>(in, n);
}
//!@}

// No 100% compatible way to declare a specialization of a function template
// using a templated class, as far as I can tell. This is the second-best
// solution.
template<size_t Bits>
void swap(fixed_integer<Bits>& left, fixed_integer<Bits>& right) {
    left._swap(right);
}

} // namespace xint
} // namespace boost

namespace std {

template<size_t Bits>
class numeric_limits<boost::xint::fixed_integer<Bits> > {
    public:
    static const bool is_specialized = true;

    static boost::xint::fixed_integer<Bits> min() throw() { return
        -~(boost::xint::fixed_integer<Bits>()); }
    static boost::xint::fixed_integer<Bits> max() throw() { return
        ~(boost::xint::fixed_integer<Bits>()); }

    static const int digits = Bits;
    static const int digits10; // = boost::xint::fixed_integer<Bits>::_c_log10;
    static const bool is_signed = true;
    static const bool is_integer = true;
    static const bool is_exact = true;
    static const int radix = 2;
    static boost::xint::fixed_integer<Bits> epsilon() throw() { return 0; }
    static boost::xint::fixed_integer<Bits> round_error() throw() { return 0; }

    static const int min_exponent = 0; // Not applicable
    static const int min_exponent10 = 0; // Not applicable
    static const int max_exponent = 0; // Not applicable
    static const int max_exponent10 = 0; // Not applicable

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent; // Not applicable
    static const bool has_denorm_loss = false; // Not applicable

    static boost::xint::fixed_integer<Bits> infinity() throw() { return 0; } // Not applicable
    static boost::xint::fixed_integer<Bits> quiet_NaN() throw() { return 0; } // Not applicable
    static boost::xint::fixed_integer<Bits> signaling_NaN() throw() { return 0; } // Not applicable
    static boost::xint::fixed_integer<Bits> denorm_min() throw() { return 0; } // Not applicable

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = true;

    static const bool traps = true; // I assume this means exceptions?
    static const bool tinyness_before = false; // Not applicable
    static const float_round_style round_style = round_indeterminate; // Not applicable
};

template<size_t Bits>
const int numeric_limits<boost::xint::fixed_integer<Bits> >::digits10 =
    static_cast<int>(boost::xint::detail::log10_bits(Bits));

} // namespace std

#endif // BOOST_INCLUDED_XINT_FIXED_INTEGER_HPP
