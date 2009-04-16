//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DETAIL_SEMAPHORE_H
#define BOOST_TP_DETAIL_SEMAPHORE_H

#include <boost/tp/detail/config.hpp>

# if defined(BOOST_WINDOWS_API)
#   include <boost/tp/detail/semaphore_windows.hpp>
# elif defined(BOOST_POSIX_API)
#   include <boost/tp/detail/semaphore_posix.hpp>
# else
#   error "No semaphore available for this platform!"
# endif

#endif // BOOST_TP_DETAIL_SEMAPHORE_H
