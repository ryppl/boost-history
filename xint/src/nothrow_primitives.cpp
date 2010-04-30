
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

#include "../boost/xint/nothrow_integer.hpp"

namespace boost {
namespace xint {

//! \copydoc xint::abs(const integer&)
nothrow_integer abs(const nothrow_integer& n) {
    try {
        if (n._get_negative()) {
            return -n;
        } else {
            return n;
        }
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::negate(const integer&)
nothrow_integer negate(const nothrow_integer& n) {
    try {
        if (n.is_nan()) return n;

        nothrow_integer nn(n);
        nn._toggle_negative();
        return BOOST_XINT_MOVE(nn);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::add(const integer&, const integer&)
nothrow_integer add(const nothrow_integer& n1, const nothrow_integer& n2) {
    try {
        nothrow_integer r;
        detail::add(r, n1, n2);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::subtract(const integer&, const integer&)
nothrow_integer subtract(const nothrow_integer& n1, const nothrow_integer& n2) {
    try {
        nothrow_integer r;
        detail::subtract(r, n1, n2);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::multiply(const integer&, const integer&)
nothrow_integer multiply(const nothrow_integer& n, const nothrow_integer& by) {
    try {
        nothrow_integer r;
        detail::multiply(r, n, by);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::divide(const integer&, const integer&)
nothrow_integer divide(const nothrow_integer& dividend, const nothrow_integer&
    divisor)
{
    nothrow_integer::divide_t answer(divide_r(dividend, divisor));
    return BOOST_XINT_MOVE(answer.quotient);
}

/*! \copydoc xint::divide_r(const integer&, const integer&)

\note Returns two Not-a-Number values instead of throwing.
*/
nothrow_integer::divide_t divide_r(const nothrow_integer& d1, const
    nothrow_integer& d2)
{
    try {
        nothrow_integer q, r;
        detail::divide(q, r, d1, d2);
        return BOOST_XINT_FORWARD(nothrow_integer::divide_t,
            nothrow_integer::divide_t( BOOST_XINT_MOVE(q), BOOST_XINT_MOVE(r)));
    } catch (std::exception&) {
        return BOOST_XINT_FORWARD(nothrow_integer::divide_t,
            nothrow_integer::divide_t(nothrow_integer::nan(),
            nothrow_integer::nan()));
    }
}

} // namespace xint
} // namespace boost
