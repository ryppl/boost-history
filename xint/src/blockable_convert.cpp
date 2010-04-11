
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
    \brief Contains the conversion functions for blockable integers.

    Note that the xint::blockable::to<T> function is not here, because it's a
    template function and must be defined in a header file.
*/

#include "../boost/xint/xint.hpp"

namespace boost {
namespace xint {
namespace blockable {

using namespace detail;

/*! \copydoc core::to_string

\note If exceptions are blocked, it returns an empty string instead of throwing
an exception.
*/
std::string to_string(const integer& n, size_t base, bool uppercase) {
    try {
        return to_string(core::integer(n), base, uppercase);
    } catch (exception&) {
        if (exceptions_allowed()) throw;
        else return std::string();
    }
}

//! \copydoc core::from_string
integer from_string(const std::string& str, size_t base) {
    try {
        if (str==detail::nan_text) return integer::nan();
        return integer(core::from_string(str, base));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return integer::nan();
    }
}

//! \copydoc core::from_binary
integer from_binary(const std::string& str) {
    try {
        return integer(core::from_binary(str));
    } catch (exception&) {
        if (exceptions_allowed()) throw;
        else return integer::nan();
    }
}

//! \copydoc core::to_binary
std::string to_binary(const integer& n) {
    try {
        return to_binary(core::integer(n));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return std::string();
    }
}

} // namespace blockable
} // namespace xint
} // namespace boost
