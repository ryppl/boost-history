
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
#include <functional>

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
    if (!ignoresign) {
        int sign1=b1.sign(), sign2=b2.sign();
        if (sign1==0 && sign2==0) return 0;
        if (sign1==0) return -sign2;
        if (sign2==0) return sign1;

        if (sign1 != sign2) return sign1;
        if (sign1 < 0) return compare(-b2, -b1, ignoresign);
    }

    if (b1._get_length() != b2._get_length()) {
        return ((b1._get_length() < b2._get_length()) ? -1 : 1);
    } else {
        size_t len=b1._get_length();
        const detail::digit_t *b1d=b1._get_digits(), *b1i=b1d+len;
        const detail::digit_t *b2d=b2._get_digits(), *b2i=b2d+len;
        while (b1i > b1d)
            if (*(--b1i) != *(--b2i))
                return ((*b1i < *b2i) ? -1 : 1);
    }

    return 0;
}

bool operator!(const integer &num1) { return num1.sign()==0; }
bool operator==(const integer &num1, const integer &num2) {
    return compare(num1, num2)==0; }
bool operator!=(const integer& num1, const integer& num2) {
    return compare(num1, num2)!=0; }
bool operator<(const integer& num1, const integer& num2) {
    return compare(num1, num2)<0; }
bool operator>(const integer& num1, const integer& num2) {
    return compare(num1, num2)>0; }
bool operator<=(const integer& num1, const integer& num2) {
    return compare(num1, num2)<=0; }
bool operator>=(const integer& num1, const integer& num2) {
    return compare(num1, num2)>=0; }

} // namespace xint
} // namespace boost
