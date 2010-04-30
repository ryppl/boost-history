
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
    \brief Contains the bit-manipulation functions for the \c nothrow_integer
           type.
*/

#include "../boost/xint/nothrow_integer.hpp"

namespace boost {
namespace xint {

/*! \copydoc xint::getbit

\note Returns false instead of throwing.
*/
bool getbit(const nothrow_integer& n, size_t bit) {
    try {
        return detail::getbit(n, bit);
    } catch (std::exception&) {
        return false;
    }
}

//! \copydoc xint::setbit
void setbit(nothrow_integer& n, size_t bit) {
    try {
        detail::setbit(n, bit);
    } catch (std::exception&) {
        // Do nothing
    }
}

//! \copydoc xint::clearbit
void clearbit(nothrow_integer& n, size_t bit) {
    try {
        detail::clearbit(n, bit);
    } catch (std::exception&) {
        // Do nothing
    }
}

/*! \copydoc xint::lowestbit

\note Returns zero instead of throwing.
*/
size_t lowestbit(const nothrow_integer& n, size_t valueIfZero) {
    try {
        return detail::lowestbit(n, valueIfZero);
    } catch (std::exception&) {
        return 0;
    }
}

/*! \copydoc xint::highestbit

\note Returns zero instead of throwing.
*/
size_t highestbit(const nothrow_integer& n, size_t valueIfZero) {
    try {
        return detail::highestbit(n, valueIfZero);
    } catch (std::exception&) {
        return 0;
    }
}

//! \copydoc xint::bitwise_and
nothrow_integer bitwise_and(const nothrow_integer& n1, const nothrow_integer& n2) {
    try {
        nothrow_integer r;
        detail::bitwise_and(r, n1, n2);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::bitwise_or
nothrow_integer bitwise_or(const nothrow_integer& n1, const nothrow_integer& n2) {
    try {
        nothrow_integer r;
        detail::bitwise_or(r, n1, n2);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::bitwise_xor
nothrow_integer bitwise_xor(const nothrow_integer& n1, const nothrow_integer& n2) {
    try {
        nothrow_integer r;
        detail::bitwise_xor(r, n1, n2);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::shift
nothrow_integer shift(const nothrow_integer& n, int byBits) {
    if (byBits > 0) return shift_left(n, byBits);
    else return shift_right(n, -byBits);
}

//! \copydoc xint::shift_left
nothrow_integer shift_left(const nothrow_integer& n, size_t byBits) {
    try {
        nothrow_integer r;
        detail::shift_left(r, n, byBits);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::shift_right
nothrow_integer shift_right(const nothrow_integer& n, size_t byBits) {
    try {
        nothrow_integer r;
        detail::shift_right(r, n, byBits);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

} // namespace xint
} // namespace boost
