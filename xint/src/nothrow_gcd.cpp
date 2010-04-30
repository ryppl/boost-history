
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
    \brief Greatest Common Denominator and Least Common Multple for the \c
           nothrow_integer type.
*/

#include "../boost/xint/nothrow_integer.hpp"

namespace boost {
namespace xint {

//! \copydoc xint::gcd(const integer&, const integer&)
nothrow_integer gcd(const nothrow_integer& num1, const nothrow_integer& num2) {
    try {
        nothrow_integer r;
        detail::gcd(r, num1, num2);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::lcm(const integer&, const integer&)
nothrow_integer lcm(const nothrow_integer& num1, const nothrow_integer& num2) {
    try {
        nothrow_integer r;
        detail::lcm(r, num1, num2);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

} // namespace xint
} // namespace boost
