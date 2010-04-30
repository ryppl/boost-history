
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
    \brief Contains the definitions for \c nothrow_integer functions related to
           powers of a number.
*/

#include "../boost/xint/nothrow_integer.hpp"

namespace boost {
namespace xint {

//! \copydoc xint::pow2
nothrow_integer nothrow_pow2(size_t e) {
    nothrow_integer r;
    setbit(r, e);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc xint::sqr(const integer&)
nothrow_integer sqr(const nothrow_integer& n) {
    try {
        nothrow_integer r;
        detail::sqr(r, n);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::pow(const integer&, const integer&)
nothrow_integer pow(const nothrow_integer& n, const nothrow_integer& e) {
    try {
        nothrow_integer r;
        detail::pow(r, n, e);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::factorial
nothrow_integer nothrow_factorial(size_t n) {
    try {
        nothrow_integer r;
        detail::factorial(r, n);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

} // namespace xint
} // namespace boost
