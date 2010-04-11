
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
    \brief Contains the bit-manipulation functions for blockable integers.
*/

#include "../boost/xint/xint.hpp"

namespace boost {
namespace xint {
namespace blockable {

/*! \copydoc core::getbit

\note If exceptions are blocked, returns false instead of throwing.
*/
bool getbit(const integer& n, size_t bit) {
    try {
        return getbit(core::integer(n), bit);
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return false;
    }
}

//! \copydoc core::setbit
void setbit(integer& n, size_t bit) {
    try {
        core::integer nn(n);
        setbit(nn, bit);
        n=integer(nn);
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
    }
}

//! \copydoc core::clearbit
void clearbit(integer& n, size_t bit) {
    try {
        core::integer nn(n);
        clearbit(nn, bit);
        n=integer(nn);
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
    }
}

/*! \copydoc core::lowestbit

\note If exceptions are blocked, returns zero instead of throwing.
*/
size_t lowestbit(const integer& n, size_t valueIfZero) {
    try {
        return lowestbit(core::integer(n), valueIfZero);
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return 0;
    }
}

/*! \copydoc core::highestbit

\note If exceptions are blocked, returns zero instead of throwing.
*/
size_t highestbit(const integer& n, size_t valueIfZero) {
    try {
        return highestbit(core::integer(n), valueIfZero);
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return 0;
    }
}

//! \copydoc core::bitwise_and
integer bitwise_and(const integer& n1, const integer& n2) {
    try {
        return integer(bitwise_and(core::integer(n1), core::integer(n2)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return integer::nan();
    }
}

//! \copydoc core::bitwise_or
integer bitwise_or(const integer& n1, const integer& n2) {
    try {
        return integer(bitwise_or(core::integer(n1), core::integer(n2)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return integer::nan();
    }
}

//! \copydoc core::bitwise_xor
integer bitwise_xor(const integer& n1, const integer& n2) {
    try {
        return integer(bitwise_xor(core::integer(n1), core::integer(n2)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return integer::nan();
    }
}

//! \copydoc core::shift
integer shift(const integer& n, int byBits) {
    if (byBits > 0) return shift_left(n, byBits);
    else return shift_right(n, -byBits);
}

//! \copydoc core::shift_left
integer shift_left(const integer& n, size_t byBits) {
    try {
        return integer(shift_left(core::integer(n), byBits));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return integer::nan();
    }
}

//! \copydoc core::shift_right
integer shift_right(const integer& n, size_t byBits) {
    try {
        return integer(shift_right(core::integer(n), byBits));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return integer::nan();
    }
}

} // namespace blockable
} // namespace xint
} // namespace boost
