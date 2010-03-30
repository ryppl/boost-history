
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the definitions for functions related to powers of a
    number.
*/

#include "../xint.hpp"
#include "../xint_data_t.hpp"

#include <vector>

namespace xint {

using namespace detail;

namespace {

    bool addOverflow(doubledigit_t &n1, doubledigit_t n2) {
        doubledigit_t t=(n1>>1)+(n2>>1);
        if ((n1&1) && (n2&1)) ++t;
        n1+=n2;
        if (t & doubledigit_hibit) return true;
        return false;
    }

} // namespace

integer pow2(size_t exponent) {
    integer r;
    setbit(r, exponent);
    return r;
}

integer sqr(const integer& n) {
    n._throw_if_nan();

    const data_t *ndata=n._get_data();
    std::vector<doubledigit_t> a(ndata->mLength*2+1, 0);
    doubledigit_t *adigit=&a[0];

    int i, j;
    integer addend;
    data_t *addenddata=addend._get_data();
    addenddata->alloc(ndata->mLength*2+1);

    digit_t *ndigitip=ndata->digits;
    for (i=0; i<ndata->mLength; ++i, ++ndigitip) {
        digit_t ndigiti=*ndigitip;
        doubledigit_t t=(doubledigit_t(ndigiti) * ndigiti);
        if (addOverflow(adigit[2*i], t)) ++addenddata->digits[2*i+2];

        for (j=i+1; j<ndata->mLength; ++j) {
            doubledigit_t t=(doubledigit_t(ndata->digits[j]) * ndigiti);
            if (t & doubledigit_hibit) ++addenddata->digits[i+j+2];
            t<<=1;
            if (addOverflow(adigit[i+j], t)) ++addenddata->digits[i+j+2];
        }
    }

    integer answer;
    data_t *answerdata=answer._get_data();
    answerdata->alloc(ndata->mLength*2+1);

    doubledigit_t carry=0;
    for (i=0, j=ndata->mLength*2+1; i<j; ++i) {
        if (addOverflow(carry, adigit[i])) ++addenddata->digits[i+2];
        answerdata->digits[i]=digit_t(carry);
        carry>>=bits_per_digit;
    }

    answerdata->skipLeadingZeros();
    addenddata->skipLeadingZeros();
    answer+=addend;

    return answer;
}

integer pow(const integer& n, const integer& exponent) {
    bool neg=(n.sign() < 0 && exponent.odd());

    int length=exponent._get_length(), lastBitCount=0;
    digit_t ee(exponent._get_digit(length-1));
    while (ee != 0) { ee >>= 1; ++lastBitCount; }

    integer p(abs(n)), answer=integer::one();
    for (int eIndex=0; eIndex < length; ++eIndex) {
        digit_t e(exponent._get_digit(eIndex));

        int bitCount(eIndex == length-1 ? lastBitCount : bits_per_digit);
        while (bitCount-- > 0) {
            if (e & 0x01) answer*=p;
            p=sqr(p);
            e >>= 1;
        }
    }

    answer._set_negative(neg);
    return answer;
}

integer factorial(size_t n) {
    integer r(integer::one());
    if (n == (std::numeric_limits<size_t>::max)()) {
        // It's highly unlikely that the system will be able to calculate this,
        // or that anyone might want to, but someday it will be possible. This
        // code keeps the function from going into an infinite loop if/when that
        // happens.
        r=(std::numeric_limits<size_t>::max)();
        --n;
    }
    for (size_t i=2; i<=n; ++i) r*=i;
    return r;
}

} // namespace xint
