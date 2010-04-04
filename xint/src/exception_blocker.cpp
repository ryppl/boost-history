
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file holds definitions for the exception blocker classes and functions.
*/

#include "../xint.hpp"

#ifdef XINT_THREADSAFE
    #include <boost/thread.hpp>
#endif

namespace xint {
namespace {

#ifdef XINT_THREADSAFE
    boost::thread_specific_ptr<bool> allowed;

    struct EBlockerToken: public detail::token {
        EBlockerToken(bool newAllowState) {
            if (allowed.get()==0) allowed.reset(new bool(true));
            mOldState=*allowed;
            *allowed=newAllowState;
        }
        ~EBlockerToken() { *allowed=mOldState; }

        bool mOldState;
    };
#else
    bool allowed=true;

    struct EBlockerToken: public detail::token {
        EBlockerToken(bool newAllowState): mOldState(allowed) { allowed=newAllowState; }
        ~EBlockerToken() { allowed=mOldState; }

        bool mOldState;
    };
#endif

} // namespace

bool exceptions_allowed() {
    #ifdef XINT_THREADSAFE
        // Defaults to true
        return (allowed.get()==0 || *allowed==true);
    #else
        return allowed;
    #endif
}

token block_exceptions() {
    return token(new EBlockerToken(false));
}

token allow_exceptions() {
    return token(new EBlockerToken(true));
}

} // namespace xint

