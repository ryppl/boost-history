
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

#include "../boost/xint/xint.hpp"
#include "../boost/xint/xint_monty.hpp"

namespace xint {

/*! \brief Get the remainder after dividing a number by another.

\param[in] n The dividend.
\param[in] m The divisor.

\returns The result of \c n % \c m.
*/
integer mod(const integer& n, const integer& m) {
    integer r=divide_r(n, m).second;
    if (r.sign() < 0) r+=abs(m);
    return r;
}

/*! \brief Get the modular product of two integers.

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

\param[in] n The integer to exponentiate.
\param[in] e The exponent.
\param[in] m The modulus.
\param[in] noMontgomery Used for testing, leave false for normal operation.

\returns The result of \c pow(n, \c exponent) % \c modulus.

\exception xint::invalid_modulus if the modulus is less than one.

\remarks
Because this function keeps the intermediate results small, it is far faster
and uses far less memory than writing out the equivalent calculations.

\par
In addition, this function will use the Montgomery Reduction internally, if the
modulus is an odd number (and if \c noMontgomery isn't set), which is almost
always faster than the non-Montgomery method.
*/
integer powmod(const integer& n, const integer& e, const integer& m, bool
    noMontgomery)
{
    if (m < integer::one()) {
        if (exceptions_allowed()) throw invalid_modulus();
        else return integer(not_a_number());
    }
    if (e.sign()==0) return integer::one();

    bool neg=(n.sign() < 0 && e.odd());

    integer answer(integer::one());

    // Montgomery's method is often noticeably faster, but only works if the
    // m is odd.
    if (m.odd() && !noMontgomery) {
        answer=montgomeryPowerMod(abs(n) % m, abs(e), m);
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
    return answer;
}

} // namespace xint
