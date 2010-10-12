//  boost thread_clock.cpp  -----------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

//--------------------------------------------------------------------------------------//


// define BOOST_CHRONO_SOURCE so that <boost/chrono/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
#define BOOST_CHRONO_SOURCE

#include <boost/chrono/detail/inlined/thread_clock.hpp>

#if 0


#include <boost/chrono/config.hpp>
#include <boost/version.hpp>
#if defined(BOOST_CHRONO_HAS_THREAD_CLOCK)
#include <boost/chrono/thread_clock.hpp>
#include <boost/throw_exception.hpp>
#include <boost/system/system_error.hpp>
#include <cassert>

//----------------------------------------------------------------------------//
//                                Windows                                     //
//----------------------------------------------------------------------------//
#if defined(BOOST_CHRONO_WINDOWS_API)
#include "win/thread_clock.cpp"

//----------------------------------------------------------------------------//
//                                 Mac                                        //
//----------------------------------------------------------------------------//
#elif defined(BOOST_CHRONO_MAC_API)
#include "mac/thread_clock.cpp"

//----------------------------------------------------------------------------//
//                                POSIX                                     //
//----------------------------------------------------------------------------//
#elif defined(BOOST_CHRONO_POSIX_API)
#include "posix/thread_clock.cpp"

#endif  // POSIX


#endif
#endif
