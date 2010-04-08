
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
    \brief The main header file for the XInt library.

    This is the only header that programs using the library should need to
    include.
*/

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
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

namespace boost {
//! \brief All of the commonly-used functions are within this namespace.
namespace xint {

////////////////////////////////////////////////////////////////////////////////
// The boring-but-necessary preliminary stuff

//! \brief The items within this namespace are meant for internal use only.
namespace detail {
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
    const digit_t       digit_hibit       = (digit_t(1) << (bits_per_digit-1));
    const doubledigit_t doubledigit_hibit = (doubledigit_t(1) << (bits_per_digit*2-1));
    const doubledigit_t digit_overflowbit = (doubledigit_t(1) << bits_per_digit);
    const digit_t       digit_mask        = digit_t(digit_overflowbit-1);
    //!@}

    //! Holds the text representation of the Not-a-Number value.
    const extern std::string nan_text;

    struct data_t;

    //! An empty struct, used in xint::token
    struct token { };

    //! The base class for random_generator.
    struct base_random_generator {
        typedef unsigned int result_type;
        virtual result_type operator()()=0;
    };

    //! Support class for the random generator code.
    template <class T>
    struct random_generator: public base_random_generator {
        typedef boost::uniform_int<result_type> dist_t;
        typedef boost::variate_generator<T&, dist_t> gen_t;
        random_generator(T *g): genobj(g), gen(*g,
            dist_t((std::numeric_limits<result_type>::min)(),
                (std::numeric_limits<result_type>::max)())) { }
        result_type operator()() { return gen(); }
        std::auto_ptr<T> genobj;
        gen_t gen;
    };

    //! Support function \see xint::set_random_generator
    void set_random_generator(base_random_generator *obj);

    /*!
        \return An unsigned int from the current random generator.
        \see xint::set_random_generator
    */
    unsigned int get_random();
} // namespace detail

//! Used for the exception-blocker.
typedef std::auto_ptr<detail::token> token;

//! A callback function takes no parameters and returns a bool.
//! @see xint::is_prime
//! @see xint::random_prime
typedef boost::function<bool ()> callback_t;

//! An empty callback_t, used as a default parameter for some functions.
//! @see xint::is_prime
//! @see xint::random_prime
const callback_t no_callback;

//! A value that can be used for the 'base' parameter of the string-to-integer functions
//! @see xint::integer::integer(const std::string& str, size_t base)
//! @see xint::from_string
const size_t autobase=(std::numeric_limits<size_t>::max)();

class integer;

/*! \brief Contains the core integer type and core functions.

The types and functions in the xint::core namespace operate exactly like those
of the same names in the xint namespace, except that they ignore any
exception-blocking and do not support the Not-a-Number value. If you attempt to
assign a Not-a-Number to a core::integer, it will throw an xint::not_a_number
exception.

These functions are not documented separately; see the corresponding classes and
functions in the xint::integer namespace for documentation on them.
*/
namespace core {

//! \brief The core integer class.
class integer {
    public:

    //! \name Constructors & Destructors
    //!@{
    integer();
    integer(const integer& b);
    explicit integer(const ::boost::xint::integer& b);
    template <typename T> integer(const T& n,
        typename boost::enable_if<boost::is_integral<T> >::type* = 0,
        typename boost::enable_if<boost::is_signed<T> >::type* = 0);
    template <typename T> integer(const T& n,
        typename boost::enable_if<boost::is_integral<T> >::type* = 0,
        typename boost::enable_if<boost::is_unsigned<T> >::type* = 0);
    explicit integer(const std::string& str, size_t base=10);
    ~integer();
    //!@}

    //! \name Mathematical and Bitwise Operators
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

    //! \name Preconstructed integers
    //@{
    static const integer& zero();
    static const integer& one();
    //@}

    //! \name Miscellaneous Functions
    //!@{
    bool odd() const;
    bool even() const;
    int  sign() const;

    size_t hex_digits() const;
    //!@}

    //! \name Internal Functions
    //@{
    detail::data_t *_get_data() { return data; }
    const detail::data_t *_get_data() const { return data; }
    detail::digit_t _get_digit(size_t index) const;
    detail::digit_t _get_digit(size_t index, bool) const; //!< \overload
    size_t _get_length() const;
    void _set_negative(bool negative);
    void _make_unique();
    //@}

    protected: /////////////////////////////////////////////////////////////////
    void _init(detail::digit_t init=0);
    void _init(const integer &c); //!< \overload
    void _init(boost::uintmax_t n); //!< \overload
    void _attach();
    void _detach();

