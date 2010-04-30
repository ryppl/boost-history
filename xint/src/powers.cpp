
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
    \brief Contains the definitions for functions related to powers of a number.
*/

#include "../boost/xint/integer.hpp"
#include "../boost/xint/fixed_integer.hpp"

#include <vector>

namespace boost {
namespace xint {

using namespace detail;

namespace detail {

void sqr(base_integer& target, const base_integer& n) {
    const digit_t *ndigits = n._get_digits();
    size_t nlen = n._get_length();

    integer r;
    r._realloc(nlen * 2 + 1);
    digit_t *rdigits = r._get_writable_digits();

    // Calculate the product of digits of unequal index
    size_t i = 0;
    doubledigit_t c;
    do {
        const doubledigit_t ii = ndigits[i];
        const digit_t *jp = ndigits + i + 1;
        digit_t *rp = rdigits + i + (i + 1), *rpe = rdigits + i + nlen;

        c = 0;
        while (rp < rpe) {
            doubledigit_t t = ii * *jp++ + *rp + c;
            *rp++ = static_cast<digit_t>(t);
            c = (t >> bits_per_digit);
        }
        *rp = static_cast<digit_t>(c);
    } while (++i < nlen - 1);

    // Multiplication of inner products by two
    c = 0;
    digit_t *rp = rdigits + 1, *rpe = rdigits + (2 * nlen) - 1;
    do {
        doubledigit_t t = (doubledigit_t(*rp) << 1) + c;
        *rp++ = static_cast<digit_t>(t);
        c = (t >> bits_per_digit);
    } while (rp < rpe);
    *rp = static_cast<digit_t>(c);

    // Addition of inner squares
    const digit_t *ip = ndigits, *ipe = ndigits + nlen;
    rp = rdigits;
    c = 0;
    do {
        doubledigit_t t = doubledigit_t(*ip) * *ip + *rp + c;
        *rp++ = static_cast<digit_t>(t);
        c = (t >> bits_per_digit);

        t = c + *rp;
        *rp++ = static_cast<digit_t>(t);
        c = (t >> bits_per_digit);
    } while (++ip < ipe);
    *rp += static_cast<digit_t>(c);

    r._cleanup();
    target._attach(r);
}

void pow(base_integer& target, const base_integer& n, const base_integer& e) {
    if (e._is_zero()) {
        // Anything to the zeroth power is one.
        target._set_unsigned(1);
        return;
    } else if (!e._is_zero() && e._get_negative()) {
        // Anything to a negative power is a fractional value, which an integer
        // library can't represent.
        target._set_unsigned(0);
        return;
    }

    bool neg=(!n._is_zero() && n._get_negative() && (e._get_digit(0) & 0x01));

    size_t length=e._get_length(), lastBitCount=0;
    digit_t ee(e._get_digit(length-1));
    while (ee != 0) { ee >>= 1; ++lastBitCount; }

    temp_t p(n);
    p._set_negative(false);

    // If target is a fixed integer, we want to calculate using the same
    // limitations on intermediate values.
    temp_t answer(target, 1);
    answer._set_unsigned(1);
    for (size_t eIndex=0; eIndex < length; ++eIndex) {
        digit_t eee(e._get_digit(eIndex));

        int bitCount(int(eIndex == length-1 ? lastBitCount : bits_per_digit));
        while (bitCount-- > 0) {
            if (eee & 0x01) multiply(answer, answer, p);
            sqr(p, p);
            eee >>= 1;
        }
    }

    answer._set_negative(neg);
    target._attach(answer);
}

void factorial(base_integer& target, size_t n) {
    // If target is a fixed integer, we want to calculate using the same
    // limitations on intermediate values.
    temp_t answer(target, 1);
    answer._set_unsigned(1);

    fixed_integer_any nn(n);
    while (nn > 1) {
        multiply(answer, answer, nn);
        nn._decrement();
    }

    target._attach(answer);
}

} // namespace detail

/*! \brief Calculate the value of 2<sup>e</sup>

- Complexity: O(1)

\param[in] e The exponent to operate on.

\returns 2 to the power of \c e.

\remarks
This is a convenience function, to help with self-documenting code.
*/
integer pow2(size_t e) {
    integer r;
    setbit(r, e);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Calculate the value of n<sup>2</sup>

- Complexity: O(n<sup>2</sup>)

\param[in] n The integer to operate on.

\returns \c n times \c n.

This function uses a faster algorithm than the standard multiplication one.
*/
integer sqr(const integer& n) {
    integer r;
    detail::sqr(r, n);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Calculate the value of n<sup>e</sup>

- Complexity: O(n<sub>n</sub><sup>2</sup>n<sub>e</sub>)

\param[in] n, e The integers to operate on.

\returns \c n to the power of \c e.
*/
integer pow(const integer& n, const integer& e) {
    integer r;
    detail::pow(r, n, e);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Calculate the value of \c n!

- Complexity: O(n<sup>3</sup>)

\param[in] n The value to operate on.

\returns \c n factorial, defined as <code>1*2*3*...*n</code>

\warning
Factorials get ridiculously huge, even with fairly small values of \c n. This
function, when used with a large parameter, is the easiest way to exhaust the
system's memory.
*/
integer factorial(size_t n) {
    integer r;
    detail::factorial(r, n);
    return BOOST_XINT_MOVE(r);
}

} // namespace xint
} // namespace boost
