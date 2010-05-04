
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
    \brief Contains the internal definitions required for all XInt types.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_INTERNALS_HPP
#define BOOST_INCLUDED_XINT_INTERNALS_HPP

// For performance testing, uncomment one of these defines.
//#define XINT_TEST_BASELINE
//#define XINT_TEST_MOVE
//#define XINT_TEST_COPY_ON_WRITE

#include <string>
#include <vector>
#include <queue>
#include <limits>
#include <cstddef> // for size_t
#include <iostream>
#include <stdexcept>
#include <boost/integer.hpp>
#include <boost/cstdint.hpp>
#include <boost/function.hpp>
#include <boost/type_traits.hpp>

#if defined XINT_TEST_BASELINE
    #define PERFORMANCE_TEST
#elif defined XINT_TEST_COPY_ON_WRITE
    #define PERFORMANCE_TEST
    #define BOOST_XINT_USE_COPY_ON_WRITE
#elif defined XINT_TEST_MOVE
    #define PERFORMANCE_TEST
    #define BOOST_XINT_USE_MOVE
#else
    #ifdef XINT_THREADSAFE
        #define BOOST_XINT_USE_MOVE
    #else
        #define BOOST_XINT_USE_COPY_ON_WRITE
    #endif
#endif

#ifdef BOOST_XINT_USE_MOVE
    #include <boost/move/move.hpp>
    #define BOOST_XINT_COPY_ASSIGN_REF(type) BOOST_COPY_ASSIGN_REF(type)
    #define BOOST_XINT_COPYABLE_AND_MOVABLE(type) BOOST_COPYABLE_AND_MOVABLE(type)
    #define BOOST_XINT_RV_REF(type) BOOST_RV_REF(type)
    #define BOOST_XINT_MOVE(value) boost::move(value)
    #define BOOST_XINT_FORWARD(type, value) boost::forward<type>(value)
#else
    template <typename T> class disable_boost_rv_ref: public T { };
    #define BOOST_XINT_COPY_ASSIGN_REF(type) const type&
    #define BOOST_XINT_COPYABLE_AND_MOVABLE(type)
    #define BOOST_XINT_RV_REF(type) disable_boost_rv_ref<type>
    #define BOOST_XINT_MOVE(value) value
    #define BOOST_XINT_FORWARD(type, value) value
#endif

namespace boost {

//! \brief All the types and functions of the library are within this namespace.
namespace xint {

class integer;
class nothrow_integer;
class fixed_integer_any;
template <size_t Bits> class fixed_integer;

//! A callback function takes no parameters and returns a bool.
//! \see xint::is_prime
//! \see xint::random_prime
typedef boost::function<bool ()> callback_t;

//! The format for the binary representations of %integers.
//! \see xint::to_binary
//! \see xint::from_binary
typedef std::vector<unsigned char> binary_t;

//! An empty callback_t, used as a default parameter for some functions.
//! \see xint::is_prime
//! \see xint::random_prime
const callback_t no_callback;

//! A value that can be used for the 'base' parameter of the string-to-integer functions
//! \see xint::integer::integer(const std::string& str, size_t base)
//! \see xint::from_string
const size_t autobase=(std::numeric_limits<size_t>::max)();

//! \brief The items within this namespace are meant for internal use only.
namespace detail {
//! @cond detail

/*!
    This is the type used for calculations. It should be the largest
    built-in integral type the system can offer, for maximum processing
    efficiency.
*/
typedef boost::uintmax_t doubledigit_t;

/*!
    This is the type used to store information about the %integer. It must
    contain at least half the number of bits that a doubledigit_t can hold,
    and is most memory-efficient when it's exactly that.
*/
typedef boost::uint_t<std::numeric_limits<doubledigit_t>::digits / 2>::fast
    digit_t;

//! \name Some constants used by the library.
//!@{
const size_t        bits_per_digit    = std::numeric_limits<digit_t>::digits;
const size_t        digits_in_uintmax =
    (std::numeric_limits<boost::uintmax_t>::digits + bits_per_digit - 1) /
    bits_per_digit;
const digit_t       digit_hibit       = (digit_t(1) << (bits_per_digit-1));
const doubledigit_t doubledigit_hibit = (doubledigit_t(1) << (bits_per_digit*2-1));
const doubledigit_t digit_overflowbit = (doubledigit_t(1) << bits_per_digit);
const digit_t       digit_mask        = digit_t(digit_overflowbit-1);
//!@}

//! Holds the text representation of the Not-a-Number value.
const extern std::string nan_text;

//! \brief Holds the magnitude digits of an integer.
struct data_t {
    size_t copies, length, max_length;
    digit_t fixed_mask; //!< For the topmost digit; zero for variable-sized
    digit_t magnitude[1];
};

//! \name The data_t functions.
//!@{
void zero(digit_t *p, digit_t *end);
void zero(digit_t *p, size_t size);
data_t *allocate(size_t max_digits, size_t initial_length=
    (std::numeric_limits<size_t>::max)(), digit_t fixed_mask=0);
data_t *allocate_fixed(digit_t mask, size_t max_digits, size_t initial_length=
    (std::numeric_limits<size_t>::max)());
data_t *allocate_fixed_zero(digit_t mask, size_t max_digits);
data_t *reallocate(data_t *olddata, size_t newsize);
data_t *duplicate(data_t *olddata, size_t extra=0);
data_t *duplicate(data_t *olddata, data_t *data_to_copy, size_t extra=0);
void deallocate(data_t *data);
//!@}

class temp_t;
class base_variable_length_integer;

//! \brief The base class for all integer classes.
class base_integer {
    public:
    ~base_integer();

