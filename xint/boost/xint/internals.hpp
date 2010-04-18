
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

#include <string>
#include <limits>
#include <cstddef> // for size_t
#include <iostream>
#include <stdexcept>
#include <boost/integer.hpp>
#include <boost/cstdint.hpp>
#include <boost/function.hpp>

namespace boost {

//! \brief All the types and functions of the library are within this namespace.
namespace xint {

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

//! \brief Holds the magnitude digits of an integer.
template <int digit_count=1>
struct data_t {
    size_t copies, length, max_length;
    digit_t magnitude[digit_count];
};

//! \name The variable-length data_t functions.
//!@{
data_t<> *allocate(size_t max_digits, size_t initial_length=
    (std::numeric_limits<size_t>::max)());
data_t<> *reallocate(data_t<> *olddata, size_t newsize);
data_t<> *duplicate(data_t<> *olddata, size_t extra=0);
data_t<> *duplicate(data_t<> *olddata, data_t<> *data_to_copy, size_t extra=0);
void deallocate(data_t<> *data);
//!@}

//! \brief The base class for the integer classes.
class base_integer {
    public:
    ~base_integer();

    //! \name Internal Functions
    //@{
    detail::digit_t _get_digit(size_t index) const;
    detail::digit_t _get_digit(size_t index, bool) const; //!< \overload
    detail::digit_t* _get_digits();
    const detail::digit_t* _get_digits() const; //!< \overload
    size_t _get_length() const;
    bool _is_unique() const;
    bool _is_zero() const;
    bool _get_negative() const;
    void _set_negative(bool neg);
    void _cleanup();
    void _realloc(size_t new_digit_count);
    void _duplicate(const base_integer& copy, size_t extra);
    void _shift_left(size_t byBits);
    void _shift_right(size_t byBits);
    void _add(const base_integer& n);
    void _subtract(const base_integer& n);
    //@}

    protected:
    base_integer(): fixed_mask(0), negative(false), data(0) { }
    base_integer(size_t count, digit_t mask, void *d);

    bool _is_nan() const { return (data==0); };
    void _base_make_unique();
    void _attach(data_t<> *new_data, bool neg=false);
    void _attach(const base_integer& copy);
    void _attach_0();
    void _attach_1(digit_t n);
    void _attach_n(boost::uintmax_t n);
    void _set_n(boost::uintmax_t n);

    private:
    digit_t fixed_mask; //!< For the topmost digit; zero for variable-sized

    bool negative;
    data_t<> *data;
};

} // namespace detail

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
class nothrow_integer;
template <size_t bits> class fixed_integer;

//! \name Miscellaneous functions
//!@{
bool opt_secure_mode();
bool opt_thread_safe();
//!@}

} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_INTERNALS_HPP
