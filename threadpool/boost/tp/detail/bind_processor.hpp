//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_THREAD_BIND_PROCESSOR_H
#define BOOST_THREAD_BIND_PROCESSOR_H

#include <boost/config.hpp>

#define BOOST_THREAD_BIND_PROCESSOR_AFFINITY 1

#if defined(linux) || defined(__linux) || defined(__linux__)
#include <boost/tp/detail/bind_processor_linux.hpp>
// #elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
// #include <boost/tp/detail/bind_processor_bsd.hpp>
#elif defined(sun) || defined(__sun)
#include <boost/tp/detail/bind_processor_solaris.hpp>
#elif defined(__sgi)
#include <boost/tp/detail/bind_processor_irix.hpp>
#elif defined(__IBMCPP__) || defined(_AIX)
#include <boost/tp/detail/bind_processor_aix.hpp>
#elif defined(__hpux)
#include <boost/tp/detail/bind_processor_hpux.hpp>
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#include <boost/tp/detail/bind_processor_windows.hpp>
#else
#undef BOOST_THREAD_BIND_PROCESSOR_AFFINITY
#endif

#endif // BOOST_THREAD_BIND_PROCESSOR_H
