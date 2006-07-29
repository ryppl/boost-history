//
// Boost.Process
// Checks that posix_child.hpp can be included without errors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)

#   include <boost/process/posix_child.hpp>

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;

// ------------------------------------------------------------------------

namespace boost {
namespace process {

class posix_launcher {
public:
    static
    void*
    test_it(void)
    {
        bp::command_line cl("test");
        bp::posix_child::handle_type h = (bp::posix_child::handle_type)0;
        bpd::file_handle fh;
        bp::posix_child::pipe_map pipes;
        return new bp::posix_child(h, cl, fh, fh, fh, pipes, pipes);
    }
};

} // namespace process
} // namespace boost

// ------------------------------------------------------------------------

void*
test_it(void)
{
    return bp::posix_launcher::test_it();
}

// ------------------------------------------------------------------------

#endif
