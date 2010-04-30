
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

#include "../boost/xint/integer.hpp"
#include <functional>

namespace boost {
namespace xint {

namespace detail {
int compare(const base_integer &b1, const base_integer &b2, bool ignoresign) {
    bool invert_answer=false;
    if (!ignoresign) {
        if (b1._is_zero()) {
            if (b2._is_zero()) return 0;
            return (b2._get_negative() ? 1 : -1);
        } else if (b2._is_zero()) {
            return (b1._get_negative() ? -1 : 1);
        } else {
            if (b1._get_negative() != b2._get_negative())
                return (b1._get_negative() ? -1 : 1);
            if (b1._get_negative()) invert_answer=true;
        }
    }

    int answer=0;
    size_t len=b1._get_length(), len2=b2._get_length();
    if (len != len2) {
        answer=((len < len2) ? -1 : 1);
    } else {
        const detail::digit_t *b1d=b1._get_digits(), *b1i=b1d+len;
        const detail::digit_t *b2d=b2._get_digits(), *b2i=b2d+len;
        while (b1i > b1d)
            if (*(--b1i) != *(--b2i)) {
                answer = ((*b1i < *b2i) ? -1 : 1);
                break;
            }
    }

    return (invert_answer ? -answer : answer);
}
} // namespace detail

/*! \brief Compare two integer objects.

- Complexity: O(n) at worst

\param[in] b1, b2 The integers to compare.
\param[in] ignoresign If \c true, the absolute values of b1 and b2 are compared,
instead of their signed values. Used internally.

\returns A negative number if \c b1 < \c b2; zero if \c b1 == \c b2, or a
positive number if \c b1 > \c b2.
*/
int compare(const integer &b1, const integer &b2, bool ignoresign) {
    return detail::compare(b1, b2, ignoresign);
}

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

/*! \brief Compare two fixed_integer objects.

- Complexity: O(n) at worst

\param[in] b1, b2 The integers to compare. They can be the same size or
different sizes.
\param[in] ignoresign If \c true, the absolute values of b1 and b2 are compared,
instead of their signed values. Used internally.

\returns A negative number if \c b1 < \c b2; zero if \c b1 == \c b2, or a
positive number if \c b1 > \c b2.
*/
int compare(const fixed_integer_any& b1, const fixed_integer_any& b2, bool
    ignoresign)
{
    return detail::compare(b1, b2, ignoresign);
}

bool operator==(const fixed_integer_any &num1, const fixed_integer_any &num2) {
    return compare(num1, num2)==0; }
bool operator!=(const fixed_integer_any& num1, const fixed_integer_any& num2) {
    return compare(num1, num2)!=0; }
bool operator<(const fixed_integer_any& num1, const fixed_integer_any& num2) {
    return compare(num1, num2)<0; }
bool operator>(const fixed_integer_any& num1, const fixed_integer_any& num2) {
    return compare(num1, num2)>0; }
bool operator<=(const fixed_integer_any& num1, const fixed_integer_any& num2) {
    return compare(num1, num2)<=0; }
bool operator>=(const fixed_integer_any& num1, const fixed_integer_any& num2) {
    return compare(num1, num2)>=0; }

} // namespace xint
} // namespace boost
