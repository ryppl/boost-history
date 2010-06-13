
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
    \brief Contains the most basic types, constants, and includes.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_BASIC_TYPES_HPP
#define BOOST_INCLUDED_XINT_BASIC_TYPES_HPP

#ifdef _WIN32
    // Solely for the strong random number generator.
    #define STRICT
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

#include <string>
#include <vector>
#include <queue>
#include <limits>
#include <cstddef> // for std::size_t
#include <cassert>
#include <sstream>
#include <fstream>
#include <ctime>
#include <boost/integer.hpp>
#include <boost/cstdint.hpp>
#include <boost/function.hpp>
#include <boost/parameter.hpp>
#include <boost/type_traits.hpp>
#include <boost/scoped_array.hpp>
#include <boost/static_assert.hpp>

// Uncomment this to use Boost.Move, which can improve the speed when using
// extremely large numbers.
//#define BOOST_XINT_USE_MOVE

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
namespace xint {

//! \brief A callback function takes no parameters and returns a bool.
typedef boost::function<bool ()> callback_t;

//! \brief An empty callback_t, used as a default parameter for some functions.
const callback_t no_callback;

//! \brief The format for the binary representations of %integers.
typedef std::vector<unsigned char> binary_t;

//! \brief A value that can be used for the 'base' parameter of the
//!        string-to-integer constructor functions.
const std::size_t autobase = (std::numeric_limits<size_t>::max)();

//! \brief This makes it easier to tell when an item is measured in bits.
typedef std::size_t bitsize_t;

//! \brief Holds the results from a division-with-remainder call.
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
        { quotient = copy.quotient; remainder = copy.remainder; return *this; }
    base_divide_t& operator=(BOOST_XINT_RV_REF(base_divide_t) move) {
        using std::swap;
        swap(quotient, move.quotient);
        swap(remainder, move.remainder);
        return *this; }

    T quotient, remainder;

    private:
    BOOST_XINT_COPYABLE_AND_MOVABLE(base_divide_t)
};

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
const std::size_t        minimum_digits    = 1;
const std::size_t        bits_per_digit    =
    std::numeric_limits<digit_t>::digits;
const std::size_t        digits_in_uintmax =
    (std::numeric_limits<boost::uintmax_t>::digits + bits_per_digit - 1) /
    bits_per_digit;
const digit_t       digit_hibit       = (digit_t(1) << (bits_per_digit-1));
const doubledigit_t doubledigit_hibit = (doubledigit_t(1) << (bits_per_digit*2-1));
const doubledigit_t digit_overflowbit = (doubledigit_t(1) << bits_per_digit);
const digit_t       digit_mask        = digit_t(digit_overflowbit-1);
//!@}

////////////////////////////////////////////////////////////////////////////////

//! Holds the magnitude digits of an integer. Uses the "struct hack" -- look it
//! up if you're not familiar with it.
struct magnitude_t {
    private:
    std::size_t copy_count;

    public:
    const bool readonly;
    const std::size_t max_length;
    digit_t digits[minimum_digits]; // Must be last data item, see "struct hack"

    magnitude_t(std::size_t max_len = minimum_digits, bool ro = false):
        copy_count(0), readonly(ro), max_length(max_len) { }
    magnitude_t(std::size_t max_len, bool ro, digit_t init): copy_count(0),
        readonly(ro), max_length(max_len) { digits[0] = init; }
    std::size_t copies() const { return copy_count; }

    void inc() { ++copy_count; }
    bool dec() { return (--copy_count == 0); }
};

inline std::size_t bits_to_digits(std::size_t bits) { return (bits +
    bits_per_digit - 1) / bits_per_digit; }

//! @endcond detail
} // namespace detail
} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_BASIC_TYPES_HPP
