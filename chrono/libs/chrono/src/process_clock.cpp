//  boost process_timer.cpp  -----------------------------------------------------------//

//  Copyright Beman Dawes 1994, 2006, 2008

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

//--------------------------------------------------------------------------------------//

// define BOOST_CHRONO_SOURCE so that <boost/chrono/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
#define BOOST_CHRONO_SOURCE 

#include <boost/chrono/config.hpp>
#include <boost/chrono/process_times.hpp>
#include <cassert>

//----------------------------------------------------------------------------//
//                                Windows                                     //
//----------------------------------------------------------------------------//
#if defined(BOOST_CHRONO_WINDOWS_API)
#include "win/process_clock.cpp"

//----------------------------------------------------------------------------//
//                                 Mac                                        //
//----------------------------------------------------------------------------//
#elif defined(BOOST_CHRONO_MAC_API)
#include "mac/process_clock.cpp"

//----------------------------------------------------------------------------//
//                                POSIX                                     //
//----------------------------------------------------------------------------//
#elif defined(BOOST_CHRONO_POSIX_API)
#include "posix/process_clock.cpp"

#endif  // POSIX


