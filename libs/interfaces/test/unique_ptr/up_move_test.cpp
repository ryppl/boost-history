// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#include <boost/interfaces/unique_ptr.hpp>
#include "../detail/counter.hpp"
#include "../detail/hierarchies.hpp"
#include "../detail/unit_test_framework.hpp"

using namespace boost;
using namespace boost::interfaces;
using namespace boost::interfaces::test;
using boost::unit_test_framework::test_suite; 

void sink(unique_ptr<ifirst> ptr) 
{ 
    BOOST_CHECK(ptr ? true : false);
    BOOST_CHECK(!!ptr);
    BOOST_CHECK(ptr.get() != 0);
}

void move_test()
{
    {
        unique_ptr<ifirst> ptr(new cfirst<1>);
        sink(move(ptr));
    }
    BOOST_CHECK(counter< cfirst<1> >::value() == 0);

    {
        unique_ptr<iderived1> ptr(new cderived1<1>);
        sink(move(ptr));
    }
    BOOST_CHECK(counter< cderived1<1> >::value() == 0);

    {
        unique_ptr<iderived4> ptr(new cderived4<1>);
        sink(move(ptr));
    }
    BOOST_CHECK(counter< cderived4<1> >::value() == 0);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("unique_ptr move test");
    test->add(BOOST_TEST_CASE(&move_test));
    return test;
}
