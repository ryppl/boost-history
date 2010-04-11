
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
    \brief Greatest Common Denominator and Least Common Multple for blockable
           integers.

    Also holds the definition for invmod, since it uses the GCD algorithm.
*/

#include "../boost/xint/xint.hpp"

namespace boost {
namespace xint {
namespace blockable {

//! \copydoc core::gcd
integer gcd(const integer& num1, const integer& num2) {
    try {
        return integer(gcd(core::integer(num1), core::integer(num2)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return integer::nan();
    }
}

//! \copydoc core::lcm
integer lcm(const integer& num1, const integer& num2) {
    try {
        return integer(lcm(core::integer(num1), core::integer(num2)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return integer::nan();
    }
}

/*! \copydoc core::invmod

\note If exceptions are blocked, it returns zero instead of throwing an
exception.
*/
integer invmod(const integer& n, const integer& m) {
    try {
        return integer(invmod(core::integer(n), core::integer(m)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return integer::nan();
    }
}

} // namespace blockable
} // namespace xint
} // namespace boost
