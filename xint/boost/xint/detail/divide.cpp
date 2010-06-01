
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
    \brief Contains the definitions of the divide function, and its supporting
           functions.

    This file will be included by the library itself when needed.
*/

#if defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)

#if defined(BOOST_XINT_COMPILED_LIB)
    #include "internals.hpp"
#endif // defined(BOOST_XINT_COMPILED_LIB)

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

BOOST_XINT_INLINE void divide_by_single_digit(data_t& qtarget, data_t& rtarget,
    const data_t d1, digit_t d2)
{
    size_t d1len = d1.length;
    const digit_t *d1digits = d1.digits();

    doubledigit_t a = 0;
    const doubledigit_t lomask(digit_mask);
    const doubledigit_t himask(lomask << bits_per_digit);

    int m = int(d1len) - 1;
    const digit_t *d1p = d1digits + m;

    qtarget.beginmod(m + 1);
    digit_t *qp = qtarget.digits() + m, *qe = qtarget.digits() +
        qtarget.max_length();
    for (int i = m; i >= 0; --i, --d1p, --qp) {
        a = (a & ~lomask) | *d1p;
        if (qp < qe) *qp = static_cast<digit_t>(a / d2);
        a = (a & ~himask) | ((a % d2) << bits_per_digit);
    }
    qtarget.length = (std::min<size_t>)(m + 1, qtarget.max_length());
    qtarget.endmod();

    rtarget.beginmod(1);
    rtarget.digits()[0] = static_cast<digit_t>((a & himask) >> bits_per_digit);
    rtarget.length = 1;
    rtarget.endmod();
}

BOOST_XINT_INLINE void sub_divide2(data_t& qtarget, data_t& rtarget, const
    data_t d1, const data_t d2)
{
    assert(d2[d2.length - 1] >= digit_overflowbit / 2);

    const digit_t *byDigits = d2.digits();

    size_t n = d2.length, m = d1.length - n;
    size_t i = m + n, j = m;

    qtarget.beginmod(j + 1);
    rtarget.beginmod();

    qtarget.length = j + 1;
    digit_t *qdigits = qtarget.digits();
    rtarget.duplicate_data(d1);
    do {
        doubledigit_t ri = (doubledigit_t(rtarget.get_digit(i)) <<
            bits_per_digit) + rtarget.get_digit(i - 1);

        doubledigit_t q = (std::min<doubledigit_t>)(ri / byDigits[n - 1],
            digit_mask);

        data_t r2(qtarget.new_holder(), 0, 3), qq(qtarget.new_holder(), 0);
        while (1) {
            // We need three digits here, a doubledigit_t alone won't suffice.
            r2.set_unsigned(ri - (q * byDigits[n - 1]));
            r2 <<= bits_per_digit;
            r2 += data_t(qtarget.new_holder(), rtarget[i - 2]);
            qq.set_unsigned(q);
            if (data_t(qtarget.new_holder(), byDigits[n - 2]) * qq <= r2) break;
            --q;
        }

        qq.set_unsigned(q);
        data_t bq(d2 * qq);
        if (rtarget < bq) {
            --q;
            bq -= d2;
        }

        bq <<= bits_per_digit * (i - n);
        rtarget -= bq;

        qdigits[j--] = digit_t(q);
    } while (--i >= n);
    rtarget.endmod();
    qtarget.endmod();
}

//! This function weeds out single-digit division (for more efficient
//! processing), normalizes the values if necessary, then calls sub_divide2 for
//! the actual work.
BOOST_XINT_INLINE void sub_divide(data_t& qtarget, data_t& rtarget, const data_t
    d1, const data_t d2)
{
    if (d2.length == 1) {
        divide_by_single_digit(qtarget, rtarget, d1, d2[0]);
        return;
    }

    // The normalization step
    digit_t normalizer = static_cast<digit_t>(digit_overflowbit /
        (doubledigit_t(d2[d2.length - 1]) + 1));
    if (normalizer > 1) {
        const data_t norm(qtarget.new_holder(), normalizer);
        sub_divide2(qtarget, rtarget, d1 * norm, d2 * norm);

        // Denormalization step. This requires a division by a single digit_t.
        data_t tmp(qtarget.new_holder());
        divide_by_single_digit(rtarget, tmp, rtarget.duplicate(), normalizer);
    } else sub_divide2(qtarget, rtarget, d1, d2);
}

//! This function handles signs and trivial cases, and calls sub_divide for the
//! heavy lifting.
BOOST_XINT_INLINE void divide(data_t& qtarget, data_t& rtarget, const data_t d1,
    const data_t d2)
{
    int sign1 = (d1.is_zero() ? 0 : d1.negative ? -1 : 1);
    int sign2 = (d2.is_zero() ? 0 : d2.negative ? -1 : 1);
    if (sign2 == 0) throw exceptions::divide_by_zero();

    qtarget.beginmod();
    rtarget.beginmod();
    int comp = compare(d1, d2, true);
    if (comp < 0) {
        qtarget.set(0);
        rtarget.duplicate_data(d1);
    } else if (comp == 0) {
        qtarget.set(sign1 != sign2 ? -1 : 1);
        rtarget.set(0);
    } else if (sign1 < 0 && sign2 < 0) {
        sub_divide(qtarget, rtarget, d1.negate(), d2.negate());
        qtarget.negative = false;
        rtarget.negative = true;
    } else if (sign1 < 0) {
        sub_divide(qtarget, rtarget, d1.negate(), d2);
        qtarget.negative = rtarget.negative = true;
    } else if (sign2 < 0) {
        sub_divide(qtarget, rtarget, d1, d2.negate());
        qtarget.negative = true;
        rtarget.negative = false;
    } else {
        sub_divide(qtarget, rtarget, d1, d2);
        qtarget.negative = rtarget.negative = false;
    }
    rtarget.endmod();
    qtarget.endmod();
}

BOOST_XINT_INLINE data_t::divide_t divide(const data_t d1, const data_t d2) {
    data_t q(d1.new_holder()), r(d1.new_holder());
    divide(q, r, d1, d2);
    return data_t::divide_t(q, r);
}

BOOST_XINT_INLINE data_t& data_t::operator/=(const data_t b) {
    data_t remainder(new_holder());
    divide(*this, remainder, duplicate(), b);
    return *this;
}

BOOST_XINT_INLINE data_t& data_t::operator%=(const data_t b) {
    data_t quotient(new_holder());
    divide(quotient, *this, duplicate(), b);
    if (negative) *this += b.abs();
    return *this;
}

BOOST_XINT_INLINE data_t operator/(const data_t n1, const data_t n2) {
    return data_t(n1) /= n2;
}

BOOST_XINT_INLINE data_t operator%(const data_t n1, const data_t n2) {
    return data_t(n1) %= n2;
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)
