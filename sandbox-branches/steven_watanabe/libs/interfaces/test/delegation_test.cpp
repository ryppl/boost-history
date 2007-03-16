// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.

// Disclaimer: Not a Boost library.

#include <boost/interfaces.hpp>
#include <boost/interfaces/delegation.hpp>
#include "./detail/counter.hpp"
#include "./detail/hierarchies.hpp"
#include "./detail/unit_test_framework.hpp"

using boost::unit_test_framework::test_suite; 

namespace boost { namespace interfaces { namespace test {

class delegator 
    : public delegating<ifirst>,
      public delegating<isecond>
{
public:
    friend class boost::interfaces::access;
    delegator() 
    { 
        delegating<ifirst>::set(&first);
        delegating<isecond>::set(&second);
    }
private:
    cfirst<1>   first;
    csecond<1>  second;
};

} } } // End namespaces.

void delegation_test()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;

    delegator d;
    d.one(0, 0.0F);
    d.one("");
    d.two();
    d.three(0, 0.0F);
    d.three("");
    d.four();
    BOOST_CHECK((counter< method<cfirst<1>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cfirst<1>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cfirst<1>, func_two_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<csecond<1>, func_three_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<csecond<1>, func_three_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<csecond<1>, func_four_tag, sig_c_tag> >::value()) == 1);

    cfirst<2>  first1;
    csecond<2> second1;
    set_delegate<ifirst>(d, &first1);
    set_delegate<isecond>(d, &second1);
    d.one(0, 0.0F);
    d.one("");
    d.two();
    d.three(0, 0.0F);
    d.three("");
    d.four();
    BOOST_CHECK((counter< method<cfirst<2>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cfirst<2>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cfirst<2>, func_two_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<csecond<2>, func_three_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<csecond<2>, func_three_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<csecond<2>, func_four_tag, sig_c_tag> >::value()) == 1);

    cfirst<3>  first2;
    csecond<3> second2;
    set_delegate<ifirst>(d, &first2);
    set_delegate<isecond>(d, &second2);
    get_delegate<ifirst>(d)->one(0, 0.0F);
    get_delegate<ifirst>(d)->one("");
    get_delegate<ifirst>(d)->two();
    get_delegate<isecond>(d)->three(0, 0.0F);
    get_delegate<isecond>(d)->three("");
    get_delegate<isecond>(d)->four();
    BOOST_CHECK((counter< method<cfirst<3>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cfirst<3>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<cfirst<3>, func_two_tag, sig_c_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<csecond<3>, func_three_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<csecond<3>, func_three_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<csecond<3>, func_four_tag, sig_c_tag> >::value()) == 1);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("delegation test");
    test->add(BOOST_TEST_CASE(&delegation_test));
    return test;
}
