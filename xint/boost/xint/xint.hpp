
/*! \brief The Extended Integer (XInt) Library
   \details
    A fast, portable C++ library for multi-precision integer math.

    This is the main header file for the library, and the only one that
    programs using it should need to include.

\mainpage  eXtended Integer library.

A C++ library that lets your program handle much, much larger integer numbers
than the built-in int, long, or even long long types,
and handle them using the same syntax that C and C++ use for the standard integer types.

Completely portable, written entirely in modern C++,
with many different types of operating system, compiler, and hardware in mind.
It will compile cleanly on many operating systems without any changes,
automatically adapting to whatever native integer sizes are available.

It's fast. Speed of execution takes a back seat to portability,
so it doesn't include things like assembly-language modules
to wring every last CPU cycle out of it -- but it's still pretty darn fast.

Features you need. Modular arithmetic. Bit manipulation functions.
Cryptographically-secure random and prime number generation.  
A friendly and intuitive interface. An option for thread-safe operation.

It has the Boost Software License, Version 1.0. at
http://www.boost.org/LICENSE_1_0.txt

Documentation in full as html is at 
https://svn.boost.org/svn/boost/sandbox/xint/libs/xint/doc/html/index/html

and a pdf version is also available.

*/
//    Copyright 2010 by Chad Nelson

//    Distributed under the Boost Software License, Version 1.0.
//    See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_INCLUDED_XINT_H
#define BOOST_INCLUDED_XINT_H

#include <string>
#include <limits>
#include <memory> // for auto_ptr
#include <cstddef> // for size_t
#include <iostream>
#include <stdexcept>
#include <boost/integer.hpp>
#include <boost/cstdint.hpp>
#include <boost/function.hpp>
#include <boost/static_assert.hpp>

//! \namespace xint namespace for all extended integer classes and functions.
namespace xint
{  
////////////////////////////////////////////////////////////////////////////////
// The boring-but-necessary preliminary stuff

namespace detail
{ //!  \namespace xint::detail implementation details, normal users should not need to use these.
    typedef boost::uintmax_t doubledigit_t;
    typedef boost::uint_t<std::numeric_limits<doubledigit_t>::digits / 2>::fast
        digit_t;

    const size_t        bits_per_digit    = std::numeric_limits<digit_t>::digits;
    const digit_t       digit_hibit       = (digit_t(1) << (bits_per_digit-1));
    const doubledigit_t doubledigit_hibit = (doubledigit_t(1) << (bits_per_digit*2-1));
    const doubledigit_t digit_overflowbit = (doubledigit_t(1) << bits_per_digit);
    const digit_t       digit_mask        = digit_t(digit_overflowbit-1);

    const extern std::string nan_text;

    struct data_t;
    struct token { };
} // namespace detail

typedef std::auto_ptr<detail::token> token;

typedef boost::function<bool ()> callback_t;
const callback_t no_callback;

const size_t autobase=(std::numeric_limits<size_t>::max)();

class not_a_number;

////////////////////////////////////////////////////////////////////////////////
// The integer class

class integer
{  //! \brief The extended integer class.
   //! \details 
    public:
    integer(); //!< Constructs a default integer, value zero. (Can throw std::overflow_error if not enough memory to construct a new integer).
    integer(const integer& b); //!< Copy constructs a integer from another integer.
    //!< (Can throw std::overflow_error if not enough memory to construct a new integer).
    template <typename T> integer(const T& n); 
    explicit integer(const std::string& str, size_t base=10); //!< Copy constructs a integer from a digits string (decimal by default).
    explicit integer(const not_a_number&); //!< Constructs an extended integer with the NotANumber value;
    ~integer();

    bool odd() const; //!< \returns true if extended integer is odd.
    bool even() const; //!< \returns true if extended integer is even.
    int  sign() const; //!< \returns -1 if extended integer is < 0.
    bool nan() const; //!< \returns true if extended integer is Not-a-Number.

    size_t hex_digits() const; //!< \returns the number of hex digits to show the integer.

    integer& operator=(const integer &c);

    integer& operator+=(const integer& b);
    integer& operator-=(const integer& b);
    integer& operator*=(const integer& b);
    integer& operator/=(const integer& b);
    integer& operator%=(const integer& b);

    integer& operator++();
    integer& operator--();
    integer  operator++(int);
    integer  operator--(int);

    integer& operator&=(const integer& n);
    integer& operator|=(const integer& n);
    integer& operator^=(const integer& n);
    integer  operator<<(size_t shift) const;
    integer  operator>>(size_t shift) const;
    integer& operator<<=(size_t shift);
    integer& operator>>=(size_t shift);

