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

template<int N>
ifirst make_ifirst()
{
    static cfirst<N> first;
    return ifirst(first);
}

template<int N>
iderived1 make_iderived1()
{
    static cderived1<N> derived;
    return iderived1(derived);
}

template<int N>
iderived4 make_iderived4()
{
    static cderived4<N> derived;
    return iderived4(derived);
}

void return_by_value_test()
{
    // Test initializing ifirst.
    ifirst first = make_ifirst<1>();
    first.one(0, 0.0F);
    first.one("");
    first.two();
    BOOST_CHECK((counter< method<cfirst<1>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cfirst<1>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cfirst<1>, func_two_tag, sig_c_tag> >::value()) == 1);

    // Test assigning to ifirst.
    first = make_ifirst<2>();
    first.one(0, 0.0F);
    first.one("");
    first.two();
    BOOST_CHECK((counter< method<cfirst<2>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cfirst<2>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cfirst<2>, func_two_tag, sig_c_tag> >::value()) == 1);

    // Test initializing iderived1.
    iderived1 derived1 = make_iderived1<1>();
    derived1.one(0, 0.0F);
    derived1.one("");
    derived1.two();
    derived1.three(0, 0.0F);
    derived1.three("");
    derived1.four();
    BOOST_CHECK((counter< method<cderived1<1>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<1>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<1>, func_two_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<1>, func_three_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<1>, func_three_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<1>, func_four_tag, sig_c_tag> >::value()) == 1);

    // Test assigning to iderived1.
    derived1 = make_iderived1<2>();
    derived1.one(0, 0.0F);
    derived1.one("");
    derived1.two();
    derived1.three(0, 0.0F);
    derived1.three("");
    derived1.four();
    BOOST_CHECK((counter< method<cderived1<2>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<2>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<2>, func_two_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<2>, func_three_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<2>, func_three_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cderived1<2>, func_four_tag, sig_c_tag> >::value()) == 1);

    // Test initializing iderived4.
    iderived4    derived4 = make_iderived4<1>();
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
    derived4 = make_iderived4<2>();
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
    test_suite* test = BOOST_TEST_SUITE("return by value test");
    test->add(BOOST_TEST_CASE(&return_by_value_test));
    return test;
}
