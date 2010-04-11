
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
    \brief Contains the bit-manipulation functions.
*/

#include "../boost/xint/xint.hpp"
#include "../boost/xint/xint_data_t.hpp"

namespace boost {
namespace xint {

using namespace detail;

namespace core {

/*! \brief Queries the value of a particular bit in an integer.

\param[in] n The integer to query.
\param[in] bit The zero-based index of the bit you're asking about.

\returns \c true if the specified bit is set (has a value of one), \c false if
it is clear.
*/
bool getbit(const integer& n, size_t bit) {
    size_t index=bit/bits_per_digit;
    if (index < n._get_length()) {
        digit_t mask=(digit_t(1) << (bit % bits_per_digit));
        return ((n._get_digit(index) & mask) != 0);
    } else return false;
}

/*! \brief Sets a specific bit in an integer to one.

\param[in,out] n The integer to operate on.
\param[in] bit The zero-based index of the bit to change.

\returns Nothing.
*/
void setbit(integer& n, size_t bit) {
    n._make_unique();
    detail::data_t *ndata=n._get_data();

    size_t index=bit/bits_per_digit;
    if (index >= n._get_length()) ndata->realloc(index+1);

    digit_t mask=(digit_t(1) << (bit % bits_per_digit));
    ndata->digits[index] |= mask;
    ndata->skipLeadingZeros();
}

/*! \brief Sets a specific bit in an integer to zero.

\param[in,out] n The integer to operate on.
\param[in] bit The zero-based index of the bit to change.

\returns Nothing.
*/
void clearbit(integer& n, size_t bit) {
    size_t index=bit/bits_per_digit;
    if (index < n._get_length()) {
        n._make_unique();
        digit_t mask=(digit_t(1) << (bit % bits_per_digit));
        n._get_data()->digits[index] &= ~mask;
        n._get_data()->skipLeadingZeros();
    }
}

/*! \brief Obtains the index of the lowest bit in an integer with a value of one.

\param[in] n The integer to query.
\param[in] valueIfZero The value to return if \c n is zero (as there is no
correct answer in that case).

\returns The zero-based index of the lowest one-bit in the integer, or \c
valueIfZero if the integer contains no set bits.
*/
size_t lowestbit(const integer& n, size_t valueIfZero) {
    if (n.sign()==0) return valueIfZero;

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

/*! \brief Obtains the index of the highest bit in an integer with a value of one.

\param[in] n The integer to query.
\param[in] valueIfZero The value to return if \c n is zero (as there is no
correct answer in that case).

\returns The zero-based index of the highest one-bit in the integer, or \c
valueIfZero if the integer contains no set bits.
*/
size_t highestbit(const integer& n, size_t valueIfZero) {
    if (n.sign()==0) return valueIfZero;
    return static_cast<size_t>(log2(n)-1);
}

/*! \brief Combine two integers using a bitwise \c AND operation.

\param[in] n1, n2 The integers to operate on.

\returns An integer with all bits that are set in both parameters turned on.
*/
integer bitwise_and(const integer& n1, const integer& n2) {
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

/*! \brief Combine two integers using a bitwise \c OR operation.

\param[in] n1, n2 The integers to operate on.

\returns An integer with all bits that are set in either parameter turned on.
*/
integer bitwise_or(const integer& n1, const integer& n2) {
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

/*! \brief Combine two integers using a bitwise \c XOR operation.

\param[in] n1, n2 The integers to operate on.

\returns An integer with all bits that are set in either parameter, but not
both, turned on.
*/
integer bitwise_xor(const integer& n1, const integer& n2) {
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

/*! \brief Bit-shift an integer, in either direction.

\param[in] n The integer to operate on.
\param[in] byBits The number of bits to shift it. If positive, it will shift
left, negative will shift right.

\returns The bit-shifted integer.
*/
integer shift(const integer& n, int byBits) {
    if (byBits > 0) return shift_left(n, byBits);
    else return shift_right(n, -byBits);
}

/*! \brief Left-shift an integer by a specified number of bits.

\param[in] n The integer to operate on.
\param[in] byBits The number of bits to left-shift it.

\returns The bit-shifted integer.
*/
integer shift_left(const integer& n, size_t byBits) {
    if (byBits==0) return n;

    integer nn(n);
    nn._make_unique();
    nn._get_data()->shift_left(byBits);
    return nn;
}

/*! \brief Right-shift an integer by a specified number of bits.

\param[in] n The integer to operate on.
\param[in] byBits The number of bits to right-shift it.

\returns The bit-shifted integer.
*/
integer shift_right(const integer& n, size_t byBits) {
    if (byBits==0) return n;

    integer nn(n);
    nn._make_unique();
    nn._get_data()->shift_right(byBits);
    return nn;
}

} // namespace core
} // namespace xint
} // namespace boost
