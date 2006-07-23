//
// Boost.Process
// Checks that child.hpp can be included without errors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/child.hpp>

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;

// ------------------------------------------------------------------------

namespace boost {
namespace process {

class launcher {
public:
    static
    void*
    test_it(void)
    {
        bp::command_line cl("test");
        bp::attributes attrs(cl);
        bp::child::handle_type h =
            static_cast< bp::child::handle_type >(0);
        bpd::file_handle fh;
        return new bp::child(h, attrs, fh, fh, fh);
    }
};

} // namespace process
} // namespace boost

// ------------------------------------------------------------------------

void*
test_it(void)
{
    return bp::launcher::test_it();
}
