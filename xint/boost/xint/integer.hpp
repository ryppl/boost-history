
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
    \brief Declares the core integer type.
*/

#ifndef BOOST_INCLUDED_XINT_INTEGER_HPP
#define BOOST_INCLUDED_XINT_INTEGER_HPP

#include "internals.hpp"
#include "exceptions.hpp"
#include <boost/type_traits.hpp>

namespace boost {
namespace xint {

/*! \brief The core %integer class.

    This class implements the standard \c integer type.

    There are only a few member functions; most of the functionality is
    implemented using standalone functions.

    Functions that start with an underscore (such as \c _make_unique) are
    intended for internal use only. They may change arbitrarily in future
    versions.
*/
class integer: public detail::base_integer {
    public:

    //! \name Constructors & Destructors
    //!@{
    integer();
    integer(const integer& b);
    explicit integer(const nothrow_integer& b);
    explicit integer(const std::string& str, size_t base=10);
    template <typename T> integer(const T& n,
        typename boost::enable_if<boost::is_integral<T> >::type* = 0,
        typename boost::enable_if<boost::is_signed<T> >::type* = 0);
    template <typename T> integer(const T& n,
        typename boost::enable_if<boost::is_integral<T> >::type* = 0,
        typename boost::enable_if<boost::is_unsigned<T> >::type* = 0);
    //!@}

    /*! \name Mathematical and Bitwise Operators
        \brief These operate exactly like their counterparts for the built-in
               %integer types.

        Note that there is no \c operator~. Since the integer type doesn't have
        a fixed maximum size, there is no logical way to implement it.
    */
    //@{
    integer& operator=(const integer &c);

    integer& operator+=(const integer& b);
    integer& operator-=(const integer& b);
    integer& operator*=(const integer& b);
    integer& operator/=(const integer& b);
    integer& operator%=(const integer& b);

    integer& operator++(); //!< \details Preincrement operator
    integer& operator--(); //!< \details Predecrement operator
    integer  operator++(int); //!< \details Postincrement operator -- not recommended, inefficient
    integer  operator--(int); //!< \details Postdecrement operator -- not recommended, inefficient

    integer& operator&=(const integer& n);
    integer& operator|=(const integer& n);
    integer& operator^=(const integer& n);
    integer  operator<<(size_t shift) const;
    integer  operator>>(size_t shift) const;
    integer& operator<<=(size_t shift);
    integer& operator>>=(size_t shift);
    //@}

    /*! \name Preconstructed integers

        These return preconstructed integers with the values named by the
        functions. They are used internally, so that the most common values
        don't have to be constructed each time they're needed.
    */
    //@{
    static const integer& zero();
    static const integer& one();
    //@}

    //! \name Miscellaneous Functions
    //!@{
    bool odd() const;
    bool even() const;
    int  sign(bool signed_zero=false) const;

    size_t hex_digits() const;
    //!@}

