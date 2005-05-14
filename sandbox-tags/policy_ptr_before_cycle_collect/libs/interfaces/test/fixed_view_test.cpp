// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.

// Disclaimer: Not a Boost library.

#include <boost/interfaces.hpp>
#include <boost/interfaces/fixed_view.hpp>
#include "./detail/counter.hpp"
#include "./detail/hierarchies.hpp"
#include "./detail/unit_test_framework.hpp"

using boost::unit_test_framework::test_suite; 

void fixed_view_test()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;

    // Test initializing ifirst.
    cfirst<1>           first1;
    cfirst<2>           first2;
    fixed_view<ifirst>  first = first1;
    first.one(0, 0.0F);
    first.one("");
    first.two();
    BOOST_CHECK((counter< method<cfirst<1>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cfirst<1>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cfirst<1>, func_two_tag, sig_c_tag> >::value()) == 1);

    // Test initializing iderived1.
    cderived1<1>           derived1_1;
    cderived1<2>           derived1_2;
    fixed_view<iderived1>  derived1 = derived1_1;
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

    // Test initializing iderived4.
    cderived4<1>           derived4_1;
    cderived4<2>           derived4_2;
    fixed_view<iderived4>  derived4 = derived4_1;
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
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("fixed_view test");
    test->add(BOOST_TEST_CASE(&fixed_view_test));
    return test;
}
