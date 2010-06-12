
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
    \brief Contains the modular math functions.

    This file will be included by the library itself when needed.
*/

#if defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)

#if defined(BOOST_XINT_COMPILED_LIB)
    #include "internals.hpp"
    #include "monty.hpp"
#endif // defined(BOOST_XINT_COMPILED_LIB)

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

BOOST_XINT_INLINE void mulmod(data_t& target, const data_t n, const data_t by,
    const data_t modulus)
{
    target.duplicate_data(n);
    target *= by;
    target %= modulus;
}

BOOST_XINT_INLINE void sqrmod(data_t& target, const data_t n, const data_t
    modulus)
{
    square(target, n);
    target %= modulus;
}

BOOST_XINT_INLINE void powmod(data_t& target, const data_t n, const data_t e,
    const data_t m, bool noMontgomery)
{
    if (m.is_zero() || m.negative) throw exceptions::invalid_modulus();
    if (e.is_zero()) { target.set(1); return; }

    bool neg = (n.negative && e.is_odd());

    // Montgomery's method is often noticeably faster, but only works if the
    // m is odd.
    if (m.is_odd() && !noMontgomery) {
        montgomeryPowerMod(target, abs(n) % m, abs(e), m);
    } else {
        data_t answer(target.new_holder(), 1), p(abs(n));
        answer.beginmod();
        p.beginmod();

        size_t lastBitCount = 0;
        detail::digit_t ee(e[e.length - 1]);
        while (ee != 0) { ee >>= 1; ++lastBitCount; }

        for (size_t eIndex = 0; eIndex < e.length; ++eIndex) {
            detail::digit_t ee(e[eIndex]);

            int bitCount(int(eIndex == e.length - 1 ? lastBitCount :
                detail::bits_per_digit));
            while (bitCount-- > 0) {
                if (ee & 0x01) {
                    answer *= p;
                    answer %= m;
                }
                sqrmod(p, p, m);
                ee >>= 1;
            }
        }
        answer.negative = neg;
        p.endmod();
        answer.endmod();

        target.duplicate_data(answer);
    }
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)
