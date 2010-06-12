
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
    \brief Contains the square-root function definition, and any future
           root-related ones.

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

BOOST_XINT_INLINE void sqrt(data_t& target, data_t n) {
    if (n.is_zero()) { target.set(0); return; }
    if (n.negative) throw exceptions::cannot_represent("library cannot "
        "represent imaginary values (tried to take sqrt of negative number)");

    // A naive implementation using pure integers can result in an endless loop,
    // cycling between two numbers that are approximately correct (try
    // sqrt(23)). To deal with that, we multiply everything by an even power of
    // two.
    const size_t extra_bits = 1;
    n <<= (extra_bits * 2);

    // Initial guess is half the length of n, in bits
    target.beginmod();
    target.set(0);
    setbit(target, log2(n) / 2);

    // Now refine it until we're as close as we can get.
    while (1) {
        data_t guess((target + (n / target)) >> 1);
        if ((target >> extra_bits) == (guess >> extra_bits)) break;
        target.duplicate_data(guess);
    }

    // Remove half of the added bits.
    target >>= extra_bits;
    target.endmod();
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)
