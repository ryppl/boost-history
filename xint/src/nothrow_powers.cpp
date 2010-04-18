
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

#include "../boost/xint/xint.hpp"

namespace boost {
namespace xint {

//! \copydoc xint::pow2
nothrow_integer nothrow_pow2(size_t e) {
    nothrow_integer r;
    setbit(r, e);
    return r;
}

//! \copydoc xint::sqr(const integer&)
nothrow_integer sqr(const nothrow_integer& n) {
    try {
        return nothrow_integer(sqr(xint::integer(n)));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::pow(const integer&, const integer&)
nothrow_integer pow(const nothrow_integer& n, const nothrow_integer& e) {
    try {
        return nothrow_integer(pow(xint::integer(n), xint::integer(e)));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::factorial
nothrow_integer nothrow_factorial(size_t n) {
    try {
        return nothrow_integer(xint::factorial(n));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

} // namespace xint
} // namespace boost
