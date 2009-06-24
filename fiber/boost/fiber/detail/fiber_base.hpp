//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DETAIL_FIBER_BASE_H
#define BOOST_FIBERS_DETAIL_FIBER_BASE_H

#include <boost/config.hpp>

#if defined(_POSIX_VERSION)
#include <boost/fibers/detail/fiber_base_posix.hpp>
#elif defined(BOOST_WINDOWS)
#include <boost/fibers/detail/fiber_base_windows.hpp>
#else 
#error "Boost fibers unavailable on this platform"
#endif

#endif // BOOST_FIBERS_DETAIL_FIBER_BASE_H
