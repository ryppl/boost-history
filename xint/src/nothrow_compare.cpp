
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
    \brief Contains the the compare function and comparison operators for
           blockable integers.
*/

#include "../boost/xint/xint.hpp"

namespace boost {
namespace xint {
namespace blockable {

/*! \copydoc core::compare

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

} // namespace blockable
} // namespace xint
} // namespace boost
