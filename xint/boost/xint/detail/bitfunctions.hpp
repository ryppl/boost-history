
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
    \brief Contains the definitions for the bit-manipulation functions.

    This file will be included by the library itself when needed.
*/

#if defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)

#if defined(BOOST_XINT_COMPILED_LIB)
    #include "internals.hpp"
#endif // defined(BOOST_XINT_COMPILED_LIB)

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

BOOST_XINT_INLINE bool getbit(const data_t n, size_t bit_index) {
    size_t index = bit_index / bits_per_digit;
    if (index < n.length) {
        digit_t mask=(digit_t(1) << (bit_index % bits_per_digit));
        return ((n[index] & mask) != 0);
    } else return false;
}

BOOST_XINT_INLINE void setbit(data_t& n, size_t bit_index) {
    if (n.is_fixed_length() && bit_index >= (n.max_length() * bits_per_digit))
        return;

    size_t index = (bit_index / bits_per_digit);
    digit_t mask = (digit_t(1) << (bit_index % bits_per_digit));

    n.beginmod(index + 1, true);
    n.digits()[index] |= mask;
    if (n.length < index + 1) n.length = index + 1;
    n.endmod();
}

BOOST_XINT_INLINE void clearbit(data_t& n, size_t bit_index) {
    size_t index = (bit_index / bits_per_digit);
    if (index >= n.length) return; // Bit can't be set, no need to clear it.
    digit_t mask = (digit_t(1) << (bit_index % bits_per_digit));

    n.beginmod();
    n.digits()[index] &= ~mask;
    n.endmod();
}

BOOST_XINT_INLINE size_t lowestbit(const data_t n, size_t valueIfZero) {
    if (n.is_zero()) return valueIfZero;

    const digit_t *d = n.digits(), *p = d, *pe = p + n.length;
    while (p != pe && *p == 0) ++p;
    size_t r = (bits_per_digit * (p - d));

    digit_t digit = *p;
    while ((digit & 0x01)==0) { digit >>= 1; ++r; }

    return r;
}

BOOST_XINT_INLINE size_t highestbit(const data_t n, size_t valueIfZero) {
    if (n.is_zero()) return valueIfZero;
    return ((n.length - 1) * bits_per_digit) + log2(n[n.length - 1]);
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)
