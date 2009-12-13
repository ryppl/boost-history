//  chrono.cpp  --------------------------------------------------------------//

//  Copyright Beman Dawes 2008
//  Copyright Vicente J. Botet Escriba 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

// define BOOST_CHRONO_SOURCE so that <boost/filesystem/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
#define BOOST_CHRONO_SOURCE

#include <boost/chrono/chrono.hpp>
#include <boost/system/system_error.hpp>
#include <boost/throw_exception.hpp>

//----------------------------------------------------------------------------//
//                                                                            //
//                     Platform-specific Implementations                      //
//                                                                            //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//                                Windows                                     //
//----------------------------------------------------------------------------//
#if defined(BOOST_CHRONO_WINDOWS_API)
#include "win/chrono.cpp"

//----------------------------------------------------------------------------//
//                                 Mac                                        //
//----------------------------------------------------------------------------//
#elif defined(BOOST_CHRONO_MAC_API)
#include "mac/chrono.cpp"

//----------------------------------------------------------------------------//
//                                POSIX                                     //
//----------------------------------------------------------------------------//
#elif defined(BOOST_CHRONO_POSIX_API)
#include "posix/chrono.cpp"

#endif  // POSIX
