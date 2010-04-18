
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
    \brief Contains the definitions for math primitives.
*/

#include "../boost/xint/xint.hpp"
#include <cassert>

namespace boost {
namespace xint {

using namespace detail;

/*! \brief Returns the absolute value of an integer.

\param[in] n The integer to operate on.

\returns If \c n is zero or positive, returns \c n. Otherwise returns \c -n.
*/
integer abs(const integer& n) {
    return (n.sign() < 0 ? -n : n);
}

/*! \brief Return the additive inverse of an integer.

\param[in] n The integer to operate on.

\returns \c -n.
*/
integer negate(const integer& n) {
    integer nn(n);
    nn._set_negative(!nn._get_negative());
    return nn;
}

/*! \brief Calculate the sum of two integers.

\param[in] n1, n2 The integers to add.

\returns The sum of the parameters.
*/
integer add(const integer& n1, const integer& n2) {
    int sign1=n1.sign(), sign2=n2.sign();
    if (sign1 != sign2) {
        if (sign1==0) return n2;
        else if (sign2==0) return n1;
        else return subtract(n1, -n2);
    } else {
        bool swap(n1._get_length() < n2._get_length());
        const integer &larger(swap ? n2 : n1), &smaller(swap ? n1 : n2);

        integer r;
        r._duplicate(larger, 1);
        r._add(smaller);
        return r;
    }
}

/*! \brief Calculate the difference between two integers.

\param[in] n1, n2 The integers to operate on.

\returns The difference between the parameters.
*/
integer subtract(const integer& n1, const integer& n2) {
    int sign1=n1.sign(), sign2=n2.sign();
    if (sign1 != sign2) {
        if (sign1 == 0) return -n2;
        else if (sign2 == 0) return n1;
        else return add(n1, -n2);
    } else {
        bool swap=(sign1 == (n1 < n2 ? 1 : -1));
        const integer &larger(swap ? n2 : n1), &smaller(swap ? n1 : n2);

        integer r;
        r._duplicate(larger, 1);
        r._subtract(smaller);
        return (swap ? -r : r);
    }
}

/*! \brief Calculate the product of two integers.

\param[in] n, by The integers to operate on.

\returns The product of the parameters.

\remarks
Automatically uses the more-efficient squaring algorithm if it can trivially
detect that the two parameters are copies of the same number.
*/
integer multiply(const integer& n, const integer& by) {
    int nsign=n.sign(), bysign=by.sign();
    if (nsign==0 || bysign==0) return integer::zero();

    const digit_t *ndigits=n._get_digits(), *bydigits=by._get_digits();
    if (ndigits==bydigits) return sqr(n);
    size_t nlen=n._get_length(), bylen=by._get_length();

    // In multiplication, we know that the answer will be less than or equal to
    // the sum of the lengths of the operands.
    integer answer;
    answer._realloc(nlen + bylen);
    digit_t *adigits=answer._get_digits();

    // Now multiply the digits, starting at the least-significant digit.
    const digit_t *d1 = ndigits, *d1e = d1 + nlen;
    const digit_t *d2e = bydigits + bylen;
    for (int digit1=0; d1<d1e; ++digit1, ++d1) {
        const digit_t *d2=bydigits;
        for (int digit2=0; d2<d2e; ++digit2, ++d2) {
            // First multiply the two digits
            doubledigit_t t=doubledigit_t(*d1) * *d2;

            // Now add the result to the answer.
            int carry=0;
            digit_t *a = adigits + digit1 + digit2;
            doubledigit_t addt=doubledigit_t(*a) + (t & digit_mask);
            if (addt >= digit_overflowbit) carry=1;
            *a++=static_cast<digit_t>(addt);

            addt=*a + ((t >> bits_per_digit) & digit_mask) + carry;
            if (addt >= digit_overflowbit) carry=1; else carry=0;
            *a++=static_cast<digit_t>(addt);

            while (carry) {
                addt=*a+1;
                *a++ = static_cast<digit_t>(addt);
                if (addt < digit_overflowbit) break;
            }
        }
    }

    answer._set_negative(nsign != bysign);
    answer._cleanup();
    return answer;
}

namespace {

std::pair<integer, integer> divideBySingleDigit(const integer& d1, digit_t d2) {
    size_t d1len=d1._get_length();
    const digit_t *d1digits=d1._get_digits();

    integer quotient, remainder;
    quotient._realloc(d1len);
    remainder._realloc(1);

    doubledigit_t a=0;
    const doubledigit_t lomask(digit_mask);
    const doubledigit_t himask(doubledigit_t(digit_mask) << bits_per_digit);

    int m = int(d1len) - 1;
    const digit_t *d1p=d1digits+m;
    digit_t *qp=quotient._get_digits()+m;
    for (int i = m; i >= 0; --i, --d1p, --qp) {
        a = (a & ~lomask) | *d1p;
        *qp = static_cast<digit_t>(a / d2);
        a = (a & ~himask) | ((a % d2) << bits_per_digit);
    }
    remainder._get_digits()[0] = static_cast<digit_t>((a & himask) >>
        bits_per_digit);

    quotient._cleanup();
    remainder._cleanup();
    return std::make_pair(quotient, remainder);
}

std::pair<integer, integer> subDivide(integer d1, integer d2) {
    const digit_t *byDigits = d2._get_digits();

    // The normalization step
    digit_t d = static_cast<digit_t>(digit_overflowbit /
        (doubledigit_t(byDigits[d2._get_length()-1])+1));
    if (d > 1) {
        d1 *= d;
        d2 *= d;

        // Gotta reset this
        byDigits = d2._get_digits();
    }
    assert(d2._get_digit(d2._get_length()-1) >= digit_overflowbit / 2);

    integer quotient, r(d1);

    size_t n=d2._get_length(), m=d1._get_length() - n;
    size_t i=m+n, j=m;
    quotient._realloc(j+1);
    digit_t *qdigits=quotient._get_digits();

    r._make_unique();
    do {
        doubledigit_t ri=(doubledigit_t(r._get_digit(i, true))
            << bits_per_digit) + r._get_digit(i-1, true);
        doubledigit_t q=(std::min<doubledigit_t>)(ri / byDigits[n-1], digit_mask);

        while (1) {
            // We need three digits here, a doubledigit_t won't suffice.
            doubledigit_t r2a=ri - (q * byDigits[n-1]);
            integer r2=(integer(r2a) << bits_per_digit) + r._get_digit(i-2);
            if (byDigits[n-2] * q <= r2) break;
            --q;
        }

        integer bq=d2*q;
        if (r < bq) { --q; bq -= d2; }

        integer rtemp((r >> (bits_per_digit * (i-n))) - bq);

        digit_t *rdigits=r._get_digits();

        size_t rlen=r._get_length();
        for (size_t x=0; x<=n && i-n+x < rlen; ++x)
            rdigits[i-n+x]=rtemp._get_digit(x, true);
        r._cleanup();

        qdigits[j--]=digit_t(q);
    } while (--i >= n);

    quotient._cleanup();

    if (d > 1) {
        // Denormalization step. This requires a division by a single digit_t.
        integer remainder=divideBySingleDigit(r, d).first;
        return std::make_pair(quotient, remainder);
    } else {
        return std::make_pair(quotient, r);
    }
}

} // namespace

/*! \brief Calculate how many \c dividends would fit into \c divisor.

\param[in] dividend, divisor The integers to operate on.

\returns The integer value of \c dividend divided by \c divisor.

\exception exceptions::divide_by_zero if \c divisor is zero.
*/
integer divide(const integer& dividend, const integer& divisor) {
    return divide_r(dividend, divisor).first;
}

/*! \brief Calculate how many \c dividends would fit into \c divisor, with the
           remainder.

\param[in] d1 The dividend.
\param[in] d2 The divisor.

\returns An \c std::pair containing the quotient and remainder of \c d1 divided
by \c d2.

\exception exceptions::divide_by_zero if \c d2 is zero.
*/
std::pair<integer, integer> divide_r(const integer& d1, const integer& d2) {
    int sign1=d1.sign(), sign2=d2.sign();
    if (sign2==0) throw exceptions::divide_by_zero();

    int comp=compare(d1, d2, true);
    if (comp<0) {
        return std::make_pair(integer::zero(), d1);
    } else if (comp==0) {
        return std::make_pair(sign1 != sign2 ? integer::one() : integer(-1),
            integer::zero());
    }

    if (sign1 < 0 && sign2 < 0) {
        std::pair<integer, integer> a=subDivide(-d1, -d2);
        a.second._set_negative(true);
        return a;
    } else if (sign1 < 0) {
        std::pair<integer, integer> a=subDivide(-d1, d2);
        a.first._set_negative(true);
        a.second._set_negative(true);
        return a;
    } else if (sign2 < 0) {
        std::pair<integer, integer> a=subDivide(d1, -d2);
        a.first._set_negative(true);
        return a;
    };

    if (d2._get_length() == 1) {
        return divideBySingleDigit(d1, d2._get_digit(0));
    } else {
        return subDivide(d1, d2);
    }
}

} // namespace xint
} // namespace boost
