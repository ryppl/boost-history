//
// Boost.Process
// Checks that process.hpp can be included without errors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/process.hpp>

namespace bp = ::boost::process;

// ------------------------------------------------------------------------

class test_process :
    public bp::process
{
public:
    test_process(handle_type h, const bp::command_line& cl) :
        bp::process(h, cl)
    {
    }
};

// ------------------------------------------------------------------------

void*
test_it(void)
{
    test_process::handle_type h = (test_process::handle_type)0;
    bp::command_line cl("test");
    return new test_process(h, cl);
}
