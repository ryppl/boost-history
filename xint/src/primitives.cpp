
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

#include "../boost/xint/integer.hpp"
#include <cassert>

namespace boost {
namespace xint {

//! @cond detail
namespace detail {

void sub_add(base_integer& target, const base_integer& n1, const base_integer&
    n2)
{
    bool swap(n1._get_length() < n2._get_length());
    const base_integer &larger(swap ? n2 : n1), &smaller(swap ? n1 : n2);

    // The answer to any addition problem contains, at most, one digit more
    // than the largest addend.
    integer a;
    a._realloc(larger._get_length() + 1);

    // Add the digits, starting at the least-significant digit.
    const digit_t *lp = larger._get_digits(), *l_end = lp + larger._get_length(),
        *sp = smaller._get_digits(), *s_end = sp + smaller._get_length();
    digit_t *ap = a._get_writable_digits(), *a_end = ap + a._get_length();

    digit_t carry = 0;
    while (sp != s_end) {
        doubledigit_t t = doubledigit_t(*lp++) + *sp++ + carry;
        if (t >= digit_overflowbit) {
            carry = 1;
            *ap++ = static_cast<digit_t>(t - digit_overflowbit);
        } else {
            carry = 0;
            *ap++ = static_cast<digit_t>(t);
        }
    }

    if (carry) {
        while (lp != l_end) {
            if (*lp == digit_mask) { *ap++ = 0; ++lp; }
            else { *ap++ = *lp++ + 1; carry = 0; break; }
        }
    }
    if (carry && ap != a_end) *ap++ = 1;

    while (lp != l_end) *ap++ = *lp++;
    while (ap != a_end) *ap++ = 0;

    if (n1._get_negative()) a._set_negative(true);
    a._cleanup();
    target._attach(a);
}

void sub_subtract(base_integer& target, const base_integer& n1, const
    base_integer& n2)
{
    bool neg1=n1._get_negative();
    bool swap=((neg1 ? 1 : -1) == compare(n1, n2));
    const base_integer &larger(swap ? n2 : n1), &smaller(swap ? n1 : n2);

    // For subtraction, the answer will always be less than or equal to the
    // size of the longest operand.
    integer a;
    a._realloc(larger._get_length());

    // Now subtract the digits, starting at the least-significant one.
    const digit_t *lp = larger._get_digits(), *l_end = lp + larger._get_length(),
        *sp = smaller._get_digits(), *s_end = sp + smaller._get_length();
    digit_t *ap = a._get_writable_digits();

    digit_t borrow = 0;
    while (sp != s_end) {
        doubledigit_t t = digit_overflowbit + *lp++ - *sp++ - borrow;
        if (t < digit_overflowbit) {
            borrow = 1;
            *ap++ = static_cast<digit_t>(t);
        } else {
            borrow = 0;
            *ap++ = static_cast<digit_t>(t - digit_overflowbit);
        }
    }

    if (borrow) {
        while (lp != l_end) {
            if (*lp == 0) { *ap++ = digit_mask; ++lp; }
            else { *ap++ = *lp++ - 1; borrow = 0; break; }
        }
    }
    while (lp != l_end) *ap++ = *lp++;
    if (borrow != 0) swap = !swap;

    if (neg1) a._set_negative(true);
    a._cleanup();

    if (swap) {
        target._attach(neg_t(a));
    } else {
        target._attach(a);
    }
}

void divide_by_single_digit(base_integer& qtarget, base_integer& rtarget, const
    base_integer& d1, digit_t d2)
{
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
    digit_t *qp=quotient._get_writable_digits()+m;
    for (int i = m; i >= 0; --i, --d1p, --qp) {
        a = (a & ~lomask) | *d1p;
        *qp = static_cast<digit_t>(a / d2);
        a = (a & ~himask) | ((a % d2) << bits_per_digit);
    }
    remainder._get_writable_digits()[0] = static_cast<digit_t>((a & himask) >>
        bits_per_digit);

    quotient._cleanup();
    remainder._cleanup();
    qtarget._attach(quotient);
    rtarget._attach(remainder);
}

void sub_divide2(base_integer& qtarget, base_integer& rtarget, const
    base_integer& d1, const base_integer& d2)
{
    assert(d2._get_digit(d2._get_length()-1) >= digit_overflowbit / 2);

    const digit_t *byDigits = d2._get_digits();

    integer quotient, r(d1._to_integer());

    size_t n=d2._get_length(), m=d1._get_length() - n;
    size_t i=m+n, j=m;
    quotient._realloc(j+1);
    digit_t *qdigits=quotient._get_writable_digits();

    r._make_unique();
    do {
        doubledigit_t ri=(doubledigit_t(r._get_digit(i, true))
            << bits_per_digit) + r._get_digit(i-1, true);
        doubledigit_t q=(std::min<doubledigit_t>)(ri / byDigits[n-1],
            digit_mask);

        while (1) {
            // We need three digits here, a doubledigit_t won't suffice.
            doubledigit_t r2a=ri - (q * byDigits[n-1]);
            integer r2=(integer(r2a) << bits_per_digit) + r._get_digit(i-2);
            if (byDigits[n-2] * q <= r2) break;
            --q;
        }

        integer bq;
        multiply(bq, d2, fixed_integer_any(q));
        if (r < bq) {
            --q;
            subtract(bq, temp_t(bq), d2);
        }

        integer rtemp((r >> (bits_per_digit * (i-n))) - bq);

        digit_t *rdigits=r._get_writable_digits();

        size_t rlen=r._get_length();
        for (size_t x=0; x<=n && i-n+x < rlen; ++x)
            rdigits[i-n+x]=rtemp._get_digit(x, true);
        r._cleanup();

        qdigits[j--]=digit_t(q);
    } while (--i >= n);

    quotient._cleanup();

    qtarget._attach(quotient);
    rtarget._attach(r);
}

void sub_divide(base_integer& qtarget, base_integer& rtarget, const
    base_integer& d1, const base_integer& d2)
{
    if (d2._get_length()==1) {
        divide_by_single_digit(qtarget, rtarget, d1, d2._get_digit(0));
        return;
    }

    // The normalization step
    digit_t _d = static_cast<digit_t>(digit_overflowbit /
        (doubledigit_t(d2._get_digit(d2._get_length()-1))+1));
    if (_d > 1) {
        fixed_integer_any d(_d);

        integer dd1, dd2;
        multiply(dd1, d1, d);
        multiply(dd2, d2, d);

        integer q, r;
        sub_divide2(q, r, dd1, dd2);

        // Denormalization step. This requires a division by a single digit_t.
        integer qq, rr;
        divide_by_single_digit(qq, rr, r, _d);
        qtarget._attach(q);
        rtarget._attach(qq);
    } else {
        sub_divide2(qtarget, rtarget, d1, d2);
    }
}

void add(base_integer& target, const base_integer& n1, const base_integer& n2) {
    if (n1._is_zero()) {
        target._attach(n2);
        return;
    } else if (n2._is_zero()) {
        target._attach(n1);
        return;
    } else if (n1._get_negative() != n2._get_negative()) {
        sub_subtract(target, n1, neg_t(n2));
    } else {
        sub_add(target, n1, n2);
    }
}

void subtract(base_integer& target, const base_integer& n1, const base_integer&
    n2)
{
    if (n1._is_zero()) {
        target._attach(neg_t(n2));
    } else if (n2._is_zero()) {
        target._attach(n1);
    } else if (n1._get_negative() != n2._get_negative()) {
        sub_add(target, n1, neg_t(n2));
    } else {
        sub_subtract(target, n1, n2);
    }
}

void multiply(base_integer& target, const base_integer& n, const base_integer&
    by)
{
    if (n._is_zero() || by._is_zero()) {
        target._set_unsigned(0);
        return;
    }

    const digit_t *ndigits=n._get_digits(), *bydigits=by._get_digits();
    if (ndigits==bydigits) {
        sqr(target, n);
        if (n._get_negative() != by._get_negative()) target._set_negative(true);
        return;
    }

    size_t maxdigit = target._is_fixed()
        ? target._fixed_digits()
        : (std::numeric_limits<size_t>::max)();
    size_t nlen=n._get_length(), bylen=by._get_length();

    // In multiplication, we know that the answer will be less than or equal to
    // the sum of the lengths of the operands.
    integer answer;
    answer._realloc(nlen + bylen);
    digit_t *adigits=answer._get_writable_digits(), *ae = adigits +
        (std::min)(nlen + bylen, maxdigit);

    // Now multiply the digits, starting at the least-significant digit.
    const digit_t *d1 = ndigits, *d1e = d1 + (std::min)(nlen, maxdigit);
    for (int digit1 = 0; d1 < d1e; ++digit1, ++d1) {
        const digit_t *d2 = bydigits;
        const digit_t *d2e = d2 + (std::min)(bylen, maxdigit - digit1);
        for (int digit2 = 0; d2 < d2e; ++digit2, ++d2) {
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

            while (carry && a < ae) {
                addt=*a+1;
                *a++ = static_cast<digit_t>(addt);
                if (addt < digit_overflowbit) break;
            }
        }
    }

    answer._set_negative(n._get_negative() != by._get_negative());
    answer._cleanup();
    target._attach(answer);
}

void divide(base_integer& qtarget, base_integer& rtarget, const base_integer&
    d1, const base_integer& d2)
{
    int sign1 = (d1._is_zero() ? 0 : d1._get_negative() ? -1 : 1);
    int sign2 = (d2._is_zero() ? 0 : d2._get_negative() ? -1 : 1);
    if (sign2 == 0) throw exceptions::divide_by_zero();

    int comp=compare(d1, d2, true);
    if (comp < 0) {
        qtarget._attach(integer::zero());
        rtarget._attach(d1);
    } else if (comp == 0) {
        qtarget._attach(fixed_integer_any(sign1 != sign2 ? 1 : -1));
        rtarget._attach(integer::zero());
    } else if (sign1 < 0 && sign2 < 0) {
        sub_divide(qtarget, rtarget, neg_t(d1), neg_t(d2));
        rtarget._set_negative(true);
    } else if (sign1 < 0) {
        sub_divide(qtarget, rtarget, neg_t(d1), d2);
        qtarget._set_negative(true);
        rtarget._set_negative(true);
    } else if (sign2 < 0) {
        sub_divide(qtarget, rtarget, d1, neg_t(d2));
        qtarget._set_negative(true);
    } else if (d2._get_length() == 1) {
        divide_by_single_digit(qtarget, rtarget, d1, d2._get_digit(0));
    } else {
        sub_divide(qtarget, rtarget, d1, d2);
    }
}

void divide(base_integer& target, const base_integer& n1, const base_integer&
    n2)
{
    integer remainder;
    divide(target, remainder, n1, n2);
}

} // namespace detail
//! @endcond detail

/*! \brief Returns the absolute value of an integer.

- Complexity: O(1)

\param[in] n The integer to operate on.

\returns If \c n is zero or positive, returns \c n. Otherwise returns \c -n.
*/
integer abs(const integer& n) {
    if (n._get_negative()) {
        return -n;
    } else {
        return n;
    }
}

/*! \brief Return the additive inverse of an integer.

- Complexity: O(1)

\param[in] n The integer to operate on.

\returns \c -n.
*/
integer negate(const integer& n) {
    integer nn(n);
    nn._toggle_negative();
    return BOOST_XINT_MOVE(nn);
}

/*! \brief Calculate the sum of two integers.

- Complexity: O(n)

\param[in] n1, n2 The integers to add.

\returns The sum of the parameters.
*/
integer add(const integer& n1, const integer& n2) {
    integer r;
    detail::add(r, n1, n2);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Calculate the difference between two integers.

- Complexity: O(n)

\param[in] n1, n2 The integers to operate on.

\returns The difference between the parameters.
*/
integer subtract(const integer& n1, const integer& n2) {
    integer r;
    detail::subtract(r, n1, n2);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Calculate the product of two integers.

- Complexity: O(n<sup>2</sup>)

\param[in] n, by The integers to operate on.

\returns The product of the parameters.

\remarks
Automatically uses the more-efficient squaring algorithm if it can trivially
detect that the two parameters are copies of the same number.
*/
integer multiply(const integer& n, const integer& by) {
    integer r;
    detail::multiply(r, n, by);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Calculate how many \c dividends would fit into \c divisor.

- Complexity: O(n<sup>2</sup>)

\param[in] dividend, divisor The integers to operate on.

\returns The integer value of \c dividend divided by \c divisor.

\exception exceptions::divide_by_zero if \c divisor is zero.
*/
integer divide(const integer& dividend, const integer& divisor) {
    integer q, r;
    detail::divide(q, r, dividend, divisor);
    return BOOST_XINT_MOVE(q);
}

/*! \brief Calculate how many \c dividends would fit into \c divisor, with the
           remainder.

- Complexity: O(n<sup>2</sup>)

\param[in] d1 The dividend.
\param[in] d2 The divisor.

\returns A \c divide_t class containing the quotient and remainder of \c d1
divided by \c d2.

\exception exceptions::divide_by_zero if \c d2 is zero.
*/
integer::divide_t divide_r(const integer& d1, const integer& d2) {
    integer q, r;
    detail::divide(q, r, d1, d2);
    return BOOST_XINT_FORWARD(integer::divide_t, integer::divide_t(
        BOOST_XINT_MOVE(q), BOOST_XINT_MOVE(r)));
}

} // namespace xint
} // namespace boost
