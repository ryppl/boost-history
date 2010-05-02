
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
    \brief Contains the the compare function and comparison operators for the \c
           nothrow_integer type.
*/

#include "../boost/xint/nothrow_integer.hpp"

namespace boost {
namespace xint {

/*! \copydoc xint::compare(const integer&, const integer&, bool)

\note Returns 0 instead of throwing.
*/
int compare(const nothrow_integer &b1, const nothrow_integer &b2, bool ignoresign) {
    if (b1.is_nan() || b2.is_nan()) return 0;
    return detail::compare(b1, b2);
}

//! @cond detail
namespace detail {
template <typename T>
bool cmp(const nothrow_integer &b1, const nothrow_integer &b2, const T& t) {
    if (b1.is_nan() || b2.is_nan()) return false;
    return t(detail::compare(b1, b2), 0);
}
} // namespace detail
//! @endcond detail

bool operator==(const nothrow_integer &num1, const nothrow_integer &num2) {
    return detail::cmp(num1, num2, std::equal_to<int>()); }
bool operator!=(const nothrow_integer& num1, const nothrow_integer& num2) {
    return detail::cmp(num1, num2, std::not_equal_to<int>()); }
bool operator<(const nothrow_integer& num1, const nothrow_integer& num2) {
    return detail::cmp(num1, num2, std::less<int>()); }
bool operator>(const nothrow_integer& num1, const nothrow_integer& num2) {
    return detail::cmp(num1, num2, std::greater<int>()); }
bool operator<=(const nothrow_integer& num1, const nothrow_integer& num2) {
    return detail::cmp(num1, num2, std::less_equal<int>()); }
bool operator>=(const nothrow_integer& num1, const nothrow_integer& num2) {
    return detail::cmp(num1, num2, std::greater_equal<int>()); }

} // namespace xint
} // namespace boost
