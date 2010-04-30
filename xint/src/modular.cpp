
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
    \brief Contains the definitions for the basic modular math functions.

    The xint::invmod function, which logically would be here, is actually in
    gcd.cpp to share the gcd_core function.
*/

#include "../boost/xint/integer.hpp"
#include "../boost/xint/monty.hpp"

namespace boost {
namespace xint {

namespace detail {

void mod(base_integer& target, const base_integer& n, const base_integer& m) {
    integer q;
    divide(q, target, n, m);
    if (target._get_negative()) {
        temp_t mm(m);
        mm._set_negative(false);
        add(target, target, mm);
    }
}

void powmod(base_integer& target, const base_integer& _n, const base_integer&
    _e, const base_integer& _m, bool noMontgomery)
{
    if (_m._is_zero() || _m._get_negative()) throw exceptions::invalid_modulus();
    if (_e._is_zero()) {
        target._set_unsigned(1);
        return;
    }

    integer n(_n._to_integer()), e(_e._to_integer()), m(_m._to_integer()),
        answer(integer::one());
    bool neg=(n.sign() < 0 && e.odd());

    // Montgomery's method is often noticeably faster, but only works if the
    // m is odd.
    if (m.odd() && !noMontgomery) {
        answer = montgomeryPowerMod(abs(n) % m, abs(e), m);
    } else {
        integer p(abs(n));

        size_t length=e._get_length(), lastBitCount=0;
        detail::digit_t ee(e._get_digit(length-1));
        while (ee != 0) { ee >>= 1; ++lastBitCount; }

        for (size_t eIndex=0; eIndex < length; ++eIndex) {
            detail::digit_t ee(e._get_digit(eIndex));

            int bitCount(int(eIndex == length-1 ? lastBitCount :
                detail::bits_per_digit));
            while (bitCount-- > 0) {
                if (ee & 0x01) answer=mulmod(answer, p, m);
                p=sqrmod(p, m);
                ee >>= 1;
            }
        }
    }

    answer._set_negative(neg);
    target._attach(answer);
}

} // namespace detail

/*! \brief Get the remainder after dividing a number by another.

- Complexity: O(n<sup>2</sup>)

\param[in] n The dividend.
\param[in] m The divisor.

\returns The result of \c n % \c m.
*/
integer mod(const integer& n, const integer& m) {
    integer r;
    detail::mod(r, n, m);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Get the modular product of two integers.

- Complexity: O(2n<sup>2</sup>)

\param[in] n, by The integers to multiply.
\param[in] m The modulus.

\returns The result of \c n * \c by % \c m.

\remarks
This is purely a convenience function, to make it easier to write
self-documenting code. It does not provide any additional efficiency over
writing out the calculation.
*/
integer mulmod(const integer& n, const integer& by, const integer& m) {
    return mod(n * by, m);
}

/*! \brief Get the modular product of squaring an integer.

- Complexity: O(2n<sup>2</sup>)

\param[in] n The integer to square.
\param[in] m The modulus.

\returns The result of \c sqr(n) % \c m.

\remarks
This is purely a convenience function, to make it easier to write
self-documenting code. It does not provide any additional efficiency over
writing out the calculation.
*/
integer sqrmod(const integer& n, const integer& m) {
    return mod(sqr(n), m);
}

/*! \brief Get the result of \c n<sup>e</sup> % \c m, keeping the intermediate
           results (relatively) small.

- Complexity: O(n<sub>n</sub><sup>2</sup>n<sub>e</sub>)

\param[in] n The integer to exponentiate.
\param[in] e The exponent.
\param[in] m The modulus.
\param[in] noMontgomery Used for testing, leave false for normal operation.

\returns The result of \c pow(n, \c exponent) % \c modulus.

\exception exceptions::invalid_modulus if the modulus is less than one.

\remarks
Because this function keeps the intermediate results small, it is far faster
and uses far less memory than a naive implementation.

\par
In addition, this function will use the Montgomery Reduction internally, if the
modulus is an odd number (and if \c noMontgomery isn't set), which is almost
always faster than the non-Montgomery method.
*/
integer powmod(const integer& n, const integer& e, const integer& m, bool
    noMontgomery)
{
    integer r;
    detail::powmod(r, n, e, m, noMontgomery);
    return BOOST_XINT_MOVE(r);
}

} // namespace xint
} // namespace boost
