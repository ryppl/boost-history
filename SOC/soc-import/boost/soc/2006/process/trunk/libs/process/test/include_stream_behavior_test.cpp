//
// Boost.Process
// Checks that stream_behavior.hpp can be included without errors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/stream_behavior.hpp>

namespace bp = ::boost::process;

// ------------------------------------------------------------------------

void*
test_it(void)
{
    bp::stream_behavior b = bp::close_stream();
    return new bp::stream_behavior(b);
}
