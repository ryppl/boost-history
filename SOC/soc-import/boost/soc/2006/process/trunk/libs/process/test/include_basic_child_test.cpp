//
// Boost.Process
// Checks that basic_child.hpp can be included without errors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/basic_child.hpp>

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;

// ------------------------------------------------------------------------

namespace boost {
namespace process {
namespace detail {

class factories {
public:
    static
    void*
    test_it(void)
    {
        bp::basic_child< int >::handle_type h =
            (bp::basic_child< int >::handle_type)0;
        int cl = 5;
        bpd::file_handle fh;
        return new bp::basic_child< int >(h, cl, fh, fh, fh);
    }
};

} // namespace detail
} // namespace process
} // namespace boost

// ------------------------------------------------------------------------

void*
test_it(void)
{
    return bp::detail::factories::test_it();
}
