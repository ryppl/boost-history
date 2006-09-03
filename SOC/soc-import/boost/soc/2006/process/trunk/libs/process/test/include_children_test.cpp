//
// Boost.Process
// Checks that children.hpp can be included without errors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/children.hpp>

namespace bp = ::boost::process;

// ------------------------------------------------------------------------

namespace boost {
namespace process {

template< class Command_Line >
class basic_pipeline
{
public:
    static
    void*
    test_it(void)
    {
        return new bp::children();
    }
};

} // namespace process
} // namespace boost

// ------------------------------------------------------------------------

void*
test_it(void)
{
    return bp::basic_pipeline< char >::test_it();
}
