
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
    \brief Contains the definitions for the \c nothrow_integer member functions.
*/

#include "../boost/xint/nothrow_integer.hpp"
#include <memory> // for auto_ptr

namespace boost {
namespace xint {

//! @cond detail
const std::string detail::nan_text("#NaN#");
//! @endcond detail

////////////////////////////////////////////////////////////////////////////////
// The nothrow_integer class

std::auto_ptr<nothrow_integer> nothrow_integer::cZero, nothrow_integer::cOne,
    nothrow_integer::cNaN;

//! \copydoc xint::integer::integer()
nothrow_integer::nothrow_integer() {
    _attach_0();
}

/*! \brief Creates a copy of an existing integer.

\param[in] b An existing integer. If passed a Not-a-Number, it will create
another Not-a-Number.

\note
This library can use a \ref cow "copy-on-write technique", making copying (even
of large numbers) a very inexpensive operation.

\overload
*/
nothrow_integer::nothrow_integer(const nothrow_integer& b) {
    try {
        if (!b.is_nan()) _attach(b);
    } catch (std::exception&) {
        // Do nothing, automatically sets to Not-a-Number
    }
}

/*! \brief Creates a nothrow_integer from an integer.

\param[in] b An existing integer.

\note
This library can use a \ref cow "copy-on-write technique", making copying (even
of large numbers) a very inexpensive operation.

\overload
*/
nothrow_integer::nothrow_integer(const integer& b) {
    try {
        _attach(b);
    } catch (std::exception&) {
        // Do nothing, automatically sets to Not-a-Number
    }
}

/*! \brief Creates a nothrow_integer from any fixed_integer type.

\param[in] b An existing fixed_integer.

\overload
*/
nothrow_integer::nothrow_integer(const fixed_integer_any& b) {
    try {
        _attach(b);
    } catch (std::exception&) {
        // Do nothing, automatically sets to Not-a-Number
    }
}

//! \copydoc xint::integer::integer(const std::string&, size_t)
nothrow_integer::nothrow_integer(const std::string& str, size_t base) {
    try {
        _attach(from_string(str, base));
    } catch (std::exception&) {
        // Do nothing, automatically sets to Not-a-Number
    }
}

/*! \copydoc xint::integer::odd

\note Returns \c false instead of throwing.
*/
bool nothrow_integer::odd() const {
    if (is_nan()) return false;
    return ((_get_digit(0) & 0x01)==1);
}

/*! \copydoc xint::integer::even

\note Returns \c false instead of throwing.
*/
bool nothrow_integer::even() const {
    if (is_nan()) return false;
    return ((_get_digit(0) & 0x01)==0);
}

/*! \copydoc xint::integer::sign

\note Returns zero instead of throwing.
*/
int nothrow_integer::sign(bool signed_zero) const {
    if (is_nan()) return 0;
    if (!signed_zero && _is_zero()) return 0;
    else return (_get_negative() ? -1 : 1);
}

/*! \copydoc xint::integer::hex_digits

\note Returns zero instead of throwing.
*/
size_t nothrow_integer::hex_digits() const {
    try {
        size_t bits=_log2();
        return (bits+3)/4; // Four bits per hex digit, rounded up
    } catch (std::exception&) {
        return 0;
    }
}

/*! \brief Tests \c *this for Not-a-Number.

\returns \c true if \c *this is Not-a-Number, otherwise \c false.

\see \ref nan
*/
bool nothrow_integer::is_nan() const {
    return _is_nan();
}

nothrow_integer& nothrow_integer::operator+=(const nothrow_integer& addend) {
    try {
        detail::add(*this, *this, addend);
    } catch (std::exception&) {
        *this=nothrow_integer::nan();
    }
    return *this;
}

nothrow_integer& nothrow_integer::operator-=(const nothrow_integer& subtrahend) {
    try {
        detail::subtract(*this, *this, subtrahend);
    } catch (std::exception&) {
        *this=nothrow_integer::nan();
    }
    return *this;
}

nothrow_integer& nothrow_integer::operator=(BOOST_XINT_COPY_ASSIGN_REF(
    nothrow_integer) c)
{
    try {
        _attach(c);
    } catch (std::exception&) {
        _base_attach(0);
    }
    return *this;
}

nothrow_integer  nothrow_integer::operator-() const { return negate(*this); }

nothrow_integer& nothrow_integer::operator*=(const nothrow_integer& b) { detail::multiply(*this, *this, b); return *this; }
nothrow_integer& nothrow_integer::operator/=(const nothrow_integer& b) { detail::divide(*this, *this, b); return *this; }
nothrow_integer& nothrow_integer::operator%=(const nothrow_integer& b) { detail::mod(*this, *this, b); return *this; }

nothrow_integer& nothrow_integer::operator++() { _make_unique(true); _increment(); return *this; }
nothrow_integer& nothrow_integer::operator--() { _make_unique(true); _decrement(); return *this; }
nothrow_integer  nothrow_integer::operator++(int) { nothrow_integer s=*this; _make_unique(true); _increment(); return BOOST_XINT_MOVE(s); }
nothrow_integer  nothrow_integer::operator--(int) { nothrow_integer s=*this; _make_unique(true); _decrement(); return BOOST_XINT_MOVE(s); }

nothrow_integer  nothrow_integer::operator<<(size_t shift) const { return shift_left(*this, shift); }
nothrow_integer  nothrow_integer::operator>>(size_t shift) const { return shift_right(*this, shift); }
nothrow_integer& nothrow_integer::operator&=(const nothrow_integer& n) { detail::bitwise_and(*this, *this, n); return *this; }
nothrow_integer& nothrow_integer::operator|=(const nothrow_integer& n) { detail::bitwise_or(*this, *this, n); return *this; }
nothrow_integer& nothrow_integer::operator^=(const nothrow_integer& n) { detail::bitwise_xor(*this, *this, n); return *this; }

nothrow_integer& nothrow_integer::operator<<=(size_t shift) {
    try {
        detail::shift_left(*this, *this, shift);
    } catch (std::exception&) {
        *this=nothrow_integer::nan();
    }
    return *this;
}

nothrow_integer& nothrow_integer::operator>>=(size_t shift) {
    try {
        detail::shift_right(*this, *this, shift);
    } catch (std::exception&) {
        *this=nothrow_integer::nan();
    }
    return *this;
}

const nothrow_integer& nothrow_integer::zero() {
    if (cZero.get()==0) {
        cZero.reset(new nothrow_integer(integer::zero()));
        cZero->_set_readonly();
    }
    return *cZero;
}

const nothrow_integer& nothrow_integer::one() {
    if (cOne.get()==0) {
        cOne.reset(new nothrow_integer(integer::one()));
        cOne->_set_readonly();
    }
    return *cOne;
}

const nothrow_integer& nothrow_integer::nan() {
    if (cNaN.get()==0) {
        cNaN.reset(new nothrow_integer());
        cNaN->_base_attach(0);
        cNaN->_set_readonly();
    }
    return *cNaN;
}

void nothrow_integer::_make_unique(bool) {
    try {
        detail::base_integer::_make_unique();
    } catch (std::exception&) {
        _base_attach(0); // Turns it into a Not-a-Number.
    }
}

} // namespace xint
} // namespace boost
