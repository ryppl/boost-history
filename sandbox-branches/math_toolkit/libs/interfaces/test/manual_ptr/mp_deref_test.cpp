// (C) Copyright Jonathan Turkanis 2004
// Copyright (c) 2002, 2003 Peter Dimov
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Portions addapted from <libs/smart_ptr/shared_ptr_test.cpp>

#include <boost/interfaces/manual_ptr.hpp>
#include "../detail/counter.hpp"
#include "../detail/hierarchies.hpp"
#include "../detail/unit_test_framework.hpp"

using boost::unit_test_framework::test_suite;  
        
void deref_test()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;

    // Test initializing pfirst.
    manual_ptr<ifirst> pfirst(new cfirst<1>);
    pfirst->one(0, 0.0F);
    pfirst->one("");
    pfirst->two();
    (*pfirst).one(0, 0.0F);
    (*pfirst).one("");
    (*pfirst).two();
    pfirst.get()->one(0, 0.0F);
    pfirst.get()->one("");
    pfirst.get()->two();
    BOOST_CHECK((counter< method<cfirst<1>, func_one_tag, sig_a_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cfirst<1>, func_one_tag, sig_b_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cfirst<1>, func_two_tag, sig_c_tag> >::value()) == 3);

    // Test resetting pfirst.
    pfirst.reset(new cfirst<2>);
    pfirst->one(0, 0.0F);
    pfirst->one("");
    pfirst->two();
    (*pfirst).one(0, 0.0F);
    (*pfirst).one("");
    (*pfirst).two();
    pfirst.get()->one(0, 0.0F);
    pfirst.get()->one("");
    pfirst.get()->two();
    BOOST_CHECK((counter< method<cfirst<2>, func_one_tag, sig_a_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cfirst<2>, func_one_tag, sig_b_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cfirst<2>, func_two_tag, sig_c_tag> >::value()) == 3);

    // Test resetting pfirst.
    pfirst.reset(new cfirst<2>);

    // Test initializing pderived1.
    manual_ptr<iderived1> pderived1(new cderived1<1>);
    pderived1->one(0, 0.0F);
    pderived1->one("");
    pderived1->two();
    pderived1->three(0, 0.0F);
    pderived1->three("");
    pderived1->four();
    (*pderived1).one(0, 0.0F);
    (*pderived1).one("");
    (*pderived1).two();
    (*pderived1).three(0, 0.0F);
    (*pderived1).three("");
    (*pderived1).four();
    pderived1.get()->one(0, 0.0F);
    pderived1.get()->one("");
    pderived1.get()->two();
    pderived1.get()->three(0, 0.0F);
    pderived1.get()->three("");
    pderived1.get()->four();
    BOOST_CHECK((counter< method<cderived1<1>, func_one_tag, sig_a_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived1<1>, func_one_tag, sig_b_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived1<1>, func_two_tag, sig_c_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived1<1>, func_three_tag, sig_a_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived1<1>, func_three_tag, sig_b_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived1<1>, func_four_tag, sig_c_tag> >::value()) == 3);

    // Test resetting pderived1.
    pderived1.reset(new cderived1<2>);
    pderived1->one(0, 0.0F);
    pderived1->one("");
    pderived1->two();
    pderived1->three(0, 0.0F);
    pderived1->three("");
    pderived1->four();
    (*pderived1).one(0, 0.0F);
    (*pderived1).one("");
    (*pderived1).two();
    (*pderived1).three(0, 0.0F);
    (*pderived1).three("");
    (*pderived1).four();
    pderived1.get()->one(0, 0.0F);
    pderived1.get()->one("");
    pderived1.get()->two();
    pderived1.get()->three(0, 0.0F);
    pderived1.get()->three("");
    pderived1.get()->four();
    BOOST_CHECK((counter< method<cderived1<2>, func_one_tag, sig_a_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived1<2>, func_one_tag, sig_b_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived1<2>, func_two_tag, sig_c_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived1<2>, func_three_tag, sig_a_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived1<2>, func_three_tag, sig_b_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived1<2>, func_four_tag, sig_c_tag> >::value()) == 3);

    // Test initializing pderived4.
    manual_ptr<iderived4> pderived4(new cderived4<1>);
    pderived4->one(0, 0.0F);
    pderived4->one("");
    pderived4->two();
    pderived4->three(0, 0.0F);
    pderived4->three("");
    pderived4->four();
    pderived4->five(0, 0.0F);
    pderived4->five("");
    pderived4->six();
    pderived4->seven(0, 0.0F);
    pderived4->seven("");
    pderived4->eight();
    (*pderived4).one(0, 0.0F);
    (*pderived4).one("");
    (*pderived4).two();
    (*pderived4).three(0, 0.0F);
    (*pderived4).three("");
    (*pderived4).four();
    (*pderived4).five(0, 0.0F);
    (*pderived4).five("");
    (*pderived4).six();
    (*pderived4).seven(0, 0.0F);
    (*pderived4).seven("");
    (*pderived4).eight();
    pderived4.get()->one(0, 0.0F);
    pderived4.get()->one("");
    pderived4.get()->two();
    pderived4.get()->three(0, 0.0F);
    pderived4.get()->three("");
    pderived4.get()->four();
    pderived4.get()->five(0, 0.0F);
    pderived4.get()->five("");
    pderived4.get()->six();
    pderived4.get()->seven(0, 0.0F);
    pderived4.get()->seven("");
    pderived4.get()->eight();
    BOOST_CHECK((counter< method<cderived4<1>, func_one_tag, sig_a_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<1>, func_one_tag, sig_b_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<1>, func_two_tag, sig_c_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<1>, func_three_tag, sig_a_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<1>, func_three_tag, sig_b_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<1>, func_four_tag, sig_c_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<1>, func_five_tag, sig_a_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<1>, func_five_tag, sig_b_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<1>, func_six_tag, sig_c_tag> >::value()) == 3);

    // Test resetting pderived4.
    pderived4.reset(new cderived4<2>);
    pderived4->one(0, 0.0F);
    pderived4->one("");
    pderived4->two();
    pderived4->three(0, 0.0F);
    pderived4->three("");
    pderived4->four();
    pderived4->five(0, 0.0F);
    pderived4->five("");
    pderived4->six();
    pderived4->seven(0, 0.0F);
    pderived4->seven("");
    pderived4->eight();
    (*pderived4).one(0, 0.0F);
    (*pderived4).one("");
    (*pderived4).two();
    (*pderived4).three(0, 0.0F);
    (*pderived4).three("");
    (*pderived4).four();
    (*pderived4).five(0, 0.0F);
    (*pderived4).five("");
    (*pderived4).six();
    (*pderived4).seven(0, 0.0F);
    (*pderived4).seven("");
    (*pderived4).eight();
    pderived4.get()->one(0, 0.0F);
    pderived4.get()->one("");
    pderived4.get()->two();
    pderived4.get()->three(0, 0.0F);
    pderived4.get()->three("");
    pderived4.get()->four();
    pderived4.get()->five(0, 0.0F);
    pderived4.get()->five("");
    pderived4.get()->six();
    pderived4.get()->seven(0, 0.0F);
    pderived4.get()->seven("");
    pderived4.get()->eight();
    BOOST_CHECK((counter< method<cderived4<2>, func_one_tag, sig_a_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<2>, func_one_tag, sig_b_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<2>, func_two_tag, sig_c_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<2>, func_three_tag, sig_a_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<2>, func_three_tag, sig_b_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<2>, func_four_tag, sig_c_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<2>, func_five_tag, sig_a_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<2>, func_five_tag, sig_b_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<2>, func_six_tag, sig_c_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<2>, func_five_tag, sig_a_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<2>, func_five_tag, sig_b_tag> >::value()) == 3);
    BOOST_CHECK((counter< method<cderived4<2>, func_six_tag, sig_c_tag> >::value()) == 3);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("manual_ptr deref test");
    test->add(BOOST_TEST_CASE(&deref_test));
    return test;
}
