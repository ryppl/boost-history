
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_SEMAPHORE_H
#define BOOST_TASK_DETAIL_SEMAPHORE_H

#include <boost/task/detail/config.hpp>

# if defined(BOOST_WINDOWS_API)
#   include <boost/task/detail/semaphore_windows.hpp>
# elif defined(BOOST_POSIX_API)
#   include <boost/task/detail/semaphore_posix.hpp>
# else
#   error "No semaphore available for this platform!"
# endif

#endif // BOOST_TASK_DETAIL_SEMAPHORE_H
