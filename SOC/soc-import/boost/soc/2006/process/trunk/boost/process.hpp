//
// Boost.Process
// Top header that includes everything.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#if !defined(BOOST_PROCESS_HPP)
#define BOOST_PROCESS_HPP

#include <boost/process/config.hpp>

#include <boost/process/attributes.hpp>
#include <boost/process/basic_child.hpp>
#include <boost/process/basic_process.hpp>
#include <boost/process/command_line.hpp>
#include <boost/process/forward_decls.hpp>
#include <boost/process/launcher.hpp>
#include <boost/process/pistream.hpp>
#include <boost/process/postream.hpp>
#include <boost/process/process.hpp>
#include <boost/process/status.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
#   include <boost/process/posix_attributes.hpp>
#endif

#endif // !defined(BOOST_PROCESS_HPP)
