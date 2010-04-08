
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
    \brief Functions that don't fall into any other category.
*/

#include "../boost/xint/xint.hpp"

namespace boost {
namespace xint {

/*! \brief Tells whether the library was compiled with \link securemode Secure
Mode \endlink enabled.

\returns \c true if Secure Mode is active, \c false if it isn't.
*/
bool opt_secure_mode() {
    #ifdef XINT_SECURE
        return true;
    #else
        return false;
    #endif
}

/*! \brief Tells whether the library was compiled with the \link threadsafe
Thread Safe Operation option \endlink enabled.

\returns \c true if it was compiled with Thread Safe Operation enabled, \c false
if it wasn't.
*/
bool opt_thread_safe() {
    #ifdef XINT_THREADSAFE
        return true;
    #else
        return false;
    #endif
}

namespace core {
size_t log2(const integer& n) {
    size_t r=detail::bits_per_digit * n._get_length();
    detail::digit_t mask=detail::digit_hibit, d=n._get_digit(n._get_length()-1);
    while (mask!=0) {
        if ((mask & d)!=0) break;
        mask>>=1;
        --r;
    }
    return r;
}
} // namespace core

/*! \brief Get the log<sub>2</sub> value of an integer.

\param[in] n The integer to operate on.

\returns The %integer log<sub>2</sub> value of the integer.

\note If exceptions are blocked, returns 0 instead of throwing.

\remarks
xint::pow2(xint::log2(n)-1) will give you an integer with the highest set bit of
\c n, assuming that \c n is non-zero.

\par
Similar to the xint::highestbit function.
*/
size_t log2(const integer& n) {
    try {
        return log2(core::integer(n));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return 0;
    }
}

} // namespace xint
} // namespace boost
