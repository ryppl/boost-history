
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
#include <functional>

namespace boost {
namespace xint {

namespace core {

int compare(const integer &b1, const integer &b2, bool ignoresign) {
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

} // namespace core

/*! \brief Compare two integer objects.

\param[in] b1, b2 The integers to compare.
\param[in] ignoresign If \c true, the absolute values of b1 and b2 are compared,
instead of their signed values. Used internally.

\returns A negative number if \c b1 < \c b2; zero if \c b1 == \c b2, or a
positive number if \c b1 > \c b2.

This is the function behind all of the comparison operators. It might sometimes
be useful directly as well.

\note If exceptions are blocked, returns 0 instead of throwing.
*/
int compare(const integer &b1, const integer &b2, bool ignoresign) {
    try {
        return compare(core::integer(b1), core::integer(b2));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return 0;
    }
}

namespace {
template <typename T>
bool cmp(const integer &num1, const integer &num2, const T& t) {
    try {
        return t(compare(core::integer(num1), core::integer(num2)), 0);
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return false;
    }
}
} // namespace

bool operator!(const integer &num1) {
    try {
        return operator!(core::integer(num1));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return false;
    }
}

bool operator==(const integer &num1, const integer &num2) {
    return cmp(num1, num2, std::equal_to<int>()); }
bool operator!=(const integer& num1, const integer& num2) {
    return cmp(num1, num2, std::not_equal_to<int>()); }
bool operator<(const integer& num1, const integer& num2) {
    return cmp(num1, num2, std::less<int>()); }
bool operator>(const integer& num1, const integer& num2) {
    return cmp(num1, num2, std::greater<int>()); }
bool operator<=(const integer& num1, const integer& num2) {
    return cmp(num1, num2, std::less_equal<int>()); }
bool operator>=(const integer& num1, const integer& num2) {
    return cmp(num1, num2, std::greater_equal<int>()); }

} // namespace xint
} // namespace boost
