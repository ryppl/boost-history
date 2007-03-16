// (C) Copyright Jonathan Turkanis 2004
// Copyright (c) 2002, 2003 Peter Dimov
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Portions addapted from <libs/smart_ptr/shared_ptr_test.cpp>

#include <boost/interfaces/unique_obj.hpp>
#include "../detail/counter.hpp"
#include "../detail/hierarchies.hpp"
#include "../detail/unit_test_framework.hpp"

using boost::unit_test_framework::test_suite;  
        
void deref_test()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;

    // Test initializing pfirst.
    unique_obj<ifirst> first(new cfirst<1>);
    first.one(0, 0.0F);
    first.one("");
    first.two();
    get(first)->one(0, 0.0F);
    get(first)->one("");
    get(first)->two();
    BOOST_CHECK((counter< method<cfirst<1>, func_one_tag, sig_a_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cfirst<1>, func_one_tag, sig_b_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cfirst<1>, func_two_tag, sig_c_tag> >::value()) == 2);

    // Test resetting first.
    reset(first, new cfirst<2>);
    first.one(0, 0.0F);
    first.one("");
    first.two();
    get(first)->one(0, 0.0F);
    get(first)->one("");
    get(first)->two();
    BOOST_CHECK((counter< method<cfirst<2>, func_one_tag, sig_a_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cfirst<2>, func_one_tag, sig_b_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cfirst<2>, func_two_tag, sig_c_tag> >::value()) == 2);

    // Test initializing derived1.
    unique_obj<iderived1> derived1(new cderived1<1>);
    derived1.one(0, 0.0F);
    derived1.one("");
    derived1.two();
    derived1.three(0, 0.0F);
    derived1.three("");
    derived1.four();
    get(derived1)->one(0, 0.0F);
    get(derived1)->one("");
    get(derived1)->two();
    get(derived1)->three(0, 0.0F);
    get(derived1)->three("");
    get(derived1)->four();
    BOOST_CHECK((counter< method<cderived1<1>, func_one_tag, sig_a_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived1<1>, func_one_tag, sig_b_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived1<1>, func_two_tag, sig_c_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived1<1>, func_three_tag, sig_a_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived1<1>, func_three_tag, sig_b_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived1<1>, func_four_tag, sig_c_tag> >::value()) == 2);

    // Test resetting derived1.
    reset(derived1, new cderived1<2>);
    derived1.one(0, 0.0F);
    derived1.one("");
    derived1.two();
    derived1.three(0, 0.0F);
    derived1.three("");
    derived1.four();
    get(derived1)->one(0, 0.0F);
    get(derived1)->one("");
    get(derived1)->two();
    get(derived1)->three(0, 0.0F);
    get(derived1)->three("");
    get(derived1)->four();
    BOOST_CHECK((counter< method<cderived1<2>, func_one_tag, sig_a_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived1<2>, func_one_tag, sig_b_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived1<2>, func_two_tag, sig_c_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived1<2>, func_three_tag, sig_a_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived1<2>, func_three_tag, sig_b_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived1<2>, func_four_tag, sig_c_tag> >::value()) == 2);

    // Test initializing derived4.
    unique_obj<iderived4> derived4(new cderived4<1>);
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
    get(derived4)->one(0, 0.0F);
    get(derived4)->one("");
    get(derived4)->two();
    get(derived4)->three(0, 0.0F);
    get(derived4)->three("");
    get(derived4)->four();
    get(derived4)->five(0, 0.0F);
    get(derived4)->five("");
    get(derived4)->six();
    get(derived4)->seven(0, 0.0F);
    get(derived4)->seven("");
    get(derived4)->eight();
    BOOST_CHECK((counter< method<cderived4<1>, func_one_tag, sig_a_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<1>, func_one_tag, sig_b_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<1>, func_two_tag, sig_c_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<1>, func_three_tag, sig_a_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<1>, func_three_tag, sig_b_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<1>, func_four_tag, sig_c_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<1>, func_five_tag, sig_a_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<1>, func_five_tag, sig_b_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<1>, func_six_tag, sig_c_tag> >::value()) == 2);

    // Test resetting derived4.
    reset(derived4, new cderived4<2>);
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
    get(derived4)->one(0, 0.0F);
    get(derived4)->one("");
    get(derived4)->two();
    get(derived4)->three(0, 0.0F);
    get(derived4)->three("");
    get(derived4)->four();
    get(derived4)->five(0, 0.0F);
    get(derived4)->five("");
    get(derived4)->six();
    get(derived4)->seven(0, 0.0F);
    get(derived4)->seven("");
    get(derived4)->eight();
    BOOST_CHECK((counter< method<cderived4<2>, func_one_tag, sig_a_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<2>, func_one_tag, sig_b_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<2>, func_two_tag, sig_c_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<2>, func_three_tag, sig_a_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<2>, func_three_tag, sig_b_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<2>, func_four_tag, sig_c_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<2>, func_five_tag, sig_a_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<2>, func_five_tag, sig_b_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<2>, func_six_tag, sig_c_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<2>, func_five_tag, sig_a_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<2>, func_five_tag, sig_b_tag> >::value()) == 2);
    BOOST_CHECK((counter< method<cderived4<2>, func_six_tag, sig_c_tag> >::value()) == 2);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("unique_obj deref test");
    test->add(BOOST_TEST_CASE(&deref_test));
    return test;
}