    //! \name Internal Functions
    //@{
    detail::digit_t _get_digit(size_t index) const;
    detail::digit_t _get_digit(size_t index, bool) const; //!< \overload
    detail::digit_t* _get_writable_digits();
    const detail::digit_t* _get_digits() const; //!< \overload

    size_t _get_length() const;
    size_t _get_max_length() const;
    void _set_length(size_t digits);

    size_t _log2() const;

    bool _is_nan() const { return (data==0); };
    bool _is_unique() const;
    bool _is_zero() const;

    bool _is_fixed() const;
    size_t _fixed_digits() const;
    size_t _fixed_bits() const;

    bool _get_readonly() const;
    void _mark_movable();
    bool _is_movable() const;
    bool _get_negative() const;
    void _set_negative(bool neg);
    void _toggle_negative();

    void _realloc(size_t new_digit_count);
    void _cleanup();
    void _attach(const base_integer& copy, bool treat_as_movable);

    void _increment(bool absolute_value=false);
    void _decrement(bool absolute_value=false);

    integer _to_integer(bool treat_as_movable) const;
    void _set_signed(boost::intmax_t n);
    void _set_unsigned(boost::uintmax_t n, bool negative=false);

    void _make_unique();
    void _swap(base_integer& other);
    //@}

    protected:
    typedef unsigned int flag_t;

    base_integer();
    base_integer(const base_integer& c, bool fixed);
    explicit base_integer(data_t *adopt, bool neg=false);
    base_integer(size_t count, digit_t mask);
    void _base_attach(data_t *new_data, flag_t flags = 0, size_t
        extra_allocation = 0, bool is_movable = false);

    void _set_readonly();

    protected:
    static const flag_t flag_negative = 0x01;
    static const flag_t flag_readonly = 0x02;
    static const flag_t flag_fixedlength = 0x04;
    static const flag_t flag_movable = 0x08;

    private:
    flag_t flags;
    data_t *data;

    friend class temp_t;
    friend class base_variable_length_integer;
};

//! \brief Makes a temporary integer that can always share storage.
class temp_t: public base_integer {
    public:
    explicit temp_t(const base_integer& b) {
        flags = b.flags;
        data = b.data;
        ++data->copies;
    }

    //! Constructs an empty integer of the same type as the one passed in.
    temp_t(const base_integer& b, size_t realloc_size):
        base_integer(b._is_fixed()
            ? allocate_fixed(b.data->fixed_mask, b._fixed_digits(), realloc_size)
            : allocate(realloc_size)) { }
};

//! \brief Makes a negated temporary integer that can always share storage.
class neg_t: public temp_t {
    public:
    explicit neg_t(const base_integer& b): temp_t(b) {
        _toggle_negative();
    }
};

//! \brief The base class for the variable-length integer classes.
class base_variable_length_integer: public base_integer {
    protected:
    void _attach_0();
};

//! \brief Used internally to reorder bits into different-sized groupings.
class bitqueue_t {
    public:
    bitqueue_t(): bits_pending(0) { }
    void push(doubledigit_t bits, size_t count);
    doubledigit_t pop(size_t requestedbits);
    bool empty() const { return pending.empty(); }
    size_t size() const { return bits_pending; }

