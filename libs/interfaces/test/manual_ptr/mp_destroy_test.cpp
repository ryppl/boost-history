// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#include <boost/interfaces/manual_ptr.hpp>
#include "../detail/counter.hpp"
#include "../detail/hierarchies.hpp"
#include "../detail/unit_test_framework.hpp"

using boost::unit_test_framework::test_suite; 

void destroy_test()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;

    {

        manual_ptr<ifirst> ptr(new cfirst<1>);
        int i = counter< cfirst<1> >::value();
        int x = 0;
    }
    int i = counter< cfirst<1> >::value();
    BOOST_CHECK(counter< cfirst<1> >::value() == 1);

    {
        manual_ptr<ifirst> ptr(new cfirst<2>);
        ptr.destroy();
    }
    BOOST_CHECK(counter< cfirst<2> >::value() == 0);

    {
        manual_ptr<iderived1> ptr1(new cderived1<1>);
        manual_ptr<ifirst> ptr2(ptr1);
        ptr2.destroy();
    }
    BOOST_CHECK(counter< cderived1<1> >::value() == 0);

    {
        manual_ptr<iderived4> ptr1(new cderived4<1>);
        manual_ptr<ifirst> ptr2(ptr1);
        ptr2.destroy();
    }
    BOOST_CHECK(counter< cderived4<1> >::value() == 0);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("manual_ptr destroy test");
    test->add(BOOST_TEST_CASE(&destroy_test));
    return test;
}
