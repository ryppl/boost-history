
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
    \brief Contains the definitions for functions related to prime numbers for
           blockable integers.
*/

#include "../boost/xint/xint.hpp"

namespace boost {
namespace xint {
namespace blockable {

/*! \copydoc core::is_prime

\note If exceptions are blocked, returns -1 instead of throwing.
*/
int is_prime(const integer& n, callback_t callback) {
    try {
        return is_prime(core::integer(n), callback);
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return -1;
    }
}

//! \copydoc core::random_prime
integer random_prime(size_t size_in_bits, callback_t callback) {
    try {
        return integer(core::random_prime(size_in_bits, callback));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return integer::nan();
    }
}

} // namespace blockable
} // namespace xint
} // namespace boost
