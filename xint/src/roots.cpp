
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
    \brief Contains the definitions for functions related to roots of a number.
*/

#include "../boost/xint/integer.hpp"

namespace boost {
namespace xint {

namespace detail {

void sqrt(base_integer& target, const base_integer& nn) {
    if (nn._is_zero()) { target._attach(nn); return; }
    if (nn._get_negative()) throw exceptions::cannot_represent("library cannot "
        "represent imaginary values (tried to take sqrt of negative number)");

    // A naive implementation using pure integers can result in an endless loop,
    // cycling between two numbers that are approximately correct (try
    // sqrt(23)). To deal with that, we multiply everything by an even power of
    // two.
    const size_t extra_bits = 1;

    integer n(nn._to_integer());
    n <<= (extra_bits * 2);

    // Initial guess is half the length of n, in bits
    integer guess;
    setbit(guess, n._log2()/2);

    // Now refine it until we're as close as we can get.
    while (1) {
        integer guess2=(guess + (n / guess)) >> 1;
        if ((guess >> extra_bits) == (guess2 >> extra_bits)) break;
        guess = guess2;
    }

    // Remove half of the added bits.
    guess >>= extra_bits;
    target._attach(guess);
}

} // namespace detail

/*! \brief Calculate the square root of \c an integer.

- Complexity: O(n<sup>2</sup>)

\param[in] n The value to operate on.

\returns The largest integer that, when squared, is less than or equal to \c n.
If \c n is not a perfect square, then this value will not be the exact square
root.

\exception exceptions::cannot_represent if \c n is negative.
*/
integer sqrt(const integer& n) {
    integer r;
    detail::sqrt(r, n);
    return BOOST_XINT_MOVE(r);
}

} // namespace xint
} // namespace boost
