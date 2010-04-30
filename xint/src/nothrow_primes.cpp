
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
           the \c nothrow_integer type.
*/

#include "../boost/xint/nothrow_integer.hpp"

namespace boost {
namespace xint {

/*! \copydoc xint::is_prime(const integer&, callback_t)

\note Returns -1 instead of throwing.
*/
int is_prime(const nothrow_integer& n, callback_t callback) {
    try {
        return is_prime(xint::integer(n), callback);
    } catch (std::exception&) {
        return -1;
    }
}

//! \copydoc xint::random_prime
nothrow_integer nothrow_random_prime(size_t size_in_bits, callback_t callback) {
    try {
        nothrow_integer r(xint::random_prime(size_in_bits, callback));
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

} // namespace xint
} // namespace boost
