
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
    \brief Contains the definitions for math primitives for blockable integers.
*/

#include "../boost/xint/xint.hpp"

namespace boost {
namespace xint {
namespace blockable {

//! \copydoc core::abs
integer abs(const integer& n) {
    try {
        return integer(abs(core::integer(n)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        return integer::nan();
    }
}

//! \copydoc core::negate
integer negate(const integer& n) {
    try {
        return integer(negate(core::integer(n)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        return integer::nan();
    }
}

//! \copydoc core::add
integer add(const integer& n1, const integer& n2) {
    try {
        return integer(add(core::integer(n1), core::integer(n2)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        return integer::nan();
    }
}

//! \copydoc core::subtract
integer subtract(const integer& n1, const integer& n2) {
    try {
        return integer(subtract(core::integer(n1), core::integer(n2)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        return integer::nan();
    }
}

//! \copydoc core::multiply
integer multiply(const integer& n, const integer& by) {
    try {
        return integer(multiply(core::integer(n), core::integer(by)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        return integer::nan();
    }
}

//! \copydoc core::divide
integer divide(const integer& dividend, const integer& divisor) {
    return divide_r(dividend, divisor).first;
}

/*! \copydoc core::divide_r

\note If exceptions are blocked, returns an std::pair with two Not-a-Number
values instead of throwing.
*/
std::pair<integer, integer> divide_r(const integer& d1, const integer& d2) {
    try {
        return divide_r(core::integer(d1), core::integer(d2));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        return std::make_pair(integer::nan(), integer::nan());
    }
}

} // namespace blockable
} // namespace xint
} // namespace boost
