//
// Boost.Process
// Simple types for a Win32 system.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//
 
#if !defined(BOOST_PROCESS_TYPES_WIN32_HPP)
#define BOOST_PROCESS_TYPES_WIN32_HPP

#include <windows.h>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

// id_t: Process identifier internal to the OS.
typedef DWORD id_t;

// desc_t: Descriptor number used to manage OS's data streams.
typedef int desc_t;

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_TYPES_WIN32_HPP)
