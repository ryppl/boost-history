
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
    \brief Contains the definitions for the xint::blockable::integer member
           functions.
*/

#include "../boost/xint/xint.hpp"
#include "../boost/xint/xint_data_t.hpp"

namespace boost {
namespace xint {

const std::string detail::nan_text("#NaN#");

namespace blockable {

////////////////////////////////////////////////////////////////////////////////
// The blockable integer class

namespace {
	std::auto_ptr<integer> cZero, cOne, cNaN;
}

//! \copydoc xint::core::integer::integer()
integer::integer() {
    _init();
}

//! \copydoc xint::core::integer::integer(const core::integer&)
integer::integer(const integer& b) {
    if (!b.is_nan()) _init(b);
}

/*! \brief Creates an integer from a core::integer.

\param[in] b An existing core::integer.

\note
This library can use a \link cow copy-on-write technique\endlink, making
copying (even of large numbers) a very inexpensive operation.

\overload
*/
integer::integer(const core::integer& b) {
    _attach(b);
}

//! \copydoc xint::core::integer::integer(const std::string&, size_t)
integer::integer(const std::string& str, size_t base) {
    try {
        _init(from_string(str, base));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
    }
}

/*! \copydoc core::integer::odd

\note If exceptions are blocked, returns false instead of throwing.
*/
bool integer::odd() const {
    if (is_nan()) {
        if (exceptions_allowed()) throw not_a_number();
        else return false;
    }
    return ((_get_digit(0) & 0x01)==1);
}

/*! \copydoc core::integer::even

\note If exceptions are blocked, returns false instead of throwing.
*/
bool integer::even() const {
    if (is_nan()) {
        if (exceptions_allowed()) throw not_a_number();
        else return false;
    }
    return ((_get_digit(0) & 0x01)==0);
}

/*! \copydoc core::integer::sign

\note If exceptions are blocked, returns zero instead of throwing.
*/
int integer::sign() const {
    if (is_nan()) {
        if (exceptions_allowed()) throw not_a_number();
        else return 0;
    }
    if (_get_length()==1 && _get_digit(0)==0) return 0;
    return (_get_data()->mIsNegative ? -1 : 1);
}

/*! \copydoc core::integer::hex_digits

\note If exceptions are blocked, returns zero instead of throwing.
*/
size_t integer::hex_digits() const {
    try {
        return core::integer(*this).hex_digits();
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return 0;
    }
}

/*! \brief Tests \c *this for Not-a-Number.

\returns \c true if \c *this is Not-a-Number, otherwise \c false.

\see \ref nan
*/
bool integer::is_nan() const {
    return (_get_data()==0);
}

integer& integer::operator+=(const integer& addend) {
    try {
        *this=integer(core::integer(*this).operator+=(core::integer(addend)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        *this=integer::nan();
    }
    return *this;
}

integer& integer::operator-=(const integer& subtrahend) {
    try {
        *this=integer(core::integer(*this).operator-=(core::integer(subtrahend)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        *this=integer::nan();
    }
    return *this;
}

integer& integer::operator=(const integer &c) {
    try {
        _attach(c);
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        _attach(0);
    }
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
    try {
        *this=integer(core::integer(*this).operator<<=(shift));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        *this=integer::nan();
    }
    return *this;
}

integer& integer::operator>>=(size_t shift) {
    try {
        *this=integer(core::integer(*this).operator>>=(shift));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        *this=integer::nan();
    }
    return *this;
}

const integer& integer::zero() {
    if (cZero.get()==0) cZero.reset(new integer(core::integer::zero()));
    return *cZero;
}

const integer& integer::one() {
    if (cOne.get()==0) cOne.reset(new integer(core::integer::one()));
    return *cOne;
}

const integer& integer::nan() {
    if (cNaN.get()==0) {
        integer *n=new integer();
        n->_attach(0);
        cNaN.reset(n);
    }
    return *cNaN;
}

void integer::_init(detail::digit_t init) {
    try {
        _attach(new detail::data_t(init));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
    }
}

void integer::_init(const integer &c) {
    try {
        _attach(c);
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
    }
}

void integer::_init(boost::uintmax_t n) {
    try {
        _attach(new detail::data_t(n));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
    }
}

void integer::_make_unique() {
    if (!_is_unique()) {
        try {
            _attach(new detail::data_t(_get_data()));
        } catch (std::exception&) {
            if (exceptions_allowed()) throw;
            _attach(0); // Turns it into a Not-a-Number,
        }
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

} // namespace blockable
} // namespace xint
} // namespace boost