    /*! \name Internal Functions
        \brief These functions allow access to the internals of the %integer.
               They are intended for internal use only.
    */
    //@{
    void _make_unique();
    //@}
};

//! \name Mathematical primitives
//!@{
integer abs(const integer& n);
integer negate(const integer& n);
integer add(const integer& n, const integer& addend);
integer subtract(const integer& n, const integer& subtrahend);
integer multiply(const integer& n, const integer& multiplicand);
integer divide(const integer& dividend, const integer& divisor);
std::pair<integer, integer> divide_r(const integer& dividend, const integer&
    divisor);
//!@}

//! \name Powers and roots
//!@{
integer sqr(const integer& n);
integer pow(const integer& n, const integer& exponent);
integer pow2(size_t exponent);
integer factorial(size_t n);
integer sqrt(const integer& n);
//!@}

//! \name Conversion functions
//!@{
template <typename T> T to(const integer& n);
std::string to_string(const integer& n, size_t base=10, bool upperCase=false);
integer from_string(const std::string& str, size_t base=10);
std::string to_binary(const integer& n);
integer from_binary(const std::string& s);
//!@}

//! \name Bit-manipulation functions
//!@{
bool getbit(const integer& n, size_t bit);
void setbit(integer& n, size_t bit);
void clearbit(integer& n, size_t bit);
size_t lowestbit(const integer& n, size_t valueIfZero=0);
size_t highestbit(const integer& n, size_t valueIfZero=0);
integer bitwise_and(const integer& n1, const integer& n2);
integer bitwise_or (const integer& n1, const integer& n2);
integer bitwise_xor(const integer& n1, const integer& n2);
integer shift(const integer& n, int byBits);
integer shift_left(const integer& n, size_t byBits);
integer shift_right(const integer& n, size_t byBits);
//!@}

/*! \name Modular math functions

A common reason to use a large-integer library is to implement public-key
encryption, and the algorithms for such encryption often use modular math.
*/
//!@{
integer mod(const integer& n, const integer& modBy);
integer mulmod(const integer& n, const integer& by, const integer& modulus);
integer sqrmod(const integer& n, const integer& modulus);
integer powmod(const integer& n, const integer& exponent, const integer&
    modulus, bool noMontgomery=false);
integer invmod(const integer& n, const integer& modulus);
//!@}

//! \name Random number functions
//!@{
integer random_by_size(size_t sizeInBits, bool highBitOn=false, bool
    lowBitOn=false, bool canBeNegative=false);
//!@}

//! \name Prime number functions
//!@{
int is_prime(const integer& n, callback_t callback=no_callback);
integer random_prime(size_t sizeInBits, callback_t callback=no_callback);
//!@}

/*! \name Operators

    These act exactly the same as for the built-in %integer types.
*/
//!@{
bool operator!(const integer& n1);
bool operator<(const integer& n1, const integer& n2);
bool operator>(const integer& n1, const integer& n2);
bool operator<=(const integer& n1, const integer& n2);
bool operator>=(const integer& n1, const integer& n2);
bool operator==(const integer& n1, const integer& n2);
bool operator!=(const integer& n1, const integer& n2);

const integer& operator+(const integer& a);
integer operator-(const integer& a);
integer operator+(const integer& n1, const integer& n2);
integer operator-(const integer& n1, const integer& n2);
integer operator*(const integer& n1, const integer& n2);
integer operator/(const integer& n1, const integer& n2);
integer operator%(const integer& n1, const integer& n2);
integer operator&(const integer& n1, const integer& n2);
integer operator|(const integer& n1, const integer& n2);
integer operator^(const integer& n1, const integer& n2);
//!@}

//! \name Miscellaneous functions
//!@{
int compare(const integer &n1, const integer &n2, bool ignoresign=false);
size_t log2(const integer& n);
integer gcd(const integer& num1, const integer& num2);
integer lcm(const integer& num1, const integer& num2);
//!@}

////////////////////////////////////////////////////////////////////////////////
// Template function definitions

/*!
    Constructs a new integer object with the value of the built-in signed
    %integer passed to it.

    \param[in] n A number of type \c T.
    \tparam T The type of \c n.

    You can ignore the \c enable_if parameters, they are there only to tell
    the compiler what types it applies to.

    \overload
*/
template <typename T> integer::integer(const T& n,
    typename boost::enable_if<boost::is_integral<T> >::type*,
    typename boost::enable_if<boost::is_signed<T> >::type*)
{
    if (n >= 0) {
        if (static_cast<T>(n & detail::digit_mask) == n)
            _attach_1(detail::digit_t(n));
        else _attach_n(boost::uintmax_t(n));
    } else if (n == (std::numeric_limits<T>::min)()) {
        // Have to treat the minimum number carefully, because -n is not
        // what you'd think it is.
        _attach_n(boost::uintmax_t(-(n+1)));
        _set_negative(true);
        --(*this);
    } else {
        _attach_n(boost::uintmax_t(-n));
        _set_negative(true);
    }
}

/*!
    Constructs a new integer object with the value of the built-in unsigned
    %integer passed to it.

    \param[in] n A number of type \c T.
    \tparam T The type of \c n.

    You can ignore the \c enable_if parameters, they are there only to tell
    the compiler what types it applies to.

    \overload
*/
template <typename T> integer::integer(const T& n,
    typename boost::enable_if<boost::is_integral<T> >::type*,
    typename boost::enable_if<boost::is_unsigned<T> >::type*)
{
    if (static_cast<T>(n & detail::digit_mask) == n)
        _attach_1(detail::digit_t(n));
    else _attach_n(boost::uintmax_t(n));
}

/*! \brief Efficiently converts from an xint::integer to a built-in %integer
           type.

\param[in] n The integer to convert.
\tparam T The type to convert it to.

\returns The numeric value of \c n, converted to the specified type.

\exception exceptions::too_big if \c n would not fit into the specified type.
*/
template <typename T>
T to(const integer& n) {
    if (n < (std::numeric_limits<T>::min)()
        || n > (std::numeric_limits<T>::max)())
            throw exceptions::too_big("value out of range for requested "
                "conversion");

    T rval=0;
    size_t len=n._get_length();
    for (size_t x=0; x<len; ++x)
        rval=static_cast<T>((rval * detail::digit_overflowbit)
            + n._get_digit(len-x-1));
    if (n.sign() < 0) rval *= -1;
    return rval;
}

//! \name Stream input/output functions
//!@{
template <typename charT, typename traits>
inline std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,
    traits>& out, const integer& n)
{
    int base=((out.flags() & std::ios::hex) ? 16
        : (out.flags() & std::ios::oct) ? 8
        : 10);
    bool upperCase=(out.flags() & std::ios::uppercase ? true : false);

    int nsign=n.sign();
    if ((out.flags() & std::ios::showpos) && nsign >= 0) out << "+";

    if (out.flags() & std::ios::showbase) {
        if (nsign < 0) out << "-";

        if (base==16 && upperCase) out << "0X";
        else if (base==16) out << "0x";
        else if (base==8) out << "0";

        out << to_string(abs(n), base, upperCase);
    } else {
        out << to_string(n, base, upperCase);
    }
    return out;
}

template <typename charT, typename traits>
inline std::basic_istream<charT,traits>& operator>>(std::basic_istream<charT,
    traits>& in, integer& n)
{
    int hex=(in.flags() & std::ios::hex) ? 1 : 0;
    int dec=(in.flags() & std::ios::dec) ? 1 : 0;
    int oct=(in.flags() & std::ios::oct) ? 1 : 0;
    int count=hex+dec+oct;

    size_t base=autobase;
    if (count==1) {
        if (hex) base=16;
        else if (oct) base=8;
        else base=10;
    } else if (count>1) base=10;
    // else auto-base

    std::string s;
    if (in.peek()=='+') {
        in.ignore();
    } else if (in.peek()=='-') {
        in.ignore();
        s.push_back('-');
    }

    if (base==autobase) {
        if (in.peek()=='0') {
            in.ignore();
            int c=in.peek();
            if (c=='x' || c=='X') {
                in.ignore();
                base=16;
            } else base=8;
        } else base=10;
    }

    while (in) {
        int c=in.peek();
        if ((base==8 && (c>='0' && c<='7')) ||
            (base==10 && (c>='0' && c<='9')) ||
            (base==16 && ((c>='0' && c<='9') || (c>='a' && c<='f') ||
                (c>='A' && c<='F'))))
        {
            in.ignore();
            s.push_back(char(c));
        } else break;
    }

    try {
        integer testValue=from_string(s, base);
        n=testValue;
    } catch (std::exception&) {
        // Catch invalid strings
        in.setstate(std::ios::failbit);
    }

    return in;
}
//!@}

} // namespace xint
} // namespace boost

