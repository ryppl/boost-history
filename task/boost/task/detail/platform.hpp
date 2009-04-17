
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// this file is based on platform.hpp of boost.thread

#ifndef BOOST_TASK_DETAIL_PLATFORM_H
#define BOOST_TASK_DETAIL_PLATFORM_H

// fetch compiler and platform configuration
#include <boost/config.hpp>

// insist on threading support being available:
#include <boost/config/requires_threads.hpp>

// choose platform
# if defined(linux) || defined(__linux) || defined(__linux__)
#   define BOOST_TASK_LINUX
# elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#   define BOOST_TASK_BSD
# elif defined(sun) || defined(__sun)
#   define BOOST_TASK_SOLARIS
# elif defined(__hpux)
#   define BOOST_TASK_HPUX
# elif defined(__CYGWIN__)
#   define BOOST_TASK_CYGWIN
# elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#   define BOOST_TASK_WIN32
# elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#   define BOOST_TASK_MACOS
# elif defined(__IBMCPP__) || defined(_AIX)
#   define BOOST_TASK_AIX
# elif defined(__QNXNTO__)
#   define BOOST_TASK_QNXNTO
# endif

#endif // BOOST_TASK_DETAIL_PLATFORM_H
