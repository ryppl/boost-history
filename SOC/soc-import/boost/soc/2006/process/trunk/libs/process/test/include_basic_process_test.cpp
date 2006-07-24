//
// Boost.Process
// Checks that basic_process.hpp can be included without errors.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/basic_process.hpp>

namespace bp = ::boost::process;

// ------------------------------------------------------------------------

template< class T1, class T2 >
class test :
    public bp::basic_process< T1, T2 >
{
public:
    typedef typename bp::basic_process< T1, T2 >::handle_type handle_type;

    test(handle_type h, T1 a, T2 b) :
        bp::basic_process< T1, T2 >(h, a, b)
    {
    }
};

// ------------------------------------------------------------------------

void*
test_it(void)
{
    test< int, int >::handle_type h =
        static_cast< test< int, int >::handle_type >(0);
    return new test< int, int >(h, 5, 5);
}
