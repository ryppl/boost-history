
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the definitions for the integer member functions.
*/

#include "../xint.hpp"
#include "../xint_data_t.hpp"

#ifdef XINT_THREADSAFE
    #define XINT_DISABLE_COPY_ON_WRITE
#endif

namespace xint {

const integer *integer::cZero=0, *integer::cOne=0;
const std::string detail::nan_text("#NaN#");

integer::integer() {
    _init();
}

integer::integer(const integer& b) {
    if (b.nan()) data=0;
    else _init(b);
}

integer::integer(const std::string& str, size_t base) {
    _init(from_string(str, base));
}

integer::integer(const not_a_number&) {
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

bool integer::odd() const {
    _throw_if_nan();
    return ((_get_digit(0) & 0x01)==1);
}

bool integer::even() const {
    _throw_if_nan();
    return ((_get_digit(0) & 0x01)==0);
}

int integer::sign() const {
    _throw_if_nan();
    if (data->mIsNegative) return -1;
    if (_get_length()==1 && _get_digit(0)==0) return 0;
    return 1;
}

bool integer::nan() const {
    return (data==0);
}

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
    if (cZero==0) cZero=new integer(0);
    return *cZero;
}

const integer& integer::one() {
    if (cOne==0) cOne=new integer(1);
    return *cOne;
}

detail::digit_t integer::_get_digit(size_t index) const {
    return data->digits[index];
}

size_t integer::_get_length() const {
    return data->mLength;
}

void integer::_throw_if_nan() const {
    if (nan()) throw not_a_number();
}

} // namespace xint
