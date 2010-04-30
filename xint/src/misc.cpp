
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

#include "../boost/xint/integer.hpp"

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

/*! \brief Get the log<sub>2</sub> value of an integer.

- Complexity: O(1)

\param[in] n The %integer to operate on.

\returns The %integer log<sub>2</sub> value of the integer.

\remarks
#pow2(log2(n)-1) will give you an integer with the highest set bit of \c n,
assuming that \c n is non-zero.

\par
Similar to the #highestbit function.
*/
size_t log2(const integer& n) {
    return n._log2();
}

//! \copydoc log2(const integer&)
size_t log2(const fixed_integer_any& n) {
    return n._log2();
}

} // namespace xint
} // namespace boost
