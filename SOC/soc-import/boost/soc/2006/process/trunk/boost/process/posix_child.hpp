//
// Boost.Process
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

//!
//! \file boost/process/posix_child.hpp
//!
//! Includes the declaration of the posix_child class.
//!

#if !defined(BOOST_PROCESS_POSIX_CHILD_HPP)
/** \cond */
#define BOOST_PROCESS_POSIX_CHILD_HPP
/** \endcond */

#include <boost/process/basic_posix_child.hpp>
#include <boost/process/command_line.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

//!
//! \brief Generic instantiation of the basic_posix_child template.
//!
//! Generic instantiation of the basic_posix_child template.  This relies
//! on the command_line implementation of the Command_Line concept provided
//! by the library.
//!
typedef basic_posix_child< command_line > posix_child;

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_POSIX_CHILD_HPP)
