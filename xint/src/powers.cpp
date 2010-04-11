
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
    \brief Contains the definitions for functions related to powers of a number.
*/

#include "../boost/xint/xint.hpp"
#include "../boost/xint/xint_data_t.hpp"

#include <vector>

namespace boost {
namespace xint {

using namespace detail;

namespace core {
namespace {

    bool addOverflow(doubledigit_t &n1, doubledigit_t n2) {
        doubledigit_t t=(n1>>1)+(n2>>1);
        if ((n1&1) && (n2&1)) ++t;
        n1+=n2;
        if (t & doubledigit_hibit) return true;
        return false;
    }

} // namespace

/*! \brief Calculate the value of 2<sup>e</sup>

\param[in] e The exponent to operate on.

\returns 2 to the power of \c e.

\remarks
This is a convenience function, to help with self-documenting code.
*/
integer pow2(size_t e) {
    integer r;
    setbit(r, e);
    return r;
}

/*! \brief Calculate the value of n<sup>2</sup>

\param[in] n The integer to operate on.

\returns \c n times \c n.

This function uses a faster algorithm than the standard multiplication one.

\todo Rewrite this to eliminate the inefficient addOverflow.
*/
integer sqr(const integer& n) {
    const data_t *ndata=n._get_data();
    std::vector<doubledigit_t> a(ndata->mLength*2+1, 0);
    doubledigit_t *adigit=&a[0];

    size_t i, j;
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

/*! \brief Calculate the value of n<sup>e</sup>

\param[in] n, e The integers to operate on.

\returns \c n to the power of \c e.
*/
integer pow(const integer& n, const integer& e) {
    bool neg=(n.sign() < 0 && e.odd());

    size_t length=e._get_length(), lastBitCount=0;
    digit_t ee(e._get_digit(length-1));
    while (ee != 0) { ee >>= 1; ++lastBitCount; }

    integer p(abs(n)), answer=integer::one();
    for (size_t eIndex=0; eIndex < length; ++eIndex) {
        digit_t eee(e._get_digit(eIndex));

        int bitCount(int(eIndex == length-1 ? lastBitCount : bits_per_digit));
        while (bitCount-- > 0) {
            if (eee & 0x01) answer*=p;
            p=sqr(p);
            eee >>= 1;
        }
    }

    answer._set_negative(neg);
    return answer;
}

/*! \brief Calculate the value of \c n!

\param[in] n The value to operate on.

\returns \c n factorial, defined as <code>1*2*3*...*n</code>

\warning
Factorials get ridiculously huge, even with fairly small values of \c n. This
function, when used with a large parameter, is the easiest way to exhaust the
system's memory.
*/
integer factorial(size_t n) {
    integer r(integer::one());
    while (n > 1) { r *= n--; }
    return r;
}

} // namespace core
} // namespace xint
} // namespace boost
