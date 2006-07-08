//
// Boost.Process
// Streambuf implementation for system descriptors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//
 
#if !defined(BOOST_PROCESS_DETAIL_SYSTEMBUF_HPP)
#define BOOST_PROCESS_DETAIL_SYSTEMBUF_HPP

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_WIN32_API)
#   include <boost/process/detail/systembuf_win32.hpp>
#else
#   include <boost/process/detail/systembuf_posix.hpp>
#endif

#endif // !defined(BOOST_PROCESS_DETAIL_SYSTEMBUF_HPP)
