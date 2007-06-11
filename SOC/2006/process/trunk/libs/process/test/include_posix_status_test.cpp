//
// Boost.Process
// Checks that posix_status.hpp can be included without errors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)

#   include <boost/process/posix_status.hpp>

namespace bp = ::boost::process;

// ------------------------------------------------------------------------

namespace boost {
namespace process {

class child
{
public:
    static
    void*
    test_it(void)
    {
        return new bp::posix_status(bp::status(0));
    }
};

} // namespace process
} // namespace boost

// ------------------------------------------------------------------------

void*
test_it(void)
{
    return bp::child::test_it();
}

// ------------------------------------------------------------------------

#endif