    detail::data_t *data;
};

} // namespace core

////////////////////////////////////////////////////////////////////////////////
// The integer class

/*! \brief The extended %integer class.

    This class implements the main \c integer type.

    There are only a few member functions; most of the functionality is
    implemented using standalone functions.

    Functions that start with an underscore (such as \c _make_unique) are
    intended for internal use only. They may change arbitrarily in future
    versions.
*/
class integer {
    public:

    //! \name Constructors & Destructors
    //!@{
    integer();
    integer(const integer& b);
    integer(const core::integer& b);
    template <typename T> integer(const T& n,
        typename boost::enable_if<boost::is_integral<T> >::type* = 0,
        typename boost::enable_if<boost::is_signed<T> >::type* = 0);
    template <typename T> integer(const T& n,
        typename boost::enable_if<boost::is_integral<T> >::type* = 0,
        typename boost::enable_if<boost::is_unsigned<T> >::type* = 0);
    explicit integer(const std::string& str, size_t base=10);
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

    //! \name Miscellaneous Functions
    //!@{
    bool odd() const;
    bool even() const;
    int  sign() const;
    bool is_nan() const;

    size_t hex_digits() const;
    //!@}

    /*! \name Preconstructed integers

        These return preconstructed integers with the values named by the
        functions. They are used internally, so that the most common values
        don't have to be constructed each time they're needed, but they are
        available to your own code as well.
    */
    //@{
    static const integer& zero();
    static const integer& one();
    static const integer& nan();
    //@}

    /*! \name Internal Functions
        \brief These functions allow access to the internals of the %integer.
               They are intended for internal use only.
    */
    //@{
    detail::data_t *_get_data() { return data; }
    const detail::data_t *_get_data() const { return data; }
    detail::digit_t _get_digit(size_t index) const;
    detail::digit_t _get_digit(size_t index, bool) const; //!< \overload
    size_t _get_length() const;
    void _set_negative(bool negative);
    void _make_unique();
    //@}

    private: /////////////////////////////////////////////////////////////////
    void _init(detail::digit_t init=0);
    void _init(const integer &c); //!< \overload
    void _init(boost::uintmax_t n); //!< \overload
    void _attach();
    void _detach();

    detail::data_t *data;
};

////////////////////////////////////////////////////////////////////////////////

/*! \brief A convenience class for cryptographically-secure random numbers.

    Used with xint::set_random_generator to set the XInt library to use the
    system-supplied secure random number generator.

    \see \ref random
*/
class strong_random_generator {
    public:
    typedef unsigned int result_type;

    strong_random_generator();
    ~strong_random_generator();
    result_type operator()();

    static const bool has_fixed_range;
    static const result_type min_value;
    static const result_type max_value;
    result_type min() const;
    result_type max() const;
    double entropy() const;

