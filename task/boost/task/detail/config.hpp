
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_CONFIG_H
#define BOOST_TASK_DETAIL_CONFIG_H

#include <boost/config.hpp>

# if defined(BOOST_WINDOWS_API) && defined(BOOST_POSIX_API)
#   error "Both BOOST_WINDOWS_API and BOOST_POSIX_API are defined!"
# elif ! defined(BOOST_WINDOWS_API ) && ! defined(BOOST_POSIX_API)
#   if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__)
#     define BOOST_WINDOWS_API
#   else
#     define BOOST_POSIX_API
#   endif
# endif

# if defined(BOOST_WINDOWS_API)
#   define BOOST_WINDOWS_OS
# elif defined(linux) || defined(__linux) || defined(__linux__)
#   define BOOST_LINUX_OS
# elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#   define BOOST_xBSD_OS
# elif defined(__IBMCPP__) || defined(_AIX)
#   define BOOST_AIX_OS
# elif defined(__hpux)
#   define BOOST_HPUX_OS
# elif defined(sun) || defined(__sun)
#   define BOOST_SOLARIS_OS
# endif

#endif // BOOST_TASK_DETAIL_CONFIG_H

