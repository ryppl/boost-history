// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.

// Disclaimer: Not a Boost library.

#include <boost/interfaces.hpp>
#include "./detail/counter.hpp"
#include "./detail/template_hierarchies.hpp"
#include "./detail/unit_test_framework.hpp"

using boost::unit_test_framework::test_suite; 

void template_test()
{
    using namespace boost::interfaces::test;

    // Test initializing itemplate_first.
    ctemplate_first<1>                 template_first1;
    ctemplate_first<2>                 template_first2;
    itemplate_first<int, const char*>  template_first = template_first1;
    template_first.one(0);
    template_first.one("");
    template_first.two(0, "");
    BOOST_CHECK((counter< method<ctemplate_first<1>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_first<1>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_first<1>, func_two_tag, sig_c_tag> >::value()) == 1);

    // Test assigning to itemplate_first.
    template_first = template_first2;
    template_first.one(0);
    template_first.one("");
    template_first.two(0, "");
    BOOST_CHECK((counter< method<ctemplate_first<2>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_first<2>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_first<2>, func_two_tag, sig_c_tag> >::value()) == 1);

    // Test initializing itemplate_derived1.
    ctemplate_derived1<1>                   template_derived1_1;
    ctemplate_derived1<2>                   template_derived1_2;
    itemplate_derived1<int, const char*>    template_derived1 = template_derived1_1;
    template_derived1.one(0);
    template_derived1.one("");
    template_derived1.two(0, "");
    template_derived1.three(0);
    template_derived1.three("");
    template_derived1.four(0, "");
    BOOST_CHECK((counter< method<ctemplate_derived1<1>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived1<1>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived1<1>, func_two_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived1<1>, func_three_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived1<1>, func_three_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived1<1>, func_four_tag, sig_c_tag> >::value()) == 1);

    // Test assigning to itemplate_derived1.
    template_derived1 = template_derived1_2;
    template_derived1.one(0);
    template_derived1.one("");
    template_derived1.two(0, "");
    template_derived1.three(0);
    template_derived1.three("");
    template_derived1.four(0, "");
    BOOST_CHECK((counter< method<ctemplate_derived1<2>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived1<2>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived1<2>, func_two_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived1<2>, func_three_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived1<2>, func_three_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived1<2>, func_four_tag, sig_c_tag> >::value()) == 1);

    // Test initializing itemplate_derived4.
    ctemplate_derived4<1>                   template_derived4_1;
    ctemplate_derived4<2>                   template_derived4_2;
    itemplate_derived4<int, const char*>    template_derived4 = template_derived4_1;
    template_derived4.one(0);
    template_derived4.one("");
    template_derived4.two(0, "");
    template_derived4.three(0);
    template_derived4.three("");
    template_derived4.four(0, "");
    template_derived4.five(0);
    template_derived4.five("");
    template_derived4.six(0, "");
    template_derived4.seven(0);
    template_derived4.seven("");
    template_derived4.eight(0, "");
    BOOST_CHECK((counter< method<ctemplate_derived4<1>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<1>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<1>, func_two_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<1>, func_three_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<1>, func_three_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<1>, func_four_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<1>, func_five_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<1>, func_five_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<1>, func_six_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<1>, func_seven_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<1>, func_seven_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<1>, func_eight_tag, sig_c_tag> >::value()) == 1);

    // Test assigning to itemplate_derived4.
    template_derived4 = template_derived4_2;
    template_derived4.one(0);
    template_derived4.one("");
    template_derived4.two(0, "");
    template_derived4.three(0);
    template_derived4.three("");
    template_derived4.four(0, "");
    template_derived4.five(0);
    template_derived4.five("");
    template_derived4.six(0, "");
    template_derived4.seven(0);
    template_derived4.seven("");
    template_derived4.eight(0, "");
    BOOST_CHECK((counter< method<ctemplate_derived4<2>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<2>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<2>, func_two_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<2>, func_three_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<2>, func_three_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<2>, func_four_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<2>, func_five_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<2>, func_five_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<2>, func_six_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<2>, func_seven_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<2>, func_seven_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<ctemplate_derived4<2>, func_eight_tag, sig_c_tag> >::value()) == 1);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("template test");
    test->add(BOOST_TEST_CASE(&template_test));
    return test;
}
