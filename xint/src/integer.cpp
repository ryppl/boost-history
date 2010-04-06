
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

namespace xint {

namespace {
	std::auto_ptr<integer> cZero, cOne;
}

const std::string detail::nan_text("#NaN#");

//! \brief Creates a new integer with an initial value of zero.
integer::integer() {
    _init();
}

/*! \brief Creates a copy of an existing integer.

\param[in] b An existing integer. If passed a Not-a-Number, it will create
another Not-a-Number.

\note
This library can use a \link cow copy-on-write technique\endlink, making copying
(even of large numbers) a very inexpensive operation.

\overload
*/
integer::integer(const integer& b) {
    if (b.is_nan()) data=0;
    else _init(b);
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

/*! \brief Creates an integer with the value of Not-a-Number.

\param[in] n An xint::not_a_number object (normally used as an exception class).

\remarks
This is the official way to create an integer with the Not-a-Number value, if
you ever wish to use that value for your own purposes.

\see \ref nan

\overload
*/
integer::integer(const not_a_number& n) {
    data=0;
}

integer::~integer() {
    _detach();
}

void integer::_init(detail::digit_t init) {
    try {
        data=new detail::data_t(init);
    } catch (std::bad_alloc&) {
        throw xint::overflow_error("Out of memory allocating xint::integer");
    }
    _attach();
}

void integer::_init(const integer &c) {
    #ifdef XINT_DISABLE_COPY_ON_WRITE
        data=(c.data ? new detail::data_t(c.data) : 0);
    #else
        data=c.data;
    #endif
    _attach();
}

void integer::_init(boost::uintmax_t n) {
    static int bits=std::numeric_limits<boost::uintmax_t>::digits;
    static int maxDigits=(bits+detail::bits_per_digit-1)/detail::bits_per_digit;

    try {
        data=new detail::data_t;
    } catch (std::bad_alloc&) {
        throw xint::overflow_error("Out of memory allocating xint::integer");
    }
    _attach();

    data->alloc(maxDigits);
    for (int x=0; n != 0; ++x) {
        data->digits[x]=detail::digit_t(n & detail::digit_mask);
        n >>= detail::bits_per_digit;
    }
    data->skipLeadingZeros();
}

void integer::_attach() {
    if (data) data->attach();
}

void integer::_detach() {
    if (data && data->detach()) delete data;
}

void integer::_make_unique() {
    try {
        if (data && data->mCopies > 1) {
            detail::data_t *newstore=new detail::data_t(data);
            _detach();
            data=newstore;
            _attach();
        }
    } catch (std::bad_alloc&) {
        throw xint::overflow_error("Out of memory allocating xint::integer");
    }
}

void integer::_set_negative(bool negative) {
    if (negative != (sign() < 0)) *this=negate(*this);
}

/*! \brief Tests the lowest bit of \c *this to determine oddness.

\returns \c true if \c *this is odd, otherwise \c false.
*/
bool integer::odd() const {
    _throw_if_nan();
    return ((_get_digit(0) & 0x01)==1);
}

/*! \brief Tests the lowest bit of \c *this to determine evenness.

\returns \c true if \c *this is even, otherwise \c false.
*/
bool integer::even() const {
    _throw_if_nan();
    return ((_get_digit(0) & 0x01)==0);
}

/*! \brief Tests the sign of \c *this.

\returns -1 if \c *this is negative, 0 if it's zero, or 1 if it's greater than
zero.
*/
int integer::sign() const {
    _throw_if_nan();
    if (data->mIsNegative) return -1;
    if (_get_length()==1 && _get_digit(0)==0) return 0;
    return 1;
}

/*! \brief Tests \c *this for Not-a-Number.

\returns \c true if \c *this is Not-a-Number, otherwise \c false.

\see \ref nan
*/
bool integer::is_nan() const {
    return (data==0);
}

/*! \brief Tells you roughly how large an integer is.

\returns The number of hexadecimal digits that would be required to encode \c
*this.
*/
size_t integer::hex_digits() const {
    _throw_if_nan();
    size_t bits=log2(*this);
    return (bits+3)/4; // Four bits per hex digit, rounded up
}

integer& integer::operator+=(const integer& addend) {
    if ((sign() < 0) == (addend.sign() < 0)
        && data->mLength >= addend.data->mLength)
    {
        // Fast in-place add
        _make_unique();
        data->add(addend.data);
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
        data->subtract(subtrahend.data);
    } else {
        // This works for all cases
        *this=subtract(*this, subtrahend);
    }
    return *this;
}

integer& integer::operator=(const integer &c) {
    _detach();
    #ifdef XINT_DISABLE_COPY_ON_WRITE
        data=(c.data ? new detail::data_t(c.data) : 0);
    #else
        data=c.data;
    #endif
    _attach();
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
        data->shift_left(shift);
    }
    return *this;
}

integer& integer::operator>>=(size_t shift) {
    if (shift>0) {
        _make_unique();
        data->shift_right(shift);
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

detail::digit_t integer::_get_digit(size_t index) const {
    return data->digits[index];
}

detail::digit_t integer::_get_digit(size_t index, bool) const {
    if (index >= data->mLength) return 0;
    return data->digits[index];
}

size_t integer::_get_length() const {
    return data->mLength;
}

void integer::_throw_if_nan() const {
    if (is_nan()) throw not_a_number();
}

} // namespace xint
