// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.

// Disclaimer: Not a Boost library.

#include <boost/interfaces.hpp>
#include "./detail/counter.hpp"
#include "./detail/hierarchies.hpp"
#include "./detail/unit_test_framework.hpp"

using boost::unit_test_framework::test_suite; 

void assign_zero_test()
{
    using namespace boost::interfaces::test;

    // Test assigning to first.
    cfirst<1> first1;
    cfirst<2> first2;
    ifirst    first;
    BOOST_CHECK(!first);
    first = first1;
    BOOST_CHECK(!!first);
    first = first2;
    BOOST_CHECK(!!first);
    first = 0;
    BOOST_CHECK(!first);

    // Test assigning to iderived1.
    cderived1<1> derived1_1;
    cderived1<2> derived1_2;
    iderived1    derived1;
    BOOST_CHECK(!derived1);
    derived1 = derived1_1;
    BOOST_CHECK(!!derived1);
    derived1 = derived1_2;
    BOOST_CHECK(!!derived1);
    derived1 = 0;
    BOOST_CHECK(!derived1);

    // Test assigning to iderived4.
    cderived4<1> derived4_1;
    cderived4<2> derived4_2;
    iderived4    derived4;
    BOOST_CHECK(!derived4);
    derived4 = derived4_1;
    BOOST_CHECK(!!derived4);
    derived4 = derived4_2;
    BOOST_CHECK(!!derived4);
    derived4 = 0;
    BOOST_CHECK(!derived1);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("assign zero test");
    test->add(BOOST_TEST_CASE(&assign_zero_test));
    return test;
}
