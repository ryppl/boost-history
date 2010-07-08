//
// Boost.Process
// ~~~~~~~~~~~~~
//
// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008, 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

/**
 * \file boost/process/pid_type.hpp
 *
 * Includes the declaration of the pid type.
 */

#ifndef BOOST_PROCESS_PID_TYPE_HPP
#define BOOST_PROCESS_PID_TYPE_HPP

#include <boost/process/config.hpp>

#if defined(BOOST_POSIX_API)
#   include <sys/types.h>
#elif defined(BOOST_WINDOWS_API)
#   include <windows.h>
#endif

namespace boost {
namespace process {

#if defined(BOOST_PROCESS_DOXYGEN)
typedef NativeProcessId pid_type;
#elif defined(BOOST_POSIX_API)
typedef pid_t pid_type;
#elif defined(BOOST_WINDOWS_API)
typedef DWORD pid_type;
#endif

}
}

#endif
