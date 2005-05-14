// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#include <boost/interfaces/manual_ptr.hpp>
#include <boost/interfaces/unique_ptr.hpp>
#include "../detail/counter.hpp"
#include "../detail/hierarchies.hpp"
#include "../detail/unit_test_framework.hpp"

using namespace boost::interfaces;
using namespace boost::interfaces::test;
using boost::unit_test_framework::test_suite; 

void release_test()
{
    manual_ptr<ifirst> ptr1;
    { 
        unique_ptr<ifirst> ptr2(new cfirst<1>); 
        ptr1 = ptr2.release();
    }
    BOOST_CHECK(counter< cfirst<1> >::value() == 1);
    ptr1.destroy();
    BOOST_CHECK(counter< cfirst<1> >::value() == 0);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("unique_ptr release test");
    test->add(BOOST_TEST_CASE(&release_test));
    return test;
}