    static const integer& zero(); //!< Extended integer holding zero.
    static const integer& one(); //!< Extended integer holding one.

    // These are used internally, they're probably not useful outside of the
    // library's functions.
    detail::data_t *_get_data()
    {  
      return data; //! \returns raw data representing an extended integer.
    }
    const detail::data_t *_get_data() const { return data; }
    detail::digit_t _get_digit(size_t index) const;
    size_t _get_length() const;
    void _set_negative(bool negative);
    void _make_unique();
    void _throw_if_nan() const;

    private: ///////////////////////////////////////////////////////////////////
    void _init(detail::digit_t init=0);
    void _init(const integer &c);
    void _init(boost::uintmax_t n);
    void _attach();
    void _detach();

    static const integer *cZero; 
    static const integer *cOne; 
    detail::data_t *data; //!< Raw data representing an extended integer.
};

////////////////////////////////////////////////////////////////////////////////
// Exception-blocking and -allowing functions

bool exceptions_allowed();
token block_exceptions();
token allow_exceptions();

////////////////////////////////////////////////////////////////////////////////
// Miscellaneous functions

bool opt_secure_mode();
bool opt_thread_safe();
int compare(const integer &n1, const integer &n2, bool ignoresign=false);
size_t log2(const integer& n);
integer gcd(const integer& num1, const integer& num2);
integer lcm(const integer& num1, const integer& num2);

////////////////////////////////////////////////////////////////////////////////
// Mathematical primitives

integer abs(const integer& n);
integer negate(const integer& n);
integer add(const integer& n, const integer& addend);
integer subtract(const integer& n, const integer& subtrahend);
integer multiply(const integer& n, const integer& multiplicand);
integer divide(const integer& dividend, const integer& divisor);
std::pair<integer, integer> divide_r(const integer& dividend, const integer&
    divisor);

////////////////////////////////////////////////////////////////////////////////
// Powers and roots

integer sqr(const integer& n);
integer pow(const integer& n, const integer& exponent);
integer pow2(size_t exponent);
integer factorial(size_t n);
integer sqrt(const integer& n);

////////////////////////////////////////////////////////////////////////////////
// Conversion functions

template <typename T> T to(const integer& n);
std::string to_string(const integer& n, size_t base=10, bool upperCase=false);
integer from_string(const std::string& str, size_t base=10);
std::string to_binary(const integer& n);
integer from_binary(const std::string& s);

////////////////////////////////////////////////////////////////////////////////
// Bit-manipulation functions

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

////////////////////////////////////////////////////////////////////////////////
// Modular math functions

integer mod(const integer& n, const integer& modBy);
integer mulmod(const integer& n, const integer& by, const integer& modulus);
integer sqrmod(const integer& n, const integer& modulus);
integer powmod(const integer& n, const integer& exponent, const integer&
    modulus, bool noMontgomery=false);
integer invmod(const integer& n, const integer& modulus);

////////////////////////////////////////////////////////////////////////////////
// Random number functions
bool seed_secure();
void seed_fallback();
void seed_manual(const std::string& value);
integer random_by_size(size_t sizeInBits, bool highBitOn=false, bool
    lowBitOn=false, bool canBeNegative=false);
template <typename T> T random();
template <typename T> T random(const T& lessThanThis);
template <typename T> T random(const T& lowest, const T& highest);

////////////////////////////////////////////////////////////////////////////////
// Prime number functions

int is_prime(const integer& n, callback_t callback=no_callback);
integer random_prime(size_t sizeInBits, callback_t callback=no_callback);

} // namespace xint

////////////////////////////////////////////////////////////////////////////////
// Global operators for the integer class

bool operator!(const xint::integer& n1);
bool operator<(const xint::integer& n1, const xint::integer& n2);
bool operator>(const xint::integer& n1, const xint::integer& n2);
bool operator<=(const xint::integer& n1, const xint::integer& n2);
bool operator>=(const xint::integer& n1, const xint::integer& n2);
bool operator==(const xint::integer& n1, const xint::integer& n2);
bool operator!=(const xint::integer& n1, const xint::integer& n2);

const xint::integer& operator+(const xint::integer& a);
xint::integer operator-(const xint::integer& a);
xint::integer operator+(const xint::integer& n1, const xint::integer& n2);
xint::integer operator-(const xint::integer& n1, const xint::integer& n2);
xint::integer operator*(const xint::integer& n1, const xint::integer& n2);
xint::integer operator/(const xint::integer& n1, const xint::integer& n2);
xint::integer operator%(const xint::integer& n1, const xint::integer& n2);
xint::integer operator&(const xint::integer& n1, const xint::integer& n2);
xint::integer operator|(const xint::integer& n1, const xint::integer& n2);
xint::integer operator^(const xint::integer& n1, const xint::integer& n2);

