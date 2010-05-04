
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

#include "../boost/xint/integer.hpp"

namespace boost {
namespace xint {

//! @cond detail
namespace detail {

bool getbit(const base_integer& n, size_t bit) {
    size_t index = bit / bits_per_digit;
    if (index < n._get_length()) {
        digit_t mask=(digit_t(1) << (bit % bits_per_digit));
        return ((n._get_digit(index) & mask) != 0);
    } else return false;
}

void setbit(base_integer& n, size_t bit) {
    if (n._is_fixed() && bit >= n._fixed_bits()) return;

    n._make_unique();

    size_t index = bit / bits_per_digit;
    if (index >= n._get_length()) n._realloc(index+1);

    digit_t mask = (digit_t(1) << (bit % bits_per_digit));
    n._get_writable_digits()[index] |= mask;
    n._cleanup();
}

void clearbit(base_integer& n, size_t bit) {
    if (n._is_fixed() && bit >= n._fixed_bits()) return;

    size_t index = bit / bits_per_digit;
    if (index < n._get_length()) {
        n._make_unique();
        digit_t mask=(digit_t(1) << (bit % bits_per_digit));
        n._get_writable_digits()[index] &= ~mask;
        n._cleanup();
    }
}

size_t lowestbit(const base_integer& n, size_t valueIfZero) {
    if (n._is_zero()) return valueIfZero;

    const digit_t *p=n._get_digits(), *pe=p+n._get_length();
    while (p!=pe && *p==0) ++p;

    size_t index=(p - n._get_digits());
    size_t r=(bits_per_digit * index);
    digit_t digit=*p;

    while ((digit & 0x01)==0) {
        digit>>=1;
        ++r;
    }

    return r;
}

size_t highestbit(const base_integer& n, size_t valueIfZero) {
    return (n._is_zero() ? valueIfZero : n._log2()-1);
}

void bitwise_and(base_integer& target, const base_integer& n1, const
    base_integer& n2)
{
    bool n1larger=(n1._get_length() > n2._get_length());
    const base_integer &smaller(n1larger ? n2 : n1),
        &larger(n1larger ? n1 : n2);

    size_t maxlen = (std::min)(smaller._get_length(), target._get_max_length());

    target._make_unique();
    target._realloc(maxlen);

    const digit_t *s1 = smaller._get_digits(), *s1e = s1 + maxlen, *s2 =
        larger._get_digits();
    digit_t *t = target._get_writable_digits();

    while (s1 < s1e) *t++ = *s1++ & *s2++;

    target._cleanup();
}

void bitwise_or(base_integer& target, const base_integer& n1, const
    base_integer& n2)
{
    bool n1larger = (n1._get_length() > n2._get_length());
    const base_integer &smaller(n1larger ? n2 : n1), &larger(n1larger ? n1 : n2);

    size_t maxlen = (std::min)(larger._get_length(), target._get_max_length());

    target._make_unique();
    target._realloc(maxlen);

    const digit_t *s1 = smaller._get_digits(), *s1e = s1 +
        (std::min)(smaller._get_length(), maxlen);
    const digit_t *s2 = larger._get_digits(), *s2e = s2 + maxlen;
    digit_t *t = target._get_writable_digits();

    while (s1 < s1e) *t++ = *s1++ | *s2++;
    while (s2 < s2e) *t++ = *s2++;

    target._cleanup();
}

void bitwise_xor(base_integer& target, const base_integer& n1, const
    base_integer& n2)
{
    bool n1larger=(n1._get_length() > n2._get_length());
    const base_integer &smaller(n1larger ? n2 : n1), &larger(n1larger ? n1 : n2);

    size_t maxlen = (std::min)(larger._get_length(), target._get_max_length());

    target._make_unique();
    target._realloc(maxlen);

    const digit_t *s1 = smaller._get_digits(), *s1e = s1 +
        (std::min)(smaller._get_length(), maxlen);
    const digit_t *s2 = larger._get_digits(), *s2e = s2 + maxlen;
    digit_t *t = target._get_writable_digits();

    while (s1<s1e) *t++ = *s1++ ^ *s2++;
    while (s2<s2e) *t++ = *s2++;

    target._cleanup();
}

void shift_left(base_integer& target, const base_integer& n, size_t byBits) {
    if (byBits==0) {
        target._attach(n, false);
        return;
    }

    size_t bytes = byBits / bits_per_digit, bits = byBits % bits_per_digit;
    size_t nlen = n._get_length();

    temp_t answer(target, n._get_length() + bytes + 1);
    digit_t *tgt = answer._get_writable_digits();
    size_t anslen = answer._get_max_length();

    const digit_t *src = n._get_digits();

    // bits/bytes is distance to move
    // nlen is one greater than top source byte to move
    nlen = (std::min)(nlen, anslen - bytes - (bits != 0 ? 1 : 0));

    if (bits != 0 && bytes+1 < anslen) {
        // Handle both bits and bytes in one pass
        const digit_t *s = src + nlen - 1;
        digit_t *t = tgt + nlen + bytes;

        *t-- = *s >> (bits_per_digit - bits);
        while (s > src) {
            *t = (*s-- << bits);
            *t-- |= (*s >> (bits_per_digit - bits));
        }
        *t = (*s << bits);
    } else if (bytes != 0 && bytes+1 < anslen) {
        memcpy(tgt + bytes, src, sizeof(digit_t) * nlen);
    }
    answer._cleanup();
    answer._set_negative(n._get_negative());
    target._attach(answer, true);
}

void shift_right(base_integer& target, const base_integer& n, size_t byBits) {
    if (byBits==0) {
        target._attach(n, false);
        return;
    }

    size_t bytes = byBits / bits_per_digit, bits = byBits % bits_per_digit,
        bits2 = bits_per_digit - bits;

    temp_t answer(target, n._get_length() + bytes + 1);
    digit_t *tgt = answer._get_writable_digits();

    size_t nlen = n._get_length();
    const digit_t *src = n._get_digits();
    if (bytes < nlen) {
        if (bits != 0) {
            // Handle both bits and bytes in one pass
            const digit_t *s = src + bytes, *se = src + nlen - 1;
            digit_t *t = tgt;
            while (s!=se) {
                *t = (*s++ >> bits);
                *t++ |= (*s << bits2);
            }
            *t = (*s >> bits);
        } else if (bytes != 0) {
            memcpy(tgt, src + bytes, sizeof(digit_t) * (nlen - bytes));
        }
    }
    answer._cleanup();
    answer._set_negative(n._get_negative());
    target._attach(answer, true);
}

} // namespace detail
//! @endcond detail

/*! \brief Queries the value of a particular bit in an integer.

- Complexity: O(1)

\param[in] n The integer to query.
\param[in] bit The zero-based index of the bit you're asking about.

\returns \c true if the specified bit is set (has a value of one), \c false if
it is clear.
*/
bool getbit(const integer& n, size_t bit) {
    return detail::getbit(n, bit);
}

/*! \brief Sets a specific bit in an integer to one.

- Complexity: O(1)

\param[in,out] n The integer to operate on.
\param[in] bit The zero-based index of the bit to change.

\returns Nothing.
*/
void setbit(integer& n, size_t bit) {
    detail::setbit(n, bit);
}

/*! \brief Sets a specific bit in an integer to zero.

- Complexity: O(1)

\param[in,out] n The integer to operate on.
\param[in] bit The zero-based index of the bit to change.

\returns Nothing.
*/
void clearbit(integer& n, size_t bit) {
    detail::clearbit(n, bit);
}

/*! \brief Obtains the index of the lowest bit in an integer with a value of one.

- Complexity: O(1)

\param[in] n The integer to query.
\param[in] valueIfZero The value to return if \c n is zero (as there is no
correct answer in that case).

\returns The zero-based index of the lowest one-bit in the integer, or \c
valueIfZero if the integer contains no set bits.
*/
size_t lowestbit(const integer& n, size_t valueIfZero) {
    return detail::lowestbit(n, valueIfZero);
}

/*! \brief Obtains the index of the highest bit in an integer with a value of one.

- Complexity: O(1)

\param[in] n The integer to query.
\param[in] valueIfZero The value to return if \c n is zero (as there is no
correct answer in that case).

\returns The zero-based index of the highest one-bit in the integer, or \c
valueIfZero if the integer contains no set bits.
*/
size_t highestbit(const integer& n, size_t valueIfZero) {
    return detail::highestbit(n, valueIfZero);
}

/*! \brief Combine two integers using a bitwise \c AND operation.

- Complexity: O(n)

\param[in] n1, n2 The integers to operate on.

\returns A positive integer with all bits that are set in both parameters turned
on.
*/
integer bitwise_and(const integer& n1, const integer& n2) {
    integer r;
    detail::bitwise_and(r, n1, n2);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Combine two integers using a bitwise \c OR operation.

- Complexity: O(n)

\param[in] n1, n2 The integers to operate on.

\returns A positive integer with all bits that are set in either parameter
turned on.
*/
integer bitwise_or(const integer& n1, const integer& n2) {
    integer r;
    detail::bitwise_or(r, n1, n2);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Combine two integers using a bitwise \c XOR operation.

- Complexity: O(n)

\param[in] n1, n2 The integers to operate on.

\returns A positive integer with all bits that are set in either parameter, but
not both, turned on.
*/
integer bitwise_xor(const integer& n1, const integer& n2) {
    integer r;
    detail::bitwise_xor(r, n1, n2);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Bit-shift an integer, in either direction.

- Complexity: O(n)

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

- Complexity: O(n)

\param[in] n The integer to operate on.
\param[in] byBits The number of bits to left-shift it.

\returns The bit-shifted integer.
*/
integer shift_left(const integer& n, size_t byBits) {
    integer r;
    detail::shift_left(r, n, byBits);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Right-shift an integer by a specified number of bits.

- Complexity: O(n)

\param[in] n The integer to operate on.
\param[in] byBits The number of bits to right-shift it.

\returns The bit-shifted integer.
*/
integer shift_right(const integer& n, size_t byBits) {
    integer r;
    detail::shift_right(r, n, byBits);
    return BOOST_XINT_MOVE(r);
}

} // namespace xint
} // namespace boost
