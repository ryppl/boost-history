
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the definitions for functions related to roots of a
    number.
*/

#include "../xint.hpp"

namespace xint {

integer sqrt(const integer& n) {
    if (n.sign() < 0) {
        if (exceptions_allowed()) throw cannot_represent("library cannot "
            "represent imaginary values (tried to take sqrt of negative "
            "number)");
        else return integer(not_a_number());
    } else if (n.sign() == 0) return integer::zero();

    // Initial guess is half the length of n, in bits
    integer guess;
    setbit(guess, log2(n)/2);

    // Now refine it until we're as close as integers can get
    while (1) {
        integer guess2=(guess + (n/guess)) >> 1;
        if (guess == guess2) break;
        guess=guess2;
    }

    return guess;
}

} // namespace xint
