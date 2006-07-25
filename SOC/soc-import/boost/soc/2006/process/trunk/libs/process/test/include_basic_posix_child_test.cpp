//
// Boost.Process
// Checks that basic_posix_child.hpp can be included without errors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)

#   include <boost/process/basic_posix_child.hpp>

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
        bp::basic_posix_child< int, int >::handle_type h =
            (bp::basic_posix_child< int, int >::handle_type)0;
        int cl = 5;
        int attrs = 5;
        bpd::file_handle fh;
        bp::basic_posix_child< int, int >::pipe_map pipes;
        return new bp::basic_posix_child< int, int >(h, cl, attrs, fh, fh,
                                                     fh, pipes, pipes);
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
