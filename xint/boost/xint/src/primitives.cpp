
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the definitions for math primitives.
*/

#include "../xint.hpp"
#include "../xint_data_t.hpp"
#include <cassert>

namespace xint {

using namespace detail;

integer abs(const integer& n) {
    return (n < 0 ? -n : n);
}

integer negate(const integer& _n) {
    _n._throw_if_nan();

    integer n(_n);
    n._make_unique();
    n._get_data()->negate();
    return n;
}

integer add(const integer& n1, const integer& n2) {
    if (n1.sign()==0) return n2;
    if (n2.sign()==0) return n1;

    bool swapped=false;
    const data_t *n1data=n1._get_data(), *n2data=n2._get_data();
    if (n1data->mLength < n2data->mLength) { swapped=true; std::swap(n1data, n2data); }

    integer r;
    data_t *rdata=r._get_data();
    rdata->copy(n1data, 1);

    if (n1.sign() != n2.sign()) {
        int level=n2data->mLength;

        integer _n2(swapped ? n1 : n2);
        _n2._make_unique();
        _n2._get_data()->invert();

        rdata->add(_n2._get_data());

        if (rdata->mLength > level) {
            --rdata->digits[level];
            rdata->skipLeadingZeros();
        } else rdata->invert();
    } else {
        rdata->add(*n2data);
    }
    return r;
}

integer subtract(const integer& n1, const integer& n2) {
    if ((n1.sign() < 0) != (n2.sign() < 0)) {
        return add(n1, -n2);
    } else if (n1.sign() < 0) {
        return -subtract(-n1, -n2);
    }

    // Signs are both guaranteed positive now.

    if (n1 < n2) {
        return -subtract(n2, n1);
    } else {
        integer r;
        data_t *rdata=r._get_data();
        rdata->copy(n1._get_data());
        rdata->subtract(*n2._get_data());
        return r;
    }
}

integer multiply(const integer& n, const integer& by) {
    if (n.sign()==0 || by.sign()==0) return integer::zero();

    const data_t *ndata=n._get_data(), *bydata=by._get_data();
    if (ndata == bydata) return sqr(n);

    // In multiplication, we know that the answer will be less than or equal to
    // the sum of the lengths of the operands.
    integer answer;
    data_t *answerdata=answer._get_data();
    answerdata->alloc(ndata->mLength + bydata->mLength);

    // Now multiply the digits, starting at the least-significant digit.
    const digit_t *d1 = ndata->digits, *d1e = d1 + ndata->mLength;
    const digit_t *d2e = bydata->digits + bydata->mLength;
    for (int digit1=0; d1<d1e; ++digit1, ++d1) {
        const digit_t *d2=bydata->digits;
        for (int digit2=0; d2<d2e; ++digit2, ++d2) {
            // First multiply the two digits
            doubledigit_t t=doubledigit_t(*d1) * *d2;

            // Now add the result to the answer.
            int carry=0;
            digit_t *a = answerdata->digits + digit1 + digit2;
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

    answer._set_negative(n.sign() != by.sign());
    answer._get_data()->skipLeadingZeros();
    return answer;
}

namespace {

std::pair<integer, integer> divideBySingleDigit(const integer& d1, digit_t d2) {
    const data_t *d1data=d1._get_data();

    integer quotient, remainder;
    data_t *qdata=quotient._get_data(), *rdata=remainder._get_data();
    qdata->alloc(d1data->mLength);
    rdata->alloc(1);

    doubledigit_t a=0;
    const doubledigit_t lomask(digit_mask);
    const doubledigit_t himask(doubledigit_t(digit_mask) << bits_per_digit);

    int m = d1data->mLength - 1;
    const digit_t *d1p=d1data->digits+m;
    digit_t *qp=qdata->digits+m;
    for (int i = m; i >= 0; --i, --d1p, --qp) {
        a = (a & ~lomask) | *d1p;
        *qp = static_cast<digit_t>(a / d2);
        a = (a & ~himask) | ((a % d2) << bits_per_digit);
    }
    rdata->digits[0] = static_cast<digit_t>((a & himask) >>
        bits_per_digit);

    qdata->skipLeadingZeros();
    rdata->skipLeadingZeros();
    return std::make_pair(quotient, remainder);
}

std::pair<integer, integer> subDivide(integer d1, integer d2) {
    const data_t *ndata=d1._get_data(), *bydata=d2._get_data();
    const digit_t *nDigits = ndata->digits, *byDigits = bydata->digits;
    int nMSD = ndata->mLength-1, byMSD = bydata->mLength-1;

    // The normalization step
    digit_t d = static_cast<digit_t>(digit_overflowbit /
        (doubledigit_t(byDigits[byMSD])+1));
    if (d > 1) {
        d1 *= d;
        d2 *= d;

        // Gotta reset these
        ndata = d1._get_data();
        bydata = d2._get_data();
        nDigits = ndata->digits;
        byDigits = bydata->digits;
        nMSD = ndata->mLength-1;
        byMSD = bydata->mLength-1;

    }
    assert(byDigits[byMSD] >= digit_overflowbit / 2);

    // We have to figure out which digit is going to be the first one. We'll do
    // that by cutting the least-significant digits of the dividend until it
    // has the same number of digits as the divisor; if what remains is greater
    // than the divisor, then we start there, otherwise that one's zero and we
    // start on the next lower one.
    int highestQuotientDigit=(nMSD - byMSD);
    integer nTest(d1);
    nTest -= integer::one();
    nTest >>= (bits_per_digit * highestQuotientDigit);
    if (nTest < d2) --highestQuotientDigit;

    integer quotient, w;
    data_t *qdata=quotient._get_data();
    qdata->alloc(highestQuotientDigit + 1);

    for (int j = highestQuotientDigit; j >= 0; --j) {
        doubledigit_t q = (nDigits[nMSD] > byDigits[byMSD] ?
            doubledigit_t(nDigits[nMSD]) / byDigits[byMSD] :
            ((doubledigit_t(nDigits[nMSD]) << bits_per_digit) +
            nDigits[nMSD-1]) / byDigits[byMSD]);

        digit_t loopcount=0;
        while (1) {
            w=(d2 * q);
            w <<= (bits_per_digit * j);

            w = d1 - w;
            if (w.sign() < 0) {
                --q;

                // Our estimate should only be off by a maximum of two, so this
                // while loop should only run twice each time, at most.
                assert(++loopcount <= 2);
            } else break;
        }

        qdata->digits[j] = static_cast<digit_t>(q);
        if (w < d2) break;

        d1 = w;
        ndata=d1._get_data();
        nDigits = ndata->digits;
        nMSD = ndata->mLength-1;
    }

    if (d > 1) {
        // Denormalization step. This requires a division by a single digit_t.
        integer remainder=divideBySingleDigit(w, d).first;
        return std::make_pair(quotient, remainder);
    } else {
        return std::make_pair(quotient, w);
    }
}

} // namespace

integer divide(const integer& dividend, const integer& divisor) {
    return divide_r(dividend, divisor).first;
}

std::pair<integer, integer> divide_r(const integer& d1, const
    integer& d2)
{
    int sign1=d1.sign(), sign2=d2.sign();
    if (sign2==0) {
        if (exceptions_allowed()) throw divide_by_zero();
        else return std::make_pair(integer(not_a_number()), integer(not_a_number()));
    }

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

    if (d2._get_data()->mLength == 1) {
        return divideBySingleDigit(d1, d2._get_data()->digits[0]);
    } else {
        return subDivide(d1, d2);
    }
}

} // namespace xint
