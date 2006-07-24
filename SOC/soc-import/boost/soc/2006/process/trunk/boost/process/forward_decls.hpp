//
// Boost.Process
// Forward declarations for all public types.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#if !defined(BOOST_PROCESS_FORWARD_DECLS_HPP)
#define BOOST_PROCESS_FORWARD_DECLS_HPP

#include <boost/process/config.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

class attributes;

template< class Command_Line, class Attributes >
class basic_child;

template< class Command_Line, class Attributes >
class basic_process;

class command_line;

class launcher;

class pistream;

class postream;

class status;

typedef basic_child< command_line, attributes > child;
typedef basic_process< command_line, attributes > process;

#if defined(BOOST_PROCESS_POSIX_API)
class posix_attributes;
#endif

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_FORWARD_DECLS_HPP)