namespace xint {

////////////////////////////////////////////////////////////////////////////////
// Exception classes

class invalid_base: public std::invalid_argument {
    public:
    invalid_base(const std::string& what="invalid base"): invalid_argument(what)
        { }
};

class invalid_digit: public std::range_error {
    public:
    invalid_digit(const std::string& what="invalid digit"): range_error(what)
        { }
};

class invalid_modulus: public std::invalid_argument {
    public:
    invalid_modulus(const std::string& what="invalid modulus"):
        invalid_argument(what) { }
};

class divide_by_zero: public std::invalid_argument {
    public:
    divide_by_zero(const std::string& what="divide by zero error"):
        invalid_argument(what) { }
};

class too_big: public std::range_error {
    public:
    too_big(const std::string& what="value out of range for requested "
        "conversion"): range_error(what) { }
};

class not_a_number: public std::runtime_error {
    public:
    not_a_number(const std::string& what="attempted to use a Not-a-Number"):
        runtime_error(what) { }
};

////////////////////////////////////////////////////////////////////////////////
// Template function definitions

template <typename T>
integer::integer(const T& n) {
    if (std::numeric_limits<T>::is_signed) {
        if (n >= 0) {
            if (static_cast<T>(n & detail::digit_mask) == n)
                _init(detail::digit_t(n));
            else _init(boost::uintmax_t(n));
        } else if (n == (std::numeric_limits<T>::min)()) {
            // Have to treat the minimum number carefully, because -n is not
            // what you'd think it is.
            _init(boost::uintmax_t(-(n+1)));
            _set_negative(true);
            --(*this);
        } else {
            _init(boost::uintmax_t(-n));
            _set_negative(true);
        }
    } else {
        if (static_cast<T>(n & detail::digit_mask) == n)
            _init(detail::digit_t(n));
        else _init(boost::uintmax_t(n));
    }
}

template <typename T>
T to(const integer& n) {
    n._throw_if_nan();
    if (n < (std::numeric_limits<T>::min)()
        || n > (std::numeric_limits<T>::max)())
            throw too_big("value out of range for requested conversion");

    T rval=0;
    int len=n._get_length();
    for (int x=0; x<len; ++x)
        rval=static_cast<T>((rval * detail::digit_overflowbit)
            + n._get_digit(len-x-1));
    if (n.sign() < 0) rval *= -1;
    return rval;
}

template <typename T>
T random() {
    return random((std::numeric_limits<T>::min)(),
        (std::numeric_limits<T>::max)());
}

template <typename T>
T random(const T& lessThanThis) {
    return random(0, lessThanThis-1);
}

template <typename T>
T random(const T& lowest, const T& highest) {
    integer range(integer(highest)-lowest+1);
    return to<T>(lowest+(random_by_size(std::numeric_limits<T>::digits+1)
        % range));
}

// Customization for random integer within a range
template <>
integer random<integer>(const integer& lowest, const integer& highest);

template <typename charT, typename traits>
inline std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,
    traits>& out, const xint::integer& n)
{
    if (n.nan()) {
        out << detail::nan_text;
    } else {
        int base=((out.flags() & std::ios::hex) ? 16
            : (out.flags() & std::ios::oct) ? 8
            : 10);
        bool upperCase=(out.flags() & std::ios::uppercase ? true : false);

        if (out.flags() & std::ios::showbase) {
            if (n.sign() < 0) out << "-";

            if (base==16 && upperCase) out << "0X";
            else if (base==16) out << "0x";
            else if (base==8) out << "0";

            out << xint::to_string(abs(n), base, upperCase);
        } else {
            out << xint::to_string(n, base, upperCase);
        }
    }
    return out;
}

template <typename charT, typename traits>
inline std::basic_istream<charT,traits>& operator>>(std::basic_istream<charT,
    traits>& in, xint::integer& n)
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

    if (in.peek()=='#') {
        // Must be either #NaN# or an error
        char buffer[6];
        std::streamsize size=in.readsome(buffer, 5);
        buffer[size]=0;
        std::string str(buffer);

        if (str==detail::nan_text) n=integer(not_a_number());
        else in.setstate(std::ios::failbit);
    } else {
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

        token allow=allow_exceptions();
        try {
            xint::integer testValue=xint::from_string(s, base);
            n=testValue;
        } catch (xint::invalid_digit&) {
            // Catch invalid strings
            in.setstate(std::ios::failbit);
        }
    }

    return in;
}

} // namespace xint

#endif // BOOST_INCLUDED_XINT_H