namespace std {
template<>
class numeric_limits<boost::xint::integer> {
    public:
    static const bool is_specialized = true;

    static boost::xint::integer min() throw() { return 0; } // Not applicable
    static boost::xint::integer max() throw() { return 0; } // Not applicable

    static const int digits = 0; // Not applicable
    static const int digits10 = 0; // Not applicable
    static const bool is_signed = true;
    static const bool is_integer = true;
    static const bool is_exact = true;
    static const int radix = 2;
    static boost::xint::integer epsilon() throw() { return 0; }
    static boost::xint::integer round_error() throw() { return 0; }

    static const int min_exponent = 0; // Not applicable
    static const int min_exponent10 = 0; // Not applicable
    static const int max_exponent = 0; // Not applicable
    static const int max_exponent10 = 0; // Not applicable

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent; // Not applicable
    static const bool has_denorm_loss = false; // Not applicable

    static boost::xint::integer infinity() throw() { return 0; } // Not applicable
    static boost::xint::integer quiet_NaN() throw() { return 0; } // Not applicable
    static boost::xint::integer signaling_NaN() throw() { return 0; } // Not applicable
    static boost::xint::integer denorm_min() throw() { return 0; } // Not applicable

    static const bool is_iec559 = false;
    static const bool is_bounded = false;
    static const bool is_modulo = false;

    static const bool traps = true; // I assume this means exceptions?
    static const bool tinyness_before = false; // Not applicable
    static const float_round_style round_style = round_indeterminate; // Not applicable
};
} // namespace std

#endif // BOOST_INCLUDED_XINT_INTEGER_HPP
