
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_FIBER_H
#define BOOST_TASK_DETAIL_FIBER_H

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp>

# if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#include <boost/task/detail/fiber_windows.hpp>

# elif defined(_POSIX_VERSION)
#include <boost/fibers/detail/fiber_posix.hpp>

# endif

#endif // BOOST_TASK_DETAIL_FIBER_H
