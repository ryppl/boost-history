
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the definitions for functions that don't fall into any
    of the other categories.
*/

#include "../xint.hpp"

namespace xint {

bool opt_secure_mode() {
    #ifdef XINT_SECURE
        return true;
    #else
        return false;
    #endif
}

bool opt_thread_safe() {
    #ifdef XINT_THREADSAFE
        return true;
    #else
        return false;
    #endif
}

size_t log2(const integer& n) {
    n._throw_if_nan();

    size_t r=detail::bits_per_digit * n._get_length();
    detail::digit_t mask=detail::digit_hibit, d=n._get_digit(n._get_length()-1);
    while (mask!=0) {
        if ((mask & d)!=0) break;
        mask>>=1;
        --r;
    }
    return r;
}

} // namespace xint
