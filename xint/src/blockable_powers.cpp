
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
    \brief Contains the definitions for blockable integer functions related to
           powers of a number.
*/

#include "../boost/xint/xint.hpp"

namespace boost {
namespace xint {
namespace blockable {

//! \copydoc core::pow2
integer pow2(size_t e) {
    integer r;
    setbit(r, e);
    return r;
}

//! \copydoc core::sqr
integer sqr(const integer& n) {
    try {
        return integer(sqr(core::integer(n)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return integer::nan();
    }
}

//! \copydoc core::pow
integer pow(const integer& n, const integer& e) {
    try {
        return integer(pow(core::integer(n), core::integer(e)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return integer::nan();
    }
}

//! \copydoc core::factorial
integer factorial(size_t n) {
    try {
        return integer(core::factorial(n));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return integer::nan();
    }
}

} // namespace blockable
} // namespace xint
} // namespace boost
