
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

#include "../boost/xint/xint.hpp"
#include <memory> // for auto_ptr

namespace boost {
namespace xint {

const std::string detail::nan_text("#NaN#");

////////////////////////////////////////////////////////////////////////////////
// The nothrow_integer class

namespace {
	std::auto_ptr<nothrow_integer> cZero, cOne, cNaN;
}

//! \copydoc xint::integer::integer()
nothrow_integer::nothrow_integer() {
    _attach_0();
}

//! \copydoc integer::integer(const integer&)
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
        return xint::nothrow_integer(*this).hex_digits();
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
        *this=nothrow_integer(xint::nothrow_integer(*this).operator+=(xint::nothrow_integer(addend)));
    } catch (std::exception&) {
        *this=nothrow_integer::nan();
    }
    return *this;
}

nothrow_integer& nothrow_integer::operator-=(const nothrow_integer& subtrahend) {
    try {
        *this=nothrow_integer(xint::nothrow_integer(*this).operator-=(xint::nothrow_integer(subtrahend)));
    } catch (std::exception&) {
        *this=nothrow_integer::nan();
    }
    return *this;
}

nothrow_integer& nothrow_integer::operator=(const nothrow_integer &c) {
    try {
        _attach(c);
    } catch (std::exception&) {
        _attach(0);
    }
    return *this;
}

nothrow_integer& nothrow_integer::operator*=(const nothrow_integer& b) { *this=multiply(*this, b); return *this; }
nothrow_integer& nothrow_integer::operator/=(const nothrow_integer& b) { *this=divide(*this, b); return *this; }
nothrow_integer& nothrow_integer::operator%=(const nothrow_integer& b) { *this=mod(*this, b); return *this; }

nothrow_integer& nothrow_integer::operator++() { *this += one(); return *this; }
nothrow_integer& nothrow_integer::operator--() { *this -= one(); return *this; }
nothrow_integer  nothrow_integer::operator++(int) { nothrow_integer s=*this; *this += one(); return s; }
nothrow_integer  nothrow_integer::operator--(int) { nothrow_integer s=*this; *this -= one(); return s; }

nothrow_integer  nothrow_integer::operator<<(size_t shift) const { return shift_left(*this, shift); }
nothrow_integer  nothrow_integer::operator>>(size_t shift) const { return shift_right(*this, shift); }
nothrow_integer& nothrow_integer::operator&=(const nothrow_integer& n) { *this=bitwise_and(*this, n); return *this; }
nothrow_integer& nothrow_integer::operator|=(const nothrow_integer& n) { *this=bitwise_or(*this, n); return *this; }
nothrow_integer& nothrow_integer::operator^=(const nothrow_integer& n) { *this=bitwise_xor(*this, n); return *this; }

nothrow_integer& nothrow_integer::operator<<=(size_t shift) {
    try {
        *this=nothrow_integer(xint::nothrow_integer(*this).operator<<=(shift));
    } catch (std::exception&) {
        *this=nothrow_integer::nan();
    }
    return *this;
}

nothrow_integer& nothrow_integer::operator>>=(size_t shift) {
    try {
        *this=nothrow_integer(xint::nothrow_integer(*this).operator>>=(shift));
    } catch (std::exception&) {
        *this=nothrow_integer::nan();
    }
    return *this;
}

const nothrow_integer& nothrow_integer::zero() {
    if (cZero.get()==0) cZero.reset(new nothrow_integer(xint::nothrow_integer::zero()));
    return *cZero;
}

const nothrow_integer& nothrow_integer::one() {
    if (cOne.get()==0) cOne.reset(new nothrow_integer(xint::nothrow_integer::one()));
    return *cOne;
}

const nothrow_integer& nothrow_integer::nan() {
    if (cNaN.get()==0) {
        nothrow_integer *n=new nothrow_integer();
        n->_attach(0);
        cNaN.reset(n);
    }
    return *cNaN;
}

void nothrow_integer::_make_unique() {
    try {
        _base_make_unique();
    } catch (std::exception&) {
        _attach(0); // Turns it into a Not-a-Number.
    }
}

} // namespace xint
} // namespace boost
