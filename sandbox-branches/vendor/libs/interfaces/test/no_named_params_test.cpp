// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.

// Disclaimer: Not a Boost library.

#include <boost/interfaces.hpp>
#include "./detail/counter.hpp"
#include "./detail/hierarchies.hpp"
#include "./detail/unit_test_framework.hpp"

using boost::unit_test_framework::test_suite; 

namespace boost { namespace interfaces { namespace test {

BOOST_IDL_BEGIN(inamed1)
    BOOST_IDL_FN2(one, char, (int, height), (float, weight))
    BOOST_IDL_CONST_FN1(one, void, (const char*, name))
    BOOST_IDL_CONST_FN0(two, void)
BOOST_IDL_END(inamed1)

#define BOOST_IDL_NO_NAMED_PARAMS

BOOST_IDL_BEGIN(iunnamed1)
    BOOST_IDL_FN2(one, char, int, float)
    BOOST_IDL_CONST_FN1(one, void, const char*)
    BOOST_IDL_CONST_FN0(two, void)
BOOST_IDL_END(iunnamed1)

#undef BOOST_IDL_NO_NAMED_PARAMS

BOOST_IDL_BEGIN(inamed2)
    BOOST_IDL_FN2(one, char, (int, height), (float, weight))
    BOOST_IDL_CONST_FN1(one, void, (const char*, name))
    BOOST_IDL_CONST_FN0(two, void)
BOOST_IDL_END(inamed2)

#define BOOST_IDL_LEAN_AND_MEAN

BOOST_IDL_BEGIN(iunnamed2)
    BOOST_IDL_FN2(one, char, int, float)
    BOOST_IDL_CONST_FN1(one, void, const char*)
    BOOST_IDL_CONST_FN0(two, void)
BOOST_IDL_END(iunnamed2)

template<int N>
struct test {
    char one(int i, float f)
    { 
        counter< method<test<N>, func_one_tag, sig_a_tag> >::inc();
        return 0; 
    }
    void one(const char* h) const 
    { 
        counter< method<test<N>, func_one_tag, sig_b_tag> >::inc();
    }
    void two() const 
    { 
        counter< method<test<N>, func_two_tag, sig_c_tag> >::inc();
    }
};

} } } // End namespaces.

void no_named_params_test()
{
    using namespace boost::interfaces::test;
    test<1> test1;
    inamed1 named1 = test1;
    named1.one(0, 0.0F);
    named1.one("");
    named1.two();
    BOOST_CHECK((counter< method<test<1>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<test<1>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<test<1>, func_two_tag, sig_c_tag> >::value()) == 1);

    test<2> test2;
    iunnamed1 unnamed1 = test2;
    unnamed1.one(0, 0.0F);
    unnamed1.one("");
    unnamed1.two();
    BOOST_CHECK((counter< method<test<2>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<test<2>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<test<2>, func_two_tag, sig_c_tag> >::value()) == 1);

    test<3> test3;
    inamed2 named2 = test3;
    named2.one(0, 0.0F);
    named2.one("");
    named2.two();
    BOOST_CHECK((counter< method<test<3>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<test<3>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<test<3>, func_two_tag, sig_c_tag> >::value()) == 1);

    test<4> test4;
    inamed1 unnamed2 = test4;
    unnamed2.one(0, 0.0F);
    unnamed2.one("");
    unnamed2.two();
    BOOST_CHECK((counter< method<test<4>, func_one_tag, sig_a_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<test<4>, func_one_tag, sig_b_tag> >::value()) == 1);
    BOOST_CHECK((counter< method<test<4>, func_two_tag, sig_c_tag> >::value()) == 1);
}

test_suite* init_unit_test_suite(int, char* [])
{
    test_suite* test = BOOST_TEST_SUITE("no named params test");
    test->add(BOOST_TEST_CASE(&no_named_params_test));
    return test;
}
