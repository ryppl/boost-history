
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
    \brief Contains the definitions for the basic modular math functions for
           the \c nothrow_integer type.
*/

#include "../boost/xint/nothrow_integer.hpp"

namespace boost {
namespace xint {

//! \copydoc xint::mod(const integer&, const integer&)
nothrow_integer mod(const nothrow_integer& n, const nothrow_integer& m) {
    try {
        nothrow_integer r;
        detail::mod(r, n, m);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::mulmod(const integer&, const integer&, const integer&)
nothrow_integer mulmod(const nothrow_integer& n, const nothrow_integer& by, const nothrow_integer& m) {
    try {
        return mod(n * by, m);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::sqrmod(const integer&, const integer&)
nothrow_integer sqrmod(const nothrow_integer& n, const nothrow_integer& m) {
    try {
        return mod(sqr(n), m);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::powmod(const integer&, const integer&, const integer&, bool)
nothrow_integer powmod(const nothrow_integer& n, const nothrow_integer& e, const
    nothrow_integer& m, bool noMontgomery)
{
    try {
        nothrow_integer r;
        detail::powmod(r, n, e, m, noMontgomery);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

/*! \copydoc xint::invmod(const integer&, const integer&)

\note Returns zero instead of throwing.
*/
nothrow_integer invmod(const nothrow_integer& n, const nothrow_integer& m) {
    try {
        nothrow_integer r;
        detail::invmod(r, n, m);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

} // namespace xint
} // namespace boost
