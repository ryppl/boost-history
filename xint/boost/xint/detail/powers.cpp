
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
    \brief Contains functions related to powers of a number.

    This file will be included by the library itself when needed.
*/

#if defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)

#if defined(BOOST_XINT_COMPILED_LIB)
    #include "internals.hpp"
#endif // defined(BOOST_XINT_COMPILED_LIB)

namespace boost {
namespace xint {
namespace detail {
//! @cond detail

BOOST_XINT_INLINE size_t count_one_bits(const data_t n) {
    size_t r = 0;
    for (const digit_t *p = n.digits(), *pe = p + n.length; p != pe; ++p) {
        digit_t nn(*p);
        while (nn != 0) {
            if (nn & 0x01) ++r;
            nn >>= 1;
        }
    }
    return r;
}

BOOST_XINT_INLINE void pow(data_t& target, const data_t n, const data_t e) {
    if (e.is_zero()) {
        // Anything to the zeroth power is one.
        target.set(1);
        return;
    } else if (e.negative) {
        // Anything to a negative power is a fractional value, which an integer
        // library can't represent.
        target.set(0);
        return;
    }

    bool neg = (!n.is_zero() && n.negative && e.is_odd());

    size_t lastBitCount = 0;
    digit_t ee(e[e.length - 1]);
    while (ee != 0) { ee >>= 1; ++lastBitCount; }

    data_t p(n.abs());
    p.beginmod(data_t::bits_to_digits(log2(p) * log2(e)));

    target.beginmod(data_t::bits_to_digits(log2(p) * count_one_bits(e)));
    target.set(1);
    for (size_t eIndex=0; eIndex < e.length; ++eIndex) {
        digit_t eee(e[eIndex]);

        int bitCount(int(eIndex == e.length - 1 ? lastBitCount :
            bits_per_digit));
        while (bitCount-- > 0) {
            if (eee & 0x01) target *= p;
            square(p, p);

            eee >>= 1;
        }
    }
    target.negative = neg;
    target.endmod();

    p.endmod();
}

BOOST_XINT_INLINE void pow2(data_t& target, size_t exponent) {
    target.beginmod();
    target.set(0);
    setbit(target, exponent);
    target.endmod();
}

BOOST_XINT_INLINE void factorial(data_t& target, size_t n) {
    target.beginmod(data_t::bits_to_digits(n * log2(n / 2)) + 1);
    target.set(1);

    data_t one(target.new_holder(), 1);
    data_t nn(target.new_holder(), n);
    while (nn > one) {
        target *= nn;
        --nn;
    }

    target.endmod();
}

//! @endcond detail
} // namespace detail
} // namespace xint
} // namespace boost

#endif // defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)
