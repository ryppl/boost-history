
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
    \brief Contains the definitions for math primitives for the \c
           nothrow_integer type.
*/

#include "../boost/xint/xint.hpp"

namespace boost {
namespace xint {

//! \copydoc xint::abs(const integer&)
nothrow_integer abs(const nothrow_integer& n) {
    try {
        return nothrow_integer(abs(xint::integer(n)));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::negate(const integer&)
nothrow_integer negate(const nothrow_integer& n) {
    try {
        return nothrow_integer(negate(xint::integer(n)));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::add(const integer&, const integer&)
nothrow_integer add(const nothrow_integer& n1, const nothrow_integer& n2) {
    try {
        return nothrow_integer(add(xint::integer(n1), xint::integer(n2)));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::subtract(const integer&, const integer&)
nothrow_integer subtract(const nothrow_integer& n1, const nothrow_integer& n2) {
    try {
        return nothrow_integer(subtract(xint::integer(n1), xint::integer(n2)));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::multiply(const integer&, const integer&)
nothrow_integer multiply(const nothrow_integer& n, const nothrow_integer& by) {
    try {
        return nothrow_integer(multiply(xint::integer(n), xint::integer(by)));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::divide(const integer&, const integer&)
nothrow_integer divide(const nothrow_integer& dividend, const nothrow_integer& divisor) {
    return divide_r(dividend, divisor).first;
}

/*! \copydoc xint::divide_r(const integer&, const integer&)

\note Returns an std::pair with two Not-a-Number values instead of throwing.
*/
std::pair<nothrow_integer, nothrow_integer> divide_r(const nothrow_integer& d1, const nothrow_integer& d2) {
    try {
        return divide_r(xint::integer(d1), xint::integer(d2));
    } catch (std::exception&) {
        return std::make_pair(nothrow_integer::nan(), nothrow_integer::nan());
    }
}

} // namespace xint
} // namespace boost
