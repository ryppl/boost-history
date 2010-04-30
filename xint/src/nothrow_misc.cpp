
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
    \brief Functions for the \c nothrow_integer type that don't fall into any
           other category.
*/

#include "../boost/xint/nothrow_integer.hpp"

namespace boost {
namespace xint {

/*! \copydoc xint::log2(const integer&)

\note Returns 0 instead of throwing.
*/
size_t log2(const nothrow_integer& n) {
    try {
        return (n._is_nan() ? 0 : n._log2());
    } catch (std::exception&) {
        return 0;
    }
}

} // namespace xint
} // namespace boost
