
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
    \brief Contains the definitions for blockable integer functions related to
           roots of a number.
*/

#include "../boost/xint/xint.hpp"

namespace boost {
namespace xint {
namespace blockable {

//! \copydoc core::sqrt
integer sqrt(const integer& n) {
    try {
        return integer(sqrt(core::integer(n)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        return integer::nan();
    }
}

} // namespace blockable
} // namespace xint
} // namespace boost
