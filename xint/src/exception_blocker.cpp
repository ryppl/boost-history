
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
    \brief Contains the definitions for the exception-blocker classes and
    functions.
*/

#include "../boost/xint/xint.hpp"

#ifdef XINT_THREADSAFE
    #include <boost/thread.hpp>
#endif

namespace xint {
namespace {

#ifdef XINT_THREADSAFE
    boost::thread_specific_ptr<bool> allowed;
#else
    bool allowed=true;
#endif

struct EBlockerToken: public detail::token {
    #ifdef XINT_THREADSAFE
        EBlockerToken(bool newAllowState) {
            if (allowed.get()==0) allowed.reset(new bool(true));
            mOldState=*allowed;
            *allowed=newAllowState;
        }
        ~EBlockerToken() { *allowed=mOldState; }
    #else
        EBlockerToken(bool newAllowState): mOldState(allowed) {
            allowed=newAllowState; }
        ~EBlockerToken() { allowed=mOldState; }
    #endif

    bool mOldState;
};

} // namespace

/*! \brief Query whether exceptions are currently allowed or blocked.

\returns \c true if exceptions are currently allowed (the default state), \c
false if they're blocked.

\remarks
When the library is compiled for \link threadsafe thread-safe operation\endlink,
exceptions are blocked or allowed on a per-thread basis. Otherwise they
are blocked or allowed globally.
*/
bool exceptions_allowed() {
    #ifdef XINT_THREADSAFE
        // Defaults to true
        return (allowed.get()==0 || *allowed==true);
    #else
        return allowed;
    #endif
}

/*! \brief Temporarily prevent the library from throwing blockable exceptions.

\returns An xint::token. Until this token is destroyed, blockable exceptions are
blocked, unless explicitly re-enabled.

\remarks
When the library is compiled for \link threadsafe thread-safe operation\endlink,
exceptions are blocked or allowed on a per-thread basis. Otherwise they are
blocked or allowed globally.

\see \ref exceptions
*/
token block_exceptions() {
    return token(new EBlockerToken(false));
}

/*! \brief Temprarily counteract an xint::block_exceptions token to allow all
           exceptions.

\returns An xint::token. Until this token is destroyed, blockable exceptions can
be thrown, unless explicitly re-blocked.

\note
Exceptions are allowed by default.

\remarks
When the library is compiled for \link threadsafe thread-safe operation\endlink,
exceptions are blocked or allowed on a per-thread basis. Otherwise they are
blocked or allowed globally.

\see \ref exceptions
*/
token allow_exceptions() {
    return token(new EBlockerToken(true));
}

} // namespace xint

