// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.

// Disclaimer: Not a Boost library.

#include <boost/interfaces.hpp>
#include "./detail/counter.hpp"
#include "./detail/hierarchies.hpp"
#include "./detail/unit_test_framework.hpp"

using namespace boost::interfaces;
using namespace boost::interfaces::test;
using boost::unit_test_framework::test_suite; 

void test_ifirst(ifirst first)
{
    first.one(0, 0.0F);
    first.one("");
    first.two();
}

void test_isecond(isecond second)
{
    second.three(0, 0.0F);
    second.three("");
    second.four();
}

void test_iderived1(iderived1 derived1)
{
    derived1.one(0, 0.0F);
    derived1.one("");
    derived1.two();
    derived1.three(0, 0.0F);
    derived1.three("");
    derived1.four();
}

void test_ithird(ithird third)
{
    third.five(0, 0.0F);
    third.five("");
    third.six();
}

void test_ifourth(ifourth fourth)
{
    fourth.seven(0, 0.0F);
    fourth.seven("");
    fourth.eight();
}

void test_iderived3(iderived3 derived3)
{
    derived3.five(0, 0.0F);
    derived3.five("");
    derived3.six();
    derived3.seven(0, 0.0F);
    derived3.seven("");
    derived3.eight();
}

void derived_to_base_test2()
{
    // Test converting iderived1 to ifirst.
    cderived1<1> derived1_1;
    ifirst first1;
    first1 = iderived1(derived1_1);
    test_ifirst(first1);
    BOOST_CHECK((counter< method<cderived1<1>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<1>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<1>, func_two_tag, sig_c_tag> >::value()) == 1);

    // Test converting iderived1 to isecond.
    cderived1<2> derived1_2;
    isecond second1;
    second1 = iderived1(derived1_2);
    test_isecond(second1);
    BOOST_CHECK((counter< method<cderived1<2>, func_three_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<2>, func_three_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<2>, func_four_tag, sig_c_tag> >::value()) == 1);

    // Test converting iderived4 to ifirst.
    cderived4<1> derived4_1;
    ifirst first2;
    first2 = iderived4(derived4_1);
    test_ifirst(first2);
    BOOST_CHECK((counter< method<cderived4<1>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<1>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<1>, func_two_tag, sig_c_tag> >::value()) == 1);

    // Test converting iderived4 to isecond.
    cderived4<2> derived4_2;
    isecond second2;
    second2 = iderived4(derived4_2);
    test_isecond(second2);
    BOOST_CHECK((counter< method<cderived4<2>, func_three_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<2>, func_three_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<2>, func_four_tag, sig_c_tag> >::value()) == 1);

    // Test converting iderived4 to iderived1.
    cderived4<3> derived4_3;
    iderived1 derived1;
    derived1 = iderived4(derived4_3);
    test_iderived1(derived1);
    BOOST_CHECK((counter< method<cderived4<3>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<3>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<3>, func_two_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<3>, func_three_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<3>, func_three_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<3>, func_four_tag, sig_c_tag> >::value()) == 1);

    // Test converting iderived4 to ithird.
    cderived4<4> derived4_4;
    ithird third;
    third = iderived4(derived4_4);
    test_ithird(third);
    BOOST_CHECK((counter< method<cderived4<4>, func_five_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<4>, func_five_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<4>, func_six_tag, sig_c_tag> >::value()) == 1);

    // Test converting iderived4 to ifourth.
    cderived4<5> derived4_5;
    ifourth fourth;
    fourth = iderived4(derived4_5);
    test_ifourth(fourth);
    BOOST_CHECK((counter< method<cderived4<5>, func_seven_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<5>, func_seven_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<5>, func_eight_tag, sig_c_tag> >::value()) == 1);

    // Test converting iderived4 to iderived3.
    cderived4<6> derived4_6;
    iderived3 derived3;
    derived3 = iderived4(derived4_6);
    test_iderived3(derived3);
    BOOST_CHECK((counter< method<cderived4<6>, func_five_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<6>, func_five_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<6>, func_six_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<6>, func_seven_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<6>, func_seven_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived4<6>, func_eight_tag, sig_c_tag> >::value()) == 1);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("derived to base test 2");
    test->add(BOOST_TEST_CASE(&derived_to_base_test2));
    return test;
}
