#ifndef BOOST_DETAIL_LIGHTWEIGHT_MUTEX_HPP_INCLUDED
#define BOOST_DETAIL_LIGHTWEIGHT_MUTEX_HPP_INCLUDED

#if _MSC_VER >= 1020
#pragma once
#endif

//
//  boost/detail/lightweight_mutex.hpp - lightweight mutex
//
//  Copyright (c) 2002 Peter Dimov and Multi Media Ltd.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  typedef <implementation-defined> boost::detail::lightweight_mutex;
//
//  boost::detail::lightweight_mutex meets the Mutex concept requirements
//  See http://www.boost.org/libs/thread/doc/mutex_concept.html#Mutex
//
//  * Used by the smart pointer library
//  * Performance oriented
//  * Header-only implementation
//  * Small memory footprint
//  * Not a general purpose mutex, use boost::mutex, CRITICAL_SECTION or
//    pthread_mutex instead.
//  * Never spin in a tight lock/do-something/unlock loop, since
//    lightweight_mutex does not guarantee fairness.
//  * Never keep a lightweight_mutex locked for long periods.
//


#include <boost/config.hpp>

#ifndef BOOST_HAS_THREADS
#  include <boost/detail/lwm_nop.hpp>
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#  include <boost/detail/lwm_win32.hpp>
//#elif defined(linux) || defined(__linux) || defined(__linux__)
//#  include <boost/detail/lwm_linux.hpp>
#elif defined(BOOST_HAS_PTHREADS)
#  include <boost/detail/lwm_pthreads.hpp>
#else
#  include <boost/detail/lwm_nop.hpp>
#endif

#endif // #ifndef BOOST_DETAIL_LIGHTWEIGHT_MUTEX_HPP_INCLUDED
