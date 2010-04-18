
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

#include "../boost/xint/xint.hpp"

namespace boost {
namespace xint {

/*! \copydoc xint::getbit

\note Returns false instead of throwing.
*/
bool getbit(const nothrow_integer& n, size_t bit) {
    try {
        return getbit(xint::integer(n), bit);
    } catch (std::exception&) {
        return false;
    }
}

//! \copydoc xint::setbit
void setbit(nothrow_integer& n, size_t bit) {
    try {
        xint::integer nn(n);
        setbit(nn, bit);
        n=nothrow_integer(nn);
    } catch (std::exception&) {
        // Do nothing
    }
}

//! \copydoc xint::clearbit
void clearbit(nothrow_integer& n, size_t bit) {
    try {
        xint::integer nn(n);
        clearbit(nn, bit);
        n=nothrow_integer(nn);
    } catch (std::exception&) {
        // Do nothing
    }
}

/*! \copydoc xint::lowestbit

\note Returns zero instead of throwing.
*/
size_t lowestbit(const nothrow_integer& n, size_t valueIfZero) {
    try {
        return lowestbit(xint::integer(n), valueIfZero);
    } catch (std::exception&) {
        return 0;
    }
}

/*! \copydoc xint::highestbit

\note Returns zero instead of throwing.
*/
size_t highestbit(const nothrow_integer& n, size_t valueIfZero) {
    try {
        return highestbit(xint::integer(n), valueIfZero);
    } catch (std::exception&) {
        return 0;
    }
}

//! \copydoc xint::bitwise_and
nothrow_integer bitwise_and(const nothrow_integer& n1, const nothrow_integer& n2) {
    try {
        return nothrow_integer(bitwise_and(xint::integer(n1), xint::integer(n2)));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::bitwise_or
nothrow_integer bitwise_or(const nothrow_integer& n1, const nothrow_integer& n2) {
    try {
        return nothrow_integer(bitwise_or(xint::integer(n1), xint::integer(n2)));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::bitwise_xor
nothrow_integer bitwise_xor(const nothrow_integer& n1, const nothrow_integer& n2) {
    try {
        return nothrow_integer(bitwise_xor(xint::integer(n1), xint::integer(n2)));
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
        return nothrow_integer(shift_left(xint::integer(n), byBits));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

//! \copydoc xint::shift_right
nothrow_integer shift_right(const nothrow_integer& n, size_t byBits) {
    try {
        return nothrow_integer(shift_right(xint::integer(n), byBits));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

} // namespace xint
} // namespace boost
