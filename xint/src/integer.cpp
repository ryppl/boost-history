
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

#include "../boost/xint/integer.hpp"
#include "../boost/xint/nothrow_integer.hpp"
#include <memory> // for auto_ptr

namespace boost {
namespace xint {

std::auto_ptr<integer> integer::cZero, integer::cOne;

//! \brief Creates a new integer with an initial value of zero.
integer::integer() {
    _attach_0();
}

/*! \brief Creates a copy of an existing integer.

\param[in] b An existing integer.

\note
This library can use a \ref cow "copy-on-write technique", making copying (even
of large numbers) a very inexpensive operation.

\overload
*/
integer::integer(const integer& b) {
    _attach(b);
}

/*! \brief Creates an integer from a nothrow_integer.

\param[in] b An existing nothrow_integer.

\note
This library can use a \ref cow "copy-on-write technique", making copying (even
of large numbers) a very inexpensive operation.

\overload
*/
integer::integer(const nothrow_integer& b) {
    if (b.is_nan()) throw exceptions::not_a_number();
    _attach(b);
}

/*! \brief Creates an integer from any fixed_integer type.

\param[in] b An existing fixed_integer.

\overload
*/
integer::integer(const fixed_integer_any& b) {
    _attach(b);
}

/*! \brief Create an integer from a string representation.

- Complexity: O(n<sup>2</sup>)

\param[in] str A string representation of a number.
\param[in] base The base of the number, or xint::autobase.

\exception exceptions::invalid_base if the base parameter is not between 2 and 36
(inclusive) or the constant xint::autobase.
\exception exceptions::invalid_digit if the string contains any digit that cannot
be part of a number in the specified base, or if there are no valid digits.

\remarks
This will convert a string representation of a number into an integer. See the
description of the \ref xint::from_string "from_string" function for details on
its behavior.

\par
This is the most common way to initialize values that are too large to fit into
a native integral type.

\overload
*/
integer::integer(const std::string& str, size_t base) {
    detail::from_string(*this, str, base);
}

/*! \brief Tests the lowest bit of \c *this to determine oddness.

- Complexity: O(1)

\returns \c true if \c *this is odd, otherwise \c false.
*/
bool integer::odd() const {
    return ((_get_digit(0) & 0x01)==1);
}

/*! \brief Tests the lowest bit of \c *this to determine evenness.

- Complexity: O(1)

\returns \c true if \c *this is even, otherwise \c false.
*/
bool integer::even() const {
    return ((_get_digit(0) & 0x01)==0);
}

/*! \brief Tests the sign of \c *this.

- Complexity: O(1)

\param[in] signed_zero If \c false (the default value), returns zero if \c *this
is zero. If \c true, returns 1 or -1 on a zero \c *this as well. Primarily used
to identify a \ref zero "negative zero".

\returns -1 if \c *this is negative, 0 if it's zero, or 1 if it's greater than
zero.
*/
int integer::sign(bool signed_zero) const {
    if (!signed_zero && _is_zero()) return 0;
    else return (_get_negative() ? -1 : 1);
}

/*! \brief Tells you roughly how large \c *this is.

- Complexity: O(1)

\returns The number of hexadecimal digits that would be required to encode \c
*this.
*/
size_t integer::hex_digits() const {
    size_t bits=_log2();
    return (bits+3)/4; // Four bits per hex digit, rounded up
}

integer& integer::operator+=(const integer& addend) {
    detail::add(*this, *this, addend);
    return *this;
}

integer& integer::operator-=(const integer& subtrahend) {
    detail::subtract(*this, *this, subtrahend);
    return *this;
}

integer& integer::operator=(BOOST_XINT_COPY_ASSIGN_REF(integer) c) {
    _attach(c);
    return *this;
}

integer  integer::operator-() const { return negate(*this); }

integer& integer::operator*=(const integer& b) { detail::multiply(*this, *this, b); return *this; }
integer& integer::operator/=(const integer& b) { detail::divide(*this, *this, b); return *this; }
integer& integer::operator%=(const integer& b) { detail::mod(*this, *this, b); return *this; }

integer& integer::operator++() { _increment(); return *this; }
integer& integer::operator--() { _decrement(); return *this; }
integer  integer::operator++(int) { integer s=*this; _increment(); return BOOST_XINT_MOVE(s); }
integer  integer::operator--(int) { integer s=*this; _decrement(); return BOOST_XINT_MOVE(s); }

integer  integer::operator<<(size_t shift) const { return shift_left(*this, shift); }
integer  integer::operator>>(size_t shift) const { return shift_right(*this, shift); }
integer& integer::operator&=(const integer& n) { detail::bitwise_and(*this, *this, n); return *this; }
integer& integer::operator|=(const integer& n) { detail::bitwise_or(*this, *this, n); return *this; }
integer& integer::operator^=(const integer& n) { detail::bitwise_xor(*this, *this, n); return *this; }

integer& integer::operator<<=(size_t shift) {
    detail::shift_left(*this, *this, shift);
    return *this;
}

integer& integer::operator>>=(size_t shift) {
    detail::shift_right(*this, *this, shift);
    return *this;
}

const integer& integer::zero() {
    if (cZero.get()==0) {
        cZero.reset(new integer(0));
        cZero->_set_readonly();
    }
    return *cZero;
}

const integer& integer::one() {
    if (cOne.get()==0) {
        cOne.reset(new integer(1));
        cOne->_set_readonly();
    }
    return *cOne;
}

} // namespace xint
} // namespace boost
