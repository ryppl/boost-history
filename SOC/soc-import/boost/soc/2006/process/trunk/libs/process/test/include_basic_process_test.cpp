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

template< class T >
class test :
    public bp::basic_process< T >
{
public:
    typedef typename bp::basic_process< T >::handle_type handle_type;

    test(handle_type h, T a) :
        bp::basic_process< T >(h, a)
    {
    }
};

// ------------------------------------------------------------------------

void*
test_it(void)
{
    test< int >::handle_type h = static_cast< test< int >::handle_type >(0);
    return new test< int >(h, 5);
}
