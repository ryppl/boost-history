//
// Boost.Process
// Generic instantiation of basic_process.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#if !defined(BOOST_PROCESS_PROCESS_HPP)
#define BOOST_PROCESS_PROCESS_HPP

#include <boost/process/attributes.hpp>
#include <boost/process/basic_process.hpp>
#include <boost/process/command_line.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

typedef basic_process< command_line, attributes > process;

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_PROCESS_HPP)
