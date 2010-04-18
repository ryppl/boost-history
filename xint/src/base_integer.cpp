
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
    \brief Contains the definitions for the \c base_integer member functions.
*/

#include "../boost/xint/xint.hpp"

#ifdef XINT_THREADSAFE
    #define XINT_DISABLE_COPY_ON_WRITE
#endif

namespace boost {
namespace xint {
namespace detail {

namespace {
struct zerodata_t: public data_t<> {
    public:
    zerodata_t() { copies=1; length=max_length=1; *magnitude=0; }
} zerodata;
} // namespace

base_integer::base_integer(size_t count, digit_t mask, void *d):
    fixed_mask(mask), negative(false), data(reinterpret_cast<data_t<>*>(d))
{
    assert(mask != 0);

    data->copies=1;
    data->length=1;
    data->max_length=count;

    digit_t *i = data->magnitude, *ie = i + data->max_length;
    while (i != ie) *i++ = 0;
}

base_integer::~base_integer() {
    if (fixed_mask != 0 && data && data->copies == 1) deallocate(data);
}

digit_t base_integer::_get_digit(size_t index) const {
    return data->magnitude[index];
}

digit_t base_integer::_get_digit(size_t index, bool) const {
    if (index >= data->length) return 0;
    return data->magnitude[index];
}

digit_t* base_integer::_get_digits() {
    return data->magnitude;
}

const digit_t* base_integer::_get_digits() const {
    return data->magnitude;
}

size_t base_integer::_get_length() const {
    return data->length;
}

void base_integer::_duplicate(const base_integer& c, size_t extra) {
    negative=c.negative;
    data=duplicate(data, c.data, extra);
}

void base_integer::_attach(data_t<> *new_data, bool neg) {
    assert(fixed_mask == 0);

    #ifdef XINT_DISABLE_COPY_ON_WRITE
        data=duplicate(data, new_data);
    #else
        if (data && --data->copies == 0) deallocate(data);
        data=new_data;
        if (data) ++data->copies;
    #endif
    negative=neg;
}

void base_integer::_attach(const base_integer& copy) {
    _attach(copy.data, copy.negative);
}

void base_integer::_attach_0() {
    _attach(&zerodata);
}

void base_integer::_attach_1(digit_t n) {
    if (n == 0) {
        _attach(&zerodata);
    } else {
        _attach(allocate(1));
        *data->magnitude=n;
    }
}

void base_integer::_attach_n(boost::uintmax_t n) {
    static int bits = std::numeric_limits<boost::uintmax_t>::digits;
    static int maxDigits = (bits + bits_per_digit - 1) / bits_per_digit;

    _attach(allocate(maxDigits));
    _set_n(n);
}

void base_integer::_set_n(boost::uintmax_t n) {
    // Assumes that the proper allocation was already done
    data->length = data->max_length;
    digit_t *i = data->magnitude, *ie = i + data->max_length;
    while (n != 0 && i != ie) {
        *i++ = digit_t(n & digit_mask);
        n >>= bits_per_digit;
    }
    _cleanup();
}

bool base_integer::_is_unique() const {
    return (!data || data->copies == 1);
}

bool base_integer::_is_zero() const {
    return (data->length == 1 && *data->magnitude == 0);
}

bool base_integer::_get_negative() const {
    return negative;
}

void base_integer::_set_negative(bool neg) {
    negative=neg;
}

void base_integer::_cleanup() {
    if (fixed_mask) data->magnitude[data->max_length-1] &= fixed_mask;
    digit_t *p = data->magnitude + data->length - 1;
    while (p != data->magnitude && *p == 0) { --data->length; --p; }
    if (data->length == 1 && *data->magnitude == 0) negative = false;

}

void base_integer::_realloc(size_t new_digit_count) {
    assert(fixed_mask == 0);
    data = reallocate(data, new_digit_count);
}

void base_integer::_shift_left(size_t byBits) {
    size_t bytes=byBits / bits_per_digit, bits=byBits % bits_per_digit;
    size_t oldLength = data->length;

    data=reallocate(data, data->length + bytes + 1);

    if (bits != 0) {
        // Handle both bits and bytes in one pass
        digit_t *s = data->magnitude + oldLength - 1, *t = s + bytes + 1;

        *t-- = *s >> (bits_per_digit - bits);
        while (s > data->magnitude) {
            *t = (*s-- << bits);
            *t-- |= (*s >> (bits_per_digit - bits));
        }
        *t = (*s << bits);

        if (bytes != 0) {
            digit_t *t = data->magnitude, *te = t + bytes;
            while (t != te) *t++ = 0;
        }
    } else if (bytes != 0) {
        memmove(data->magnitude + bytes, data->magnitude, sizeof(digit_t) *
            oldLength);
        memset(data->magnitude, 0, sizeof(digit_t) * bytes);
    }
    _cleanup();
}

void base_integer::_shift_right(size_t byBits) {
    size_t bytes=byBits / bits_per_digit, bits=byBits % bits_per_digit,
        bits2 = bits_per_digit - bits;

    if (bytes >= data->length) {
        // Right-shift it into oblivion.
        data->length = 1;
        *data->magnitude = 0;
    } else if (bits != 0) {
        // Handle both bits and bytes in one pass
        digit_t *t = data->magnitude, *s = data->magnitude + bytes,
            *se = data->magnitude + data->length - 1;
        while (s!=se) {
            *t = (*s++ >> bits);
            *t++ |= (*s << bits2);
        }
        *t = (*s >> bits);
        if (bytes != 0) {
            memset(data->magnitude + data->length - bytes, 0, sizeof(digit_t) *
                bytes);
            data->length -= bytes;
        }
        _cleanup();
    } else if (bytes != 0) {
        memmove(data->magnitude, data->magnitude + bytes, sizeof(digit_t) *
            (data->length - bytes));
        memset(data->magnitude + data->length - bytes, 0, sizeof(digit_t) *
            bytes);
        data->length -= bytes;
        _cleanup();
    }
}

void base_integer::_base_make_unique() {
    if (!_is_unique()) data=duplicate(data);
}

void base_integer::_add(const base_integer& n) {
    //assert(mCopies==1);
    //assert(mLength >= addend.mLength);

    // The answer to any addition problem contains, at most, one digit more
    // than the largest addend.
    data=reallocate(data, data->length + 1);

    // Now add the digits, starting at the least-significant digit.
    digit_t carry = 0;
    size_t x = 0;
    for (; x < n.data->length; ++x) {
        doubledigit_t t = doubledigit_t(data->magnitude[x]) +
            n.data->magnitude[x] + carry;
        if (t >= digit_overflowbit) { carry = 1; t -= digit_overflowbit; }
            else carry=0;
        data->magnitude[x] = static_cast<digit_t>(t);
    }

    while (carry) {
        doubledigit_t t = doubledigit_t(data->magnitude[x]) + 1;
        if (t >= digit_overflowbit) { carry = 1; t -= digit_overflowbit; }
            else carry=0;
        data->magnitude[x] = static_cast<digit_t>(t);
        ++x;
    }

    _cleanup();
}

void base_integer::_subtract(const base_integer& n) {
    //assert(mCopies==1);
    //assert(mLength >= subtrahend.mLength);

    // For subtraction, the answer will always be less than or equal to the
    // size of the longest operand, so we've already got enough room.

    // Now subtract the digits, starting at the least-significant one.
    size_t x;
    int borrow = 0;
    doubledigit_t t;
    for (x = 0; x < n.data->length; ++x) {
        t=(data->magnitude[x] + digit_overflowbit) - n.data->magnitude[x] -
            borrow;
        if (t < digit_overflowbit) borrow = 1;
        else { borrow = 0; t -= digit_overflowbit; }
        data->magnitude[x]=static_cast<digit_t>(t);
    }

    for (; x < data->length && borrow; ++x) {
        t = (data->magnitude[x] + digit_overflowbit) - borrow;
        if (t < digit_overflowbit) borrow=1;
        else { borrow = 0; t -= digit_overflowbit; }
        data->magnitude[x] = static_cast<digit_t>(t);
    }

    _cleanup();
    if (borrow != 0) negative=!negative;
}

} // namespace detail
} // namespace xint
} // namespace boost