    private:
    struct impl_t;
    impl_t *impl;
};

////////////////////////////////////////////////////////////////////////////////

/*! \name Exception-blocking and -allowing functions

    See the \ref exceptions "exception-blocking page" for details on these
    functions and how to use them.
*/
//!@{
bool exceptions_allowed();
token block_exceptions();
token allow_exceptions();
//!@}

////////////////////////////////////////////////////////////////////////////////

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

namespace core {
integer abs(const integer& n);
integer negate(const integer& n);
integer add(const integer& n, const integer& addend);
integer subtract(const integer& n, const integer& subtrahend);
integer multiply(const integer& n, const integer& multiplicand);
integer divide(const integer& dividend, const integer& divisor);
std::pair<integer, integer> divide_r(const integer& dividend, const integer&
    divisor);
} // namespace core
//!@}

////////////////////////////////////////////////////////////////////////////////

//! \name Powers and roots
//!@{
integer sqr(const integer& n);
integer pow(const integer& n, const integer& exponent);
integer pow2(size_t exponent);
integer factorial(size_t n);
integer sqrt(const integer& n);

namespace core {
integer sqr(const integer& n);
integer pow(const integer& n, const integer& exponent);
integer pow2(size_t exponent);
integer factorial(size_t n);
integer sqrt(const integer& n);
} // namespace core
//!@}

////////////////////////////////////////////////////////////////////////////////

//! \name Conversion functions
//!@{
template <typename T> T to(const integer& n);
std::string to_string(const integer& n, size_t base=10, bool upperCase=false);
integer from_string(const std::string& str, size_t base=10);
std::string to_binary(const integer& n);
integer from_binary(const std::string& s);

namespace core {
template <typename T> T to(const integer& n);
std::string to_string(const integer& n, size_t base=10, bool upperCase=false);
integer from_string(const std::string& str, size_t base=10);
std::string to_binary(const integer& n);
integer from_binary(const std::string& s);
} // namespace core
//!@}

////////////////////////////////////////////////////////////////////////////////

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

namespace core {
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
} // namespace core
//!@}

////////////////////////////////////////////////////////////////////////////////

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

namespace core {
integer mod(const integer& n, const integer& modBy);
integer mulmod(const integer& n, const integer& by, const integer& modulus);
integer sqrmod(const integer& n, const integer& modulus);
integer powmod(const integer& n, const integer& exponent, const integer&
    modulus, bool noMontgomery=false);
integer invmod(const integer& n, const integer& modulus);
} // namespace core
//!@}

////////////////////////////////////////////////////////////////////////////////

//! \name Random number functions
//!@{
template <typename T> void set_random_generator(T *gen);
integer random_by_size(size_t sizeInBits, bool highBitOn=false, bool
    lowBitOn=false, bool canBeNegative=false);

namespace core {
integer random_by_size(size_t sizeInBits, bool highBitOn=false, bool
    lowBitOn=false, bool canBeNegative=false);
} // namespace core
//!@}

////////////////////////////////////////////////////////////////////////////////

//! \name Prime number functions
//!@{
int is_prime(const integer& n, callback_t callback=no_callback);
integer random_prime(size_t sizeInBits, callback_t callback=no_callback);

namespace core {
int is_prime(const integer& n, callback_t callback=no_callback);
integer random_prime(size_t sizeInBits, callback_t callback=no_callback);
} // namespace core
//!@}

////////////////////////////////////////////////////////////////////////////////

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

namespace core {
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
} // namespace core
//!@}

////////////////////////////////////////////////////////////////////////////////

//! \name Miscellaneous functions
//!@{
bool opt_secure_mode();
bool opt_thread_safe();

int compare(const integer &n1, const integer &n2, bool ignoresign=false);
size_t log2(const integer& n);
integer gcd(const integer& num1, const integer& num2);
integer lcm(const integer& num1, const integer& num2);

namespace core {
int compare(const integer &n1, const integer &n2, bool ignoresign=false);
size_t log2(const integer& n);
integer gcd(const integer& num1, const integer& num2);
integer lcm(const integer& num1, const integer& num2);
} // namespace core
//!@}

////////////////////////////////////////////////////////////////////////////////
// Exception classes

//! \name Exception classes
//!@{
/*! \brief Exception class

    Derived from \c std::invalid_argument.
*/
class invalid_base: public std::invalid_argument {
    public:
    invalid_base(const std::string& what="invalid base"): invalid_argument(what)
        { }
};

/*! \brief Exception class

    Derived from \c std::range_error.
*/
class invalid_digit: public std::range_error {
    public:
    invalid_digit(const std::string& what="invalid digit"): range_error(what)
        { }
};

/*! \brief Exception class

    Derived from \c std::invalid_argument.
*/
class invalid_modulus: public std::invalid_argument {
    public:
    invalid_modulus(const std::string& what="invalid modulus"):
        invalid_argument(what) { }
};

/*! \brief Exception class

    Derived from \c std::invalid_argument.
*/
class divide_by_zero: public std::invalid_argument {
    public:
    divide_by_zero(const std::string& what="divide by zero error"):
        invalid_argument(what) { }
};

/*! \brief Exception class

    Derived from \c std::invalid_argument.
*/
class cannot_represent: public std::invalid_argument {
    public:
    cannot_represent(const std::string& what=
        "cannot represent the requested value"): invalid_argument(what) { }
};

/*! \brief Exception class

    Derived from \c std::range_error.
*/
class too_big: public std::range_error {
    public:
    too_big(const std::string& what=
        "value out of range for requested conversion"): range_error(what) { }
};

/*! \brief Exception class

    Derived from \c std::runtime_error.

    \remarks
    Most library functions will throw an xint::not_a_number exception if you try
    to use a \link nan Not-a-Number value \endlink in them.

    \par
    See this page for details on Not-a-Number.
*/
class not_a_number: public std::runtime_error {
    public:
    not_a_number(const std::string& what="attempted to use a Not-a-Number"):
        runtime_error(what) { }
};

/*! \brief Exception class

    Derived from \c std::runtime_error.

    \note Cannot be blocked.

    \remarks
    Thrown by the xint::strong_random_generator class's constructor, if the
    system does not provide a cryptographically-secure random generator. Also
    thrown if the generator fails when asked to provide a random value, though
    this should be vanishingly rare.
*/
class no_strong_random: public std::runtime_error {
    public:
    no_strong_random(const std::string& what=
        "system does not have a strong random generator"):
        runtime_error(what) { }
};

/*! \brief Exception class

    Derived from \c std::overflow_error.

    \remarks
    Thrown when the library can't get enough memory to represent a number.
*/
class overflow_error: public std::overflow_error {
    public:
    overflow_error(const std::string& what=
        "overflow error: can't obtain enough memory to represent the requested number"):
        std::overflow_error(what) { }
};
//!@}

////////////////////////////////////////////////////////////////////////////////
// Template function definitions

namespace core {
template <typename T> integer::integer(const T& n,
    typename boost::enable_if<boost::is_integral<T> >::type*,
    typename boost::enable_if<boost::is_signed<T> >::type*)
{
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
}
} // namespace core

namespace core {
template <typename T> integer::integer(const T& n,
    typename boost::enable_if<boost::is_integral<T> >::type*,
    typename boost::enable_if<boost::is_unsigned<T> >::type*)
{
    if (static_cast<T>(n & detail::digit_mask) == n)
        _init(detail::digit_t(n));
    else _init(boost::uintmax_t(n));
}
} // namespace core

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
    data=0;
    try {
        *this=integer(core::integer(n));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        delete data;
        data=0;
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
    data=0;
    try {
        *this=integer(core::integer(n));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        delete data;
        data=0;
    }
}

namespace core {
template <typename T>
T to(const integer& n) {
    if (n < (std::numeric_limits<T>::min)()
        || n > (std::numeric_limits<T>::max)())
            throw too_big("value out of range for requested conversion");

    T rval=0;
    size_t len=n._get_length();
    for (size_t x=0; x<len; ++x)
        rval=static_cast<T>((rval * detail::digit_overflowbit)
            + n._get_digit(len-x-1));
    if (n.sign() < 0) rval *= -1;
    return rval;
}
} // namespace core

/*! \brief Efficiently converts from an xint::integer to a built-in %integer
           type.

\param[in] n The integer to convert.
\tparam T The type to convert it to.

\returns The numeric value of \c n, converted to the specified type.

\exception xint::too_big if \c n would not fit into the specified type.

\note If exceptions are blocked, returns T(0) instead of throwing.
*/
template <typename T>
T to(const integer& n) {
    try {
        return to<T>(core::integer(n));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return 0;
    }
}

/*! \brief Change the random generator that the library uses.

\param[in] gen A pointer to the new generator to use.

\warning The generator must be a heap-allocated pointer (one created with
<code>new</code>), which the XInt library assumes ownership of. Don't pass in a
stack-created item, don't try to delete the item that you passed in, and don't
attempt to use it separately after passing it to this function, or the behavior
is undefined.

\remarks
See the \link random Random Number page\endlink for an example of how to use it.
*/
template <typename T>
void set_random_generator(T *gen) {
    detail::random_generator<T> *obj=new detail::random_generator<T>(gen);
    detail::set_random_generator(obj);
}

//! \name Stream input/output functions
//!@{
namespace core {
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
} // namespace core

namespace core {
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

    token allow=allow_exceptions();
    try {
        integer testValue=from_string(s, base);
        n=testValue;
    } catch (std::exception&) {
        // Catch invalid strings
        in.setstate(std::ios::failbit);
    }

    return in;
}
} // namespace core
//!@}

//! \name Stream input/output functions
//!@{
template <typename charT, typename traits>
inline std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,
    traits>& out, const integer& n)
{
    if (n.is_nan()) {
        out << detail::nan_text;
        return out;
    } else {
        return core::operator<<(out, core::integer(n));
    }
}

template <typename charT, typename traits>
inline std::basic_istream<charT,traits>& operator>>(std::basic_istream<charT,
    traits>& in, integer& n)
{
    if (in.peek()=='#') {
        // Must be either #NaN# or an error
        char buffer[6];
        std::streamsize size=in.readsome(buffer, 5);
        buffer[size]=0;
        std::string str(buffer);

        if (str==detail::nan_text) n=integer::nan();
        else in.setstate(std::ios::failbit);
    } else {
        core::integer nn;
        core::operator>>(in, nn);
        if (in) n=nn;
    }

    return in;
}
//!@}

} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_H
