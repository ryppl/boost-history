//
// Boost.Process
// Checks that process.hpp can be included without errors.
//
// Copyright (c) 2006, 2007 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/process.hpp>

namespace bp = ::boost::process;

// ------------------------------------------------------------------------

void*
test_it(void)
{
    bp::process::id_type id = (bp::process::id_type)0;
    return new bp::process(id);
}
