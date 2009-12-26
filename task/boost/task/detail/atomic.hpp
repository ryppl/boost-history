
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASKS_DETAIL_ATOMIC_H
#define BOOST_TASKS_DETAIL_ATOMIC_H

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp>

#include <boost/task/detail/has_sync.hpp>

# if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#include <boost/task/detail/atomic_interlocked.hpp>

# elif defined( __GNUC__ ) && ( defined( __i386__ ) || defined( __x86_64__ ) )
#include <boost/task/detail/atomic_gcc_x86.hpp>

# elif defined( __GNUC__ ) && ( defined(__PPC__) || defined(__ppc__) )
#include <boost/task/detail/atomic_gcc_ppc.hpp>

# elif defined( BOOST_TASKS_HAS_SYNC)
#include <boost/task/detail/atomic_sync.hpp>

# elif defined(__GLIBCPP__) || defined(__GLIBCXX__)
#include <boost/task/detail/atomic_gcc.hpp>

# elif defined(__IBMCPP__) || defined(_AIX)
#include <boost/task/detail/atomic_aix.hpp>

# elif defined(__hpux)
#include <boost/task/detail/atomic_hpux.hpp>

# elif defined(sun) || defined(__sun)
#include <boost/task/detail/atomic_solaris.hpp>

# else
#include <boost/task/detail/atomic_interprocess.hpp>

# endif

#endif // BOOST_TASKS_DETAIL_ATOMIC_H

