//
// Boost.Process
// Checks that detail/posix_sigchld.hpp can be included without errors.
//
// Copyright (c) 2008 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)

#   include <boost/process/detail/posix_sigchld.hpp>

namespace bpd = ::boost::process::detail;

// ------------------------------------------------------------------------

void*
test_it(void)
{
    return &bpd::the_posix_sigchld_programmer;
}

// ------------------------------------------------------------------------

#endif
