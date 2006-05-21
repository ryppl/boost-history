// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.

// Disclaimer: Not a Boost library.

#include <boost/interfaces.hpp>
#include "./detail/counter.hpp"
#include "./detail/hierarchies.hpp"
#include "./detail/unit_test_framework.hpp"

using namespace boost::interfaces::test;
using boost::unit_test_framework::test_suite; 

void test(ifirst first)
{
    first.one(0, 0.0F);
    first.one("");
    first.two();
}

void test(iderived1 derived1)
{
    derived1.one(0, 0.0F);
    derived1.one("");
    derived1.two();
    derived1.three(0, 0.0F);
    derived1.three("");
    derived1.four();
}

void test(iderived4 derived4)
{
    derived4.one(0, 0.0F);
    derived4.one("");
    derived4.two();
    derived4.three(0, 0.0F);
    derived4.three("");
    derived4.four();
    derived4.five(0, 0.0F);
    derived4.five("");
    derived4.six();
    derived4.seven(0, 0.0F);
    derived4.seven("");
    derived4.eight();
}

void pass_by_value_test()
{
    // Test initializing ifirst.
    cfirst<1> first1;
    cfirst<2> first2;
    ifirst    first = first1;
    test(first);
    BOOST_CHECK((counter< method<cfirst<1>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cfirst<1>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cfirst<1>, func_two_tag, sig_c_tag> >::value()) == 1);

    // Test assigning to ifirst.
    first = first2;
    test(first);
    BOOST_CHECK((counter< method<cfirst<2>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cfirst<2>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cfirst<2>, func_two_tag, sig_c_tag> >::value()) == 1);

    // Test initializing iderived1.
    cderived1<1> derived1_1;
    cderived1<2> derived1_2;
    iderived1    derived1 = derived1_1;
    test(derived1);
    BOOST_CHECK((counter< method<cderived1<1>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<1>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<1>, func_two_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<1>, func_three_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<1>, func_three_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<1>, func_four_tag, sig_c_tag> >::value()) == 1);

    // Test assigning to iderived1.
    derived1 = derived1_2;
    test(derived1);
    BOOST_CHECK((counter< method<cderived1<2>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<2>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<2>, func_two_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<2>, func_three_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<2>, func_three_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<2>, func_four_tag, sig_c_tag> >::value()) == 1);

    // Test initializing iderived4.
    cderived4<1> derived4_1;
    cderived4<2> derived4_2;
    iderived4    derived4 = derived4_1;
    test(derived4);
    BOOST_CHECK((counter< method<cderived4<1>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<1>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<1>, func_two_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<1>, func_three_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<1>, func_three_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<1>, func_four_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<1>, func_five_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<1>, func_five_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<1>, func_six_tag, sig_c_tag> >::value()) == 1);

    // Test assigning to iderived4.
    derived4 = derived4_2;
    test(derived4);
    BOOST_CHECK((counter< method<cderived4<2>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<2>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<2>, func_two_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<2>, func_three_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<2>, func_three_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<2>, func_four_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<2>, func_five_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<2>, func_five_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<2>, func_six_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<2>, func_five_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<2>, func_five_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<2>, func_six_tag, sig_c_tag> >::value()) == 1);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("pass by value test");
    test->add(BOOST_TEST_CASE(&pass_by_value_test));
    return test;
}
