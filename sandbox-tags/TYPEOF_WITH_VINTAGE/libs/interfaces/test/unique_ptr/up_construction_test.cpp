// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#include <boost/interfaces/unique_ptr.hpp>
#include "../detail/counter.hpp"
#include "../detail/hierarchies.hpp"
#include "../detail/unit_test_framework.hpp"

using boost::unit_test_framework::test_suite;    

void construction_test()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;

    {
        unique_ptr<ifirst> ptr1(new cfirst<1>);
        unique_ptr<ifirst> ptr2(unique_ptr<ifirst>(new cfirst<1>));
    }
    BOOST_CHECK(counter< cfirst<1> >::value() == 0);

    {
        unique_ptr<ifirst> ptr1(new cderived1<1>);
        unique_ptr<ifirst> ptr2(unique_ptr<iderived1>(new cderived1<1>));
    }
    BOOST_CHECK(counter< cderived1<1> >::value() == 0);

    {
        unique_ptr<ifirst> ptr1(new cderived4<1>);
        unique_ptr<ifirst> ptr2(unique_ptr<iderived4>(new cderived4<1>));
    }
    BOOST_CHECK(counter< cderived4<1> >::value() == 0);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("unique_ptr construction test");
    test->add(BOOST_TEST_CASE(&construction_test));
    return test;
}
