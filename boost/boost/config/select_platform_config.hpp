//  Boost compiler configuration selection header file  ------------------------------//

//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version.

// locate which platform we are on and define BOOST_PLATFORM_CONFIG as needed.
// Note that we define the headers to include using "header_name" not <header_name>
// in order to prevent macro expansion within the header name (for example
// "linux" is a macro on linux systems).

#if defined(linux) || defined(__linux)
   // linux:
#  define BOOST_PLATFORM_CONFIG "boost/config/platform/linux.hpp"
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
   // BSD:
#  define BOOST_PLATFORM_CONFIG "boost/config/platform/bsd.hpp"
#elif defined(sun) || defined(__sun)
   // solaris:
#  define BOOST_PLATFORM_CONFIG "boost/config/platform/sun.hpp"
#elif defined(__sgi)
   // SGI Irix:
#  define BOOST_PLATFORM_CONFIG "boost/config/platform/irix.hpp"
#elif defined(__hpux)
   // hp unix:
#  define BOOST_PLATFORM_CONFIG "boost/config/platform/hpux.hpp"
#elif defined(__CYGWIN__)
   // cygwin is not win32:
#  define BOOST_PLATFORM_CONFIG "boost/config/platform/cygwin.hpp"
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
   // win32:
#  define BOOST_PLATFORM_CONFIG "boost/config/platform/win32.hpp"
#elif defined(__BEOS__)
   // BeOS
#  define BOOST_PLATFORM_CONFIG "boost/config/platform/be.hpp"
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
   // mac os
#  define BOOST_PLATFORM_CONFIG "boost/config/platform/mac.hpp"

#endif


