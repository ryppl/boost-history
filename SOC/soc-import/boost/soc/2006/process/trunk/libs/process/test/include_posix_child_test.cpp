//
// Boost.Process
// Checks that posix_child.hpp can be included without errors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)

#   include <boost/process/posix_child.hpp>

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;

// ------------------------------------------------------------------------

void*
test_it(void)
{
    bp::posix_child::handle_type h = (bp::posix_child::handle_type)0;
    bpd::info_map info;
    return new bp::posix_child(h, info, info);
}

// ------------------------------------------------------------------------

#endif
