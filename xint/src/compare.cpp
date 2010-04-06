
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
    \brief Contains the the compare function and comparison operators.
*/

#include "../boost/xint/xint.hpp"
#include "../boost/xint/xint_data_t.hpp"

namespace boost {
namespace xint {

/*! \brief Compare two integer objects.

\param[in] b1, b2 The integers to compare.
\param[in] ignoresign If \c true, the absolute values of b1 and b2 are compared,
instead of their signed values. Used internally.

\returns A negative number if \c b1 < \c b2; zero if \c b1 == \c b2, or a
positive number if \c b1 > \c b2.

This is the function behind all of the comparison operators. It might sometimes
be useful directly as well.
*/
int compare(const integer &b1, const integer &b2, bool ignoresign) {
    b1._throw_if_nan();
    b2._throw_if_nan();

    if (!ignoresign) {
        int sign1=b1.sign(), sign2=b2.sign();
        if (sign1==0 && sign2==0) return 0;
        if (sign1==0) return -sign2;
        if (sign2==0) return sign1;

        if (sign1 != sign2) return sign1;
        if (sign1 < 0) return compare(-b2, -b1, ignoresign);
    }

    const detail::data_t *b1data=b1._get_data();
    const detail::data_t *b2data=b2._get_data();

    int answer=0;
    if (b1data->mLength != b2data->mLength) {
        answer=((b1data->mLength < b2data->mLength) ? -1 : 1);
    } else {
        for (int x = int(b1data->mLength) - 1; x >= 0; --x) {
            if (b1data->digits[x] != b2data->digits[x]) {
                answer=((b1data->digits[x] < b2data->digits[x]) ? -1 : 1);
                break;
            }
        }
    }

    return answer;
}

bool operator!(const xint::integer &num1) { return num1.sign()==0; }
bool operator==(const xint::integer &num1, const xint::integer &num2) { return xint::compare(num1, num2)==0; }
bool operator!=(const xint::integer& num1, const xint::integer& num2) { return xint::compare(num1, num2)!=0; }
bool operator<(const xint::integer& num1, const xint::integer& num2) { return xint::compare(num1, num2)<0; }
bool operator>(const xint::integer& num1, const xint::integer& num2) { return xint::compare(num1, num2)>0; }
bool operator<=(const xint::integer& num1, const xint::integer& num2) { return xint::compare(num1, num2)<=0; }
bool operator>=(const xint::integer& num1, const xint::integer& num2) { return xint::compare(num1, num2)>=0; }

} // namespace xint
} // namespace boost
