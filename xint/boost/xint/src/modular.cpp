
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the basic modular math functions, except invmod, which
    is in gcd.cpp to share gcd_core.
*/

#include "../xint.hpp"
#include "../xint_monty.hpp"

namespace xint {

integer mod(const integer& n, const integer& modBy) {
    integer r=divide_r(n, modBy).second;
    if (r.sign() < 0) r+=abs(modBy);
    return r;
}

integer mulmod(const integer& n, const integer& by, const integer& modulus) {
    return mod(n * by, modulus);
}

integer sqrmod(const integer& n, const integer& modulus) {
    return mod(sqr(n), modulus);
}

integer powmod(const integer& n, const integer& exponent, const integer&
    modulus, bool noMontgomery)
{
    if (modulus < integer::one()) {
        if (exceptions_allowed()) throw invalid_modulus();
        else return integer(not_a_number());
    }
    if (exponent.sign()==0) return integer::one();

    bool neg=(n.sign() < 0 && exponent.odd());

    integer answer(integer::one());

    // Montgomery's method is often noticeably faster, but only works if the
    // modulus is odd.
    if (modulus.odd() && !noMontgomery) {
        answer=montgomeryPowerMod(abs(n) % modulus, abs(exponent), modulus);
    } else {
        integer p(abs(n));

        int length=exponent._get_length(), lastBitCount=0;
        detail::digit_t ee(exponent._get_digit(length-1));
        while (ee != 0) { ee >>= 1; ++lastBitCount; }

        for (int eIndex=0; eIndex < length; ++eIndex) {
            detail::digit_t e(exponent._get_digit(eIndex));

            int bitCount(eIndex == length-1 ? lastBitCount :
                detail::bits_per_digit);
            while (bitCount-- > 0) {
                if (e & 0x01) answer=mulmod(answer, p, modulus);
                p=sqrmod(p, modulus);
                e >>= 1;
            }
        }
    }

    answer._set_negative(neg);
    return answer;
}

} // namespace xint
