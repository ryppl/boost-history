//
// Boost.Process
// Library configuration.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//
 
#if !defined(BOOST_PROCESS_CONFIG_HPP)
#define BOOST_PROCESS_CONFIG_HPP

#include <boost/config.hpp>

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#   define BOOST_PROCESS_WIN32_API
#else
#   define BOOST_PROCESS_POSIX_API
#endif

#endif // !defined(BOOST_PROCESS_CONFIG_HPP)