    private:
    static const size_t ddbits = std::numeric_limits<doubledigit_t>::digits;
    typedef std::pair<doubledigit_t, size_t> indata_t;
    typedef std::queue<indata_t> inqueue_t;

    size_t bits_pending;
    inqueue_t pending;
};

int compare(const base_integer &b1, const base_integer &b2, bool
    ignoresign=false);
std::string to_string(const base_integer& n, size_t base, bool uppercase);
void from_string(base_integer& target, const std::string& str, size_t base);
void add(base_integer& target, const base_integer& n1, const base_integer& n2);
void subtract(base_integer& target, const base_integer& n1, const base_integer&
    n2);
void multiply(base_integer& target, const base_integer& n1, const base_integer&
    n2);
void divide(base_integer& qtarget, base_integer& rtarget, const base_integer&
    n1, const base_integer& n2);
void divide(base_integer& target, const base_integer& n1, const base_integer&
    n2);
void sqr(base_integer& target, const base_integer& n);
void pow(base_integer& target, const base_integer& n, const base_integer& e);
void factorial(base_integer& target, size_t n);
void sqrt(base_integer& target, const base_integer& n);
bool getbit(const base_integer& n, size_t bit);
void setbit(base_integer& n, size_t bit);
void clearbit(base_integer& n, size_t bit);
size_t lowestbit(const base_integer& n, size_t valueIfZero);
size_t highestbit(const base_integer& n, size_t valueIfZero);
void bitwise_and(base_integer& target, const base_integer& n1, const
    base_integer& n2);
void bitwise_or(base_integer& target, const base_integer& n1, const
    base_integer& n2);
void bitwise_xor(base_integer& target, const base_integer& n1, const
    base_integer& n2);
void shift_left(base_integer& target, const base_integer& n, size_t byBits);
void shift_right(base_integer& target, const base_integer& n, size_t byBits);
void to_binary(xint::binary_t& target, const base_integer& n, size_t bits);
void from_binary(base_integer& target, const xint::binary_t& s, size_t bits);
void gcd(base_integer& target, const base_integer& num1, const base_integer&
    num2);
void lcm(base_integer& target, const base_integer& num1, const base_integer&
    num2);
void mod(base_integer& target, const base_integer& n1, const base_integer&
    n2);
void invmod(base_integer& target, const base_integer& n, const base_integer& m);
void powmod(base_integer& target, const base_integer& n, const base_integer& e,
    const base_integer& m, bool noMontgomery);

size_t log10_bits(size_t bits);

//! @endcond detail
} // namespace detail

/*! \brief The base class for all fixed_integer types.

This class eases operations on different-sized fixed_integer types.
*/
class fixed_integer_any: public detail::base_integer {
    public:
    /*! \name Automatic conversion operators.

        These allow the compiler to create a temporary fixed_integer, of the
        size of boost::uintmax_t, for proper comparisons between fixed_integer
        types and literals. They shouldn't be used for any other purposes.
    */
    template <typename T> fixed_integer_any(const T& n,
        typename boost::enable_if<boost::is_integral<T> >::type* = 0,
        typename boost::enable_if<boost::is_signed<T> >::type* = 0):
        base_integer(detail::digits_in_uintmax, detail::digit_mask)
        { _set_signed(n); }

    template <typename T> fixed_integer_any(const T& n,
        typename boost::enable_if<boost::is_integral<T> >::type* = 0,
        typename boost::enable_if<boost::is_unsigned<T> >::type* = 0):
        base_integer(detail::digits_in_uintmax, detail::digit_mask)
        { _set_unsigned(n); }

