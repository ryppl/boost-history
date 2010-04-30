
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
    \brief Contains the definitions for the random number functions for the \c
           nothrow_integer type.
*/

#include "../boost/xint/nothrow_integer.hpp"

namespace boost {
namespace xint {

//! \copydoc xint::random_by_size
nothrow_integer nothrow_random_by_size(size_t bits, bool high_bit_on, bool
    low_bit_on, bool can_be_negative)
{
    try {
        return nothrow_integer(xint::random_by_size(bits, high_bit_on,
            low_bit_on, can_be_negative));
    } catch (std::exception&) {
        return nothrow_integer::nan();
    }
}

} // namespace xint
} // namespace boost
