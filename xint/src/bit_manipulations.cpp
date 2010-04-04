
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the bit-manipulation functions.
*/

#include "../xint.hpp"
#include "../xint_data_t.hpp"

namespace xint {

using namespace detail;

bool getbit(const integer& n, size_t bit) {
    n._throw_if_nan();
    size_t index=bit/bits_per_digit;
    if (index < n._get_length()) {
        digit_t mask=(digit_t(1) << (bit % bits_per_digit));
        return ((n._get_digit(index) & mask) != 0);
    } else return false;
}

void setbit(integer& n, size_t bit) {
    n._throw_if_nan();
    n._make_unique();
    detail::data_t *ndata=n._get_data();

    size_t index=bit/bits_per_digit;
    if (index >= n._get_length()) ndata->realloc(index+1);

    digit_t mask=(digit_t(1) << (bit % bits_per_digit));
    ndata->digits[index] |= mask;
    ndata->skipLeadingZeros();
}

void clearbit(integer& n, size_t bit) {
    n._throw_if_nan();
    size_t index=bit/bits_per_digit;
    if (index < n._get_length()) {
        n._make_unique();
        digit_t mask=(digit_t(1) << (bit % bits_per_digit));
        n._get_data()->digits[index] &= ~mask;
        n._get_data()->skipLeadingZeros();
    }
}

size_t lowestbit(const integer& n, size_t defaultValue) {
    if (n.sign()==0) return defaultValue;

    const detail::data_t *ndata=n._get_data();
    const digit_t *p=ndata->digits, *pe=p+ndata->mLength;
    while (p!=pe && *p==0) ++p;

    size_t index=(p - ndata->digits);
    size_t r=(bits_per_digit * index);
    digit_t digit=*p;

    while ((digit & 0x01)==0) {
        digit>>=1;
        ++r;
    }

    return r;
}

size_t highestbit(const integer& n, size_t defaultValue) {
    if (n.sign()==0) return defaultValue;
    return static_cast<size_t>(log2(n)-1);
}

integer bitwise_and(const integer& n1, const integer& n2) {
    n1._throw_if_nan();
    n2._throw_if_nan();

    const detail::data_t *smaller=n1._get_data(), *larger=n2._get_data();
    if (smaller->mLength > larger->mLength) std::swap(smaller, larger);

    integer r;
    detail::data_t *rdata=r._get_data();
    rdata->alloc(smaller->mLength);

    const digit_t *s1=smaller->digits, *s1e=s1+smaller->mLength, *s2=larger->digits;
    digit_t *t=rdata->digits;

    while (s1 < s1e) *t++ = *s1++ & *s2++;

    rdata->skipLeadingZeros();
    return r;
}

integer bitwise_or(const integer& n1, const integer& n2) {
    n1._throw_if_nan();
    n2._throw_if_nan();

    const detail::data_t *smaller=n1._get_data(), *larger=n2._get_data();
    if (smaller->mLength > larger->mLength) std::swap(smaller, larger);

    integer r;
    detail::data_t *rdata=r._get_data();
    rdata->alloc(larger->mLength);

    const digit_t *s1=smaller->digits, *s1e=s1+smaller->mLength;
    const digit_t *s2=larger->digits, *s2e=s2+larger->mLength;
    digit_t *t=rdata->digits;

    while (s1<s1e) *t++ = *s1++ | *s2++;
    while (s2<s2e) *t++ = *s2++;

    rdata->skipLeadingZeros();
    return r;
}

integer bitwise_xor(const integer& n1, const integer& n2) {
    n1._throw_if_nan();
    n2._throw_if_nan();

    const detail::data_t *smaller=n1._get_data(), *larger=n2._get_data();
    if (smaller->mLength > larger->mLength) std::swap(smaller, larger);

    integer r;
    detail::data_t *rdata=r._get_data();
    rdata->alloc(larger->mLength);

    const digit_t *s1=smaller->digits, *s1e=s1+smaller->mLength;
    const digit_t *s2=larger->digits, *s2e=s2+larger->mLength;
    digit_t *t=rdata->digits;

    while (s1<s1e) *t++ = *s1++ ^ *s2++;
    while (s2<s2e) *t++ = *s2++;

    rdata->skipLeadingZeros();
    return r;
}

integer shift(const integer& n, int byBits) {
    if (byBits > 0) return shift_left(n, byBits);
    else return shift_right(n, -byBits);
}

integer shift_left(const integer& _n, size_t byBits) {
    _n._throw_if_nan();

    if (byBits==0) return _n;

    integer n(_n);
    n._make_unique();
    n._get_data()->shift_left(byBits);
    return n;
}

integer shift_right(const integer& _n, size_t byBits) {
    _n._throw_if_nan();

    if (byBits==0) return _n;

    integer n(_n);
    n._make_unique();
    n._get_data()->shift_right(byBits);
    return n;
}

} // namespace detail
