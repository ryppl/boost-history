
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
           blockable integers.

    The xint::invmod function, which logically would be here, is actually in
    gcd.cpp to share the gcd_core function.
*/

#include "../boost/xint/xint.hpp"

namespace boost {
namespace xint {
namespace blockable {

//! \copydoc core::mod
integer mod(const integer& n, const integer& m) {
    try {
        return integer(mod(core::integer(n), core::integer(m)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        return integer::nan();
    }
}

//! \copydoc core::mulmod
integer mulmod(const integer& n, const integer& by, const integer& m) {
    try {
        return integer(mod(core::integer(n) * core::integer(by),
            core::integer(m)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        return integer::nan();
    }
}

//! \copydoc core::sqrmod
integer sqrmod(const integer& n, const integer& m) {
    try {
        return integer(mod(sqr(core::integer(n)), core::integer(m)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        return integer::nan();
    }
}

//! \copydoc core::powmod
integer powmod(const integer& n, const integer& e, const integer& m, bool
    noMontgomery)
{
    try {
        return integer(powmod(core::integer(n), core::integer(e),
            core::integer(m), noMontgomery));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        return integer::nan();
    }
}

} // namespace blockable
} // namespace xint
} // namespace boost
