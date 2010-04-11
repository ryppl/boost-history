
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
    \brief Contains the definitions for the xint::integer member functions.
*/

#include "../boost/xint/xint.hpp"
#include "../boost/xint/xint_data_t.hpp"

#ifdef XINT_THREADSAFE
    #define XINT_DISABLE_COPY_ON_WRITE
#endif

namespace boost {
namespace xint {
namespace core {

namespace {
	std::auto_ptr<integer> cZero, cOne;
}

////////////////////////////////////////////////////////////////////////////////
// The base_integer class

base_integer::~base_integer() {
    if (data && data->detach()) delete data;
}

detail::digit_t base_integer::_get_digit(size_t index) const {
    return data->digits[index];
}

detail::digit_t base_integer::_get_digit(size_t index, bool) const {
    if (index >= data->mLength) return 0;
    return data->digits[index];
}

size_t base_integer::_get_length() const {
    return data->mLength;
}

void base_integer::_attach(detail::data_t *new_data) {
    if (data && data->detach()) delete data;
    #ifdef XINT_DISABLE_COPY_ON_WRITE
        if (new_data==0 || new_data->mCopies==0) data=new_data;
        else {
            try {
                data=new detail::data_t(new_data);
            } catch (std::bad_alloc&) {
                throw xint::overflow_error("Out of memory allocating xint::integer");
            }
        }
    #else
        data=new_data;
    #endif
    if (data) data->attach();
}

void base_integer::_attach(const base_integer& copy) {
    _attach(copy.data);
}

bool base_integer::_is_unique() const {
    return (!data || data->mCopies==1);
}

////////////////////////////////////////////////////////////////////////////////
// The core integer class

//! \brief Creates a new integer with an initial value of zero.
integer::integer() {
    _init();
}

/*! \brief Creates a copy of an existing integer.

\param[in] b An existing integer. If passed a Not-a-Number, it will create
another Not-a-Number.

\note
This library can use a \link cow copy-on-write technique\endlink, making
copying (even of large numbers) a very inexpensive operation.

\overload
*/
integer::integer(const integer& b) {
    _init(b);
}

/*! \brief Creates an integer from a blockable::integer.

\param[in] b An existing blockable::integer.

\note
This library can use a \link cow copy-on-write technique\endlink, making
copying (even of large numbers) a very inexpensive operation.

\overload
*/
integer::integer(const blockable::integer& b) {
    if (b.is_nan()) throw xint::not_a_number();
    _attach(b);
}

/*! \brief Create an integer from a string representation.

\param[in] str A string representation of a number.
\param[in] base The base of the number, or xint::autobase.

\exception xint::invalid_base if the base parameter is not between 2 and 36
(inclusive) or the constant xint::autobase.
\exception xint::invalid_digit if the string contains any digit that cannot be
part of a number in the specified base, or if there are no valid digits.

\remarks
This will convert a string representation of a number into an integer. See the
description of the xint::from_string function for details on its behavior.

\par
This is the most common way to initialize values that are too large to fit into
a native integral type.

\overload
*/
integer::integer(const std::string& str, size_t base) {
    _init(from_string(str, base));
}

void integer::_init(detail::digit_t init) {
    try {
        _attach(new detail::data_t(init));
    } catch (std::bad_alloc&) {
        throw xint::overflow_error("Out of memory allocating xint::integer");
    }
}

void integer::_init(const integer &c) {
    _attach(c);
}

void integer::_init(boost::uintmax_t n) {
    try {
        _attach(new detail::data_t(n, true));
    } catch (std::bad_alloc&) {
        throw xint::overflow_error("Out of memory allocating xint::integer");
    }
}

void integer::_make_unique() {
    try {
        if (!_is_unique()) _attach(new detail::data_t(_get_data()));
    } catch (std::bad_alloc&) {
        throw xint::overflow_error("Out of memory allocating xint::integer");
    }
}

void integer::_set_negative(bool negative) {
    if (_get_data()->mIsNegative != negative) {
        _make_unique();
        detail::data_t *p=_get_data();
        p->mIsNegative=negative;
        p->skipLeadingZeros();
    }
}

/*! \brief Tests the lowest bit of \c *this to determine oddness.

\returns \c true if \c *this is odd, otherwise \c false.
*/
bool integer::odd() const {
    return ((_get_digit(0) & 0x01)==1);
}

/*! \brief Tests the lowest bit of \c *this to determine evenness.

\returns \c true if \c *this is even, otherwise \c false.
*/
bool integer::even() const {
    return ((_get_digit(0) & 0x01)==0);
}

/*! \brief Tests the sign of \c *this.

\returns -1 if \c *this is negative, 0 if it's zero, or 1 if it's greater than
zero.
*/
int integer::sign() const {
    if (_get_length()==1 && _get_digit(0)==0) return 0;
    else return (_get_data()->mIsNegative ? -1 : 1);
}

/*! \brief Tells you roughly how large an integer is.

\returns The number of hexadecimal digits that would be required to encode \c
*this.
*/
size_t integer::hex_digits() const {
    size_t bits=log2(*this);
    return (bits+3)/4; // Four bits per hex digit, rounded up
}

integer& integer::operator+=(const integer& addend) {
    if ((sign() < 0) == (addend.sign() < 0)
        && _get_length() >= addend._get_length())
    {
        // Fast in-place add
        _make_unique();
        _get_data()->add(addend._get_data());
    } else {
        // This works for all cases
        *this=add(*this, addend);
    }
    return *this;
}

integer& integer::operator-=(const integer& subtrahend) {
    if (sign() >= 0 && subtrahend.sign() >= 0 && *this >= subtrahend) {
        // Fast in-place subtract
        _make_unique();
        _get_data()->subtract(subtrahend._get_data());
    } else {
        // This works for all cases
        *this=subtract(*this, subtrahend);
    }
    return *this;
}

integer& integer::operator=(const integer &c) {
    _attach(c);
    return *this;
}

integer& integer::operator*=(const integer& b) { *this=multiply(*this, b); return *this; }
integer& integer::operator/=(const integer& b) { *this=divide(*this, b); return *this; }
integer& integer::operator%=(const integer& b) { *this=mod(*this, b); return *this; }

integer& integer::operator++() { *this += one(); return *this; }
integer& integer::operator--() { *this -= one(); return *this; }
integer  integer::operator++(int) { integer s=*this; *this += one(); return s; }
integer  integer::operator--(int) { integer s=*this; *this -= one(); return s; }

integer  integer::operator<<(size_t shift) const { return shift_left(*this, shift); }
integer  integer::operator>>(size_t shift) const { return shift_right(*this, shift); }
integer& integer::operator&=(const integer& n) { *this=bitwise_and(*this, n); return *this; }
integer& integer::operator|=(const integer& n) { *this=bitwise_or(*this, n); return *this; }
integer& integer::operator^=(const integer& n) { *this=bitwise_xor(*this, n); return *this; }

integer& integer::operator<<=(size_t shift) {
    if (shift>0) {
        _make_unique();
        _get_data()->shift_left(shift);
    }
    return *this;
}

integer& integer::operator>>=(size_t shift) {
    if (shift>0) {
        _make_unique();
        _get_data()->shift_right(shift);
    }
    return *this;
}

const integer& integer::zero() {
    if (cZero.get()==0) cZero.reset(new integer(0));
    return *cZero;
}

const integer& integer::one() {
    if (cOne.get()==0) cOne.reset(new integer(1));
    return *cOne;
}

} // namespace core
} // namespace xint
} // namespace boost
