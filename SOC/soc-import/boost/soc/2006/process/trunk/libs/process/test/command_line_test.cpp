//
// Boost.Process
// Regression tests for the command_line class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <cstdlib>
#include <iostream>

#include <boost/process/command_line.hpp>

namespace bp = ::boost::process;

int
main(void)
{
    bp::command_line cl;

    return EXIT_SUCCESS;
}
