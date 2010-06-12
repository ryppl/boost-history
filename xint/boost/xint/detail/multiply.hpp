
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
    \brief Contains the multiplication and squaring functions.

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

BOOST_XINT_INLINE void square(data_t& target, const data_t n) {
    target.set(0);
    target.beginmod(n.length * 2 + 1, true);

    const digit_t *ndigits = n.digits();
    size_t nlen = n.length;

    digit_t *rdigits = target.digits(), *rmax = rdigits + target.length;

    // Calculate the product of digits of unequal index
    size_t i = 0;
    doubledigit_t c;
    do {
        const doubledigit_t ii = ndigits[i];
        const digit_t *jp = ndigits + i + 1;
        digit_t *rp = rdigits + i + (i + 1), *rpe = rdigits + i + nlen;
        if (rpe > rmax) rpe = rmax;

        c = 0;
        while (rp < rpe) {
            doubledigit_t t = ii * *jp++ + *rp + c;
            *rp++ = static_cast<digit_t>(t);
            c = (t >> bits_per_digit);
        }
        if (rp < rmax) *rp = static_cast<digit_t>(c);
    } while (++i < nlen - 1);

    // Multiplication of inner products by two
    c = 0;
    digit_t *rp = rdigits + 1, *rpe = rdigits + (2 * nlen) - 1;
    if (rpe > rmax) rpe = rmax;

    if (rp < rmax) {
        do {
            doubledigit_t t = (doubledigit_t(*rp) << 1) + c;
            *rp++ = static_cast<digit_t>(t);
            c = (t >> bits_per_digit);
        } while (rp < rpe);
        if (rp < rmax) *rp = static_cast<digit_t>(c);
    }

    // Addition of inner squares
    const digit_t *ip = ndigits, *ipe = ndigits + nlen;
    rp = rdigits;
    c = 0;
    do {
        doubledigit_t t = doubledigit_t(*ip) * *ip + *rp + c;
        *rp++ = static_cast<digit_t>(t);
        if (rp >= rmax) break;
        c = (t >> bits_per_digit);

        t = c + *rp;
        *rp++ = static_cast<digit_t>(t);
        if (rp >= rmax) break;
        c = (t >> bits_per_digit);
    } while (++ip < ipe);
    if (rp < rmax) *rp += static_cast<digit_t>(c);

    // No need to change length
    target.negative = false;
    target.endmod();
}

BOOST_XINT_INLINE void multiply(data_t& target, const data_t n, const data_t by)
{
    if (n.is_zero() || by.is_zero()) { target.set(0); return; }

    if (n.same_storage(by)) {
        if (n.negative != by.negative) {
            square(target, n);
            target.negative = true;
            return;
        } else {
            square(target, n);
            return;
        }
    }

    target.length = 0;
    target.beginmod(n.length + by.length, true);

    digit_t *adigits = target.digits(), *ae = adigits + target.max_length();
    size_t maxdigit = target.max_length(), nlen = n.length, bylen = by.length;

    // Now multiply the digits, starting at the least-significant digit.
    const digit_t *d1 = n.digits(), *d1e = d1 + (std::min)(nlen, maxdigit);
    for (int digit1 = 0; d1 < d1e; ++digit1, ++d1) {
        const digit_t *d2 = by.digits(), *d2e = d2 + (std::min)(bylen, maxdigit
            - digit1);
        for (int digit2 = 0; d2 < d2e; ++digit2, ++d2) {
            // First multiply the two digits
            doubledigit_t t = doubledigit_t(*d1) * *d2;

            // Now add the result to the answer.
            int carry=0;
            digit_t *a = adigits + digit1 + digit2;
            doubledigit_t addt = doubledigit_t(*a) + (t & digit_mask);
            if (addt >= digit_overflowbit) carry=1;
            *a++=static_cast<digit_t>(addt);

            if (a < ae) {
                addt = *a + ((t >> bits_per_digit) & digit_mask) + carry;
                if (addt >= digit_overflowbit) carry=1; else carry=0;
                *a++ = static_cast<digit_t>(addt);

                while (carry && a < ae) {
                    addt = *a + 1;
                    *a++ = static_cast<digit_t>(addt);
                    if (addt < digit_overflowbit) break;
                }
            }
        }
    }

    // No need to change length
    target.negative = (n.negative != by.negative);
    target.endmod();
}

BOOST_XINT_INLINE data_t& data_t::operator*=(const data_t by) {
    multiply(*this, duplicate(), by);
    return *this;
}

BOOST_XINT_INLINE data_t operator*(const data_t n1, const data_t n2) {
    data_t r(n1.new_holder());
    multiply(r, n1, n2);
    return r;
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)
