
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
    \brief Functions for blockable integers that don't fall into any other
           category.
*/

#include "../boost/xint/xint.hpp"

namespace boost {
namespace xint {
namespace blockable {

/*! \copydoc core::log2

\note If exceptions are blocked, returns 0 instead of throwing.
*/
size_t log2(const integer& n) {
    try {
        return log2(core::integer(n));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return 0;
    }
}

} // namespace blockable
} // namespace xint
} // namespace boost