    protected:
    fixed_integer_any(size_t count, detail::digit_t mask):
        base_integer(count, mask) { }
    explicit fixed_integer_any(detail::data_t *adopt, bool neg=false):
        base_integer(adopt, neg) { }
    explicit fixed_integer_any(const fixed_integer_any& c):
        base_integer(c, true) { }
};

//! \name Miscellaneous functions
//!@{
bool opt_secure_mode();
bool opt_thread_safe();
//!@}

/*! \brief Holds the results from a division-with-remainder call.

This class serves essentially the same purpose as an \c std::pair. The only
reason it exists is that \c std::pair doesn't work well with Boost.Move on some
compilers.
*/
template <class T>
class base_divide_t {
    public:
    base_divide_t() { }
    base_divide_t(const T& q, const T& r): quotient(q), remainder(r) { }
    base_divide_t(const base_divide_t<T>& copy):
        quotient(copy.quotient), remainder(copy.remainder) { }
    base_divide_t(BOOST_XINT_RV_REF(base_divide_t) move) {
        quotient._swap(move.quotient); remainder._swap(move.remainder); }

    base_divide_t& operator=(BOOST_XINT_COPY_ASSIGN_REF(base_divide_t) copy)
        { quotient = copy.quotient; remainder = copy.remainder; return
        *this; }
    base_divide_t& operator=(BOOST_XINT_RV_REF(base_divide_t) move) {
        quotient._swap(move.quotient); remainder._swap(move.remainder); return
        *this; }

    T quotient, remainder;

    private:
    BOOST_XINT_COPYABLE_AND_MOVABLE(base_divide_t)
};

} // namespace xint
} // namespace boost

////////////////////////////////////////////////////////////////////////////////
// Template functions

#include "exceptions.hpp"

namespace boost {
namespace xint {

//! @cond detail
namespace detail {

template <typename T>
size_t log2(const T& n,
    typename boost::enable_if<boost::is_integral<T> >::type* = 0,
    typename boost::enable_if<boost::is_unsigned<T> >::type* = 0)
{
    size_t lo=0, hi=std::numeric_limits<T>::digits;
    while (lo < hi) {
        size_t s = lo + ((hi - lo) >> 1);
        if (n >= (T(1) << s)) lo = s + 1;
        else hi = s;
    }
    return lo;
}

template <typename T>
size_t log2(const T& nn,
    typename boost::enable_if<boost::is_integral<T> >::type* = 0,
    typename boost::enable_if<boost::is_signed<T> >::type* = 0)
{
    typedef typename boost::make_unsigned<T>::type uT;
    return log2(static_cast<uT>(nn < 0 ? -nn : nn));
}

template <typename T>
T to(const base_integer& n) {
    if (compare(n, fixed_integer_any((std::numeric_limits<T>::min)())) < 0
        || compare(n, fixed_integer_any((std::numeric_limits<T>::max)())) > 0)
            throw exceptions::too_big("value out of range for requested "
                "conversion");

    T rval = 0;
    size_t len=n._get_length();
    for (size_t x=0; x<len; ++x) {
        if (sizeof(T) > sizeof(detail::digit_t)) rval <<= detail::bits_per_digit;
        rval += static_cast<T>(n._get_digit(len - x - 1));
    }
    if (n._get_negative()) rval = -rval; // Bogus MSVC warning C4146
    return rval;
}

template <typename charT, typename traits>
inline std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,
    traits>& out, const base_integer& n)
{
    int base=((out.flags() & std::ios::hex) ? 16
        : (out.flags() & std::ios::oct) ? 8
        : 10);
    bool upperCase=(out.flags() & std::ios::uppercase ? true : false);

    int nsign = (n._is_zero() ? 0 : n._get_negative() ? -1 : 1);
    if ((out.flags() & std::ios::showpos) && nsign >= 0) out << "+";

    if (out.flags() & std::ios::showbase) {
        if (nsign < 0) out << "-";

        if (base==16 && upperCase) out << "0X";
        else if (base==16) out << "0x";
        else if (base==8) out << "0";

        if (n._get_negative()) {
            out << to_string(neg_t(n), base, upperCase);
        } else {
            out << to_string(n, base, upperCase);
        }
    } else {
        out << to_string(n, base, upperCase);
    }
    return out;
}

template <typename charT, typename traits>
inline std::basic_istream<charT,traits>& operator>>(std::basic_istream<charT,
    traits>& in, base_integer& n)
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
        detail::from_string(n, s, base);
    } catch (std::exception&) {
        // Catch invalid strings
        in.setstate(std::ios::failbit);
    }

    return in;
}

} // namespace detail
//! @endcond detail

} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_INTERNALS_HPP
