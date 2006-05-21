// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#include <boost/interfaces/manual_ptr.hpp>
#include "../detail/counter.hpp"
#include "../detail/hierarchies.hpp"
#include "../detail/unit_test_framework.hpp"

using namespace boost::interfaces;
using namespace boost::interfaces::test;
using boost::unit_test_framework::test_suite; 

void assignment_test()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;

    {
        manual_ptr<ifirst> ptr1(new cfirst<1>);
        manual_ptr<ifirst> ptr2;
        ptr2 = ptr1;
        ptr2->one(0, 0.0F);
        ptr2->one("");
        ptr2->two();
        BOOST_CHECK((counter< method<cfirst<1>, func_one_tag, sig_a_tag> >::value()) == 1);
        BOOST_CHECK((counter< method<cfirst<1>, func_one_tag, sig_b_tag> >::value()) == 1);
        BOOST_CHECK((counter< method<cfirst<1>, func_two_tag, sig_c_tag> >::value()) == 1);
    }

    {
        manual_ptr<iderived1> ptr1(new cderived1<1>);
        manual_ptr<ifirst> ptr2;
        ptr2 = ptr1;
        ptr2->one(0, 0.0F);
        ptr2->one("");
        ptr2->two();
        BOOST_CHECK((counter< method<cderived1<1>, func_one_tag, sig_a_tag> >::value()) == 1);
        BOOST_CHECK((counter< method<cderived1<1>, func_one_tag, sig_b_tag> >::value()) == 1);
        BOOST_CHECK((counter< method<cderived1<1>, func_two_tag, sig_c_tag> >::value()) == 1);
    }

    {
        manual_ptr<iderived4> ptr1(new cderived4<1>);
        manual_ptr<ifirst> ptr2;
        ptr2 = ptr1;
        ptr2->one(0, 0.0F);
        ptr2->one("");
        ptr2->two();
        BOOST_CHECK((counter< method<cderived4<1>, func_one_tag, sig_a_tag> >::value()) == 1);
        BOOST_CHECK((counter< method<cderived4<1>, func_one_tag, sig_b_tag> >::value()) == 1);
        BOOST_CHECK((counter< method<cderived4<1>, func_two_tag, sig_c_tag> >::value()) == 1);
    }
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("manual_ptr assignment test");
    test->add(BOOST_TEST_CASE(&assignment_test));
    return test;
}
