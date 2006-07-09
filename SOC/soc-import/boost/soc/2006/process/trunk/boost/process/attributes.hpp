//
// Boost.Process
// Generic instantiation of basic_attributes.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//
 
#if !defined(BOOST_PROCESS_ATTRIBUTES_HPP)
#define BOOST_PROCESS_ATTRIBUTES_HPP

#include <boost/process/basic_attributes.hpp>
#include <boost/process/command_line.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

typedef basic_attributes< command_line > attributes;

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_ATTRIBUTES_HPP)
