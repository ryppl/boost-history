
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

#include "../boost/xint/xint.hpp"

namespace boost {
namespace xint {

//! \copydoc xint::mod(const integer&, const integer&)
nothrow_integer mod(const nothrow_integer& n, const nothrow_integer& m) {
    try {
        return nothrow_integer(mod(xint::integer(n), xint::integer(m)));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::mulmod(const integer&, const integer&, const integer&)
nothrow_integer mulmod(const nothrow_integer& n, const nothrow_integer& by, const nothrow_integer& m) {
    try {
        return nothrow_integer(mod(xint::integer(n) * xint::integer(by),
            xint::integer(m)));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::sqrmod(const integer&, const integer&)
nothrow_integer sqrmod(const nothrow_integer& n, const nothrow_integer& m) {
    try {
        return nothrow_integer(mod(sqr(xint::integer(n)), xint::integer(m)));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::powmod(const integer&, const integer&, const integer&, bool)
nothrow_integer powmod(const nothrow_integer& n, const nothrow_integer& e, const nothrow_integer& m, bool
    noMontgomery)
{
    try {
        return nothrow_integer(powmod(xint::integer(n), xint::integer(e),
            xint::integer(m), noMontgomery));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

/*! \copydoc xint::invmod(const integer&, const integer&)

\note Returns zero instead of throwing.
*/
nothrow_integer invmod(const nothrow_integer& n, const nothrow_integer& m) {
    try {
        return nothrow_integer(invmod(xint::integer(n), xint::integer(m)));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

} // namespace xint
} // namespace boost
