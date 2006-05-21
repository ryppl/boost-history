// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.

// Disclaimer: Not a Boost library.

#include <boost/interfaces.hpp>
#include <boost/interfaces/crosscut.hpp>
#include "./detail/advice.hpp"
#include "./detail/counter.hpp"
#include "./detail/hierarchies.hpp"
#include "./detail/method_tags.hpp"
#include "./detail/unit_test_framework.hpp"

using boost::unit_test_framework::test_suite; 

namespace boost { namespace interfaces { namespace test {

struct first_tag { };
struct second_tag { };
struct third_tag { };
struct fourth_tag { };

#define BOOST_IDL_ENABLE_ADVICE

BOOST_IDL_BEGIN(iadvice1)
    BOOST_IDL_FN1(one, void, (int, param))
BOOST_IDL_END(iadvice1)

#undef BOOST_IDL_ENABLE_ADVICE

BOOST_IDL_BEGIN(iadviceless1)
    BOOST_IDL_FN1(one, void, int)
BOOST_IDL_END(iadviceless1)

#define BOOST_IDL_ENABLE_ADVICE
#define BOOST_IDL_NO_NAMED_PARAMS

BOOST_IDL_BEGIN(iadvice2)
    BOOST_IDL_FN1(one, void, int)
BOOST_IDL_END(iadvice2)

#undef BOOST_IDL_ENABLE_ADVICE

BOOST_IDL_BEGIN(iadviceless2)
    BOOST_IDL_FN1(one, void, int)
BOOST_IDL_END(iadviceless2)

} } } // End namespaces.

void crosscut_test()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;

    typedef crosscut_interface<
                iadvice1,
                aspect<iadvice1, advice1<first_tag> >
            > crosscut1;

    typedef crosscut_interface<
                iadviceless1,
                aspect<iadviceless1, advice1<second_tag> >
            > crosscut2;

    typedef crosscut_interface<
                iadvice2,
                aspect<iadvice2, advice1<third_tag> >
            > crosscut3;

    typedef crosscut_interface<
                iadviceless2,
                aspect<iadviceless2, advice1<fourth_tag> >
            > crosscut4;

    cpointcut obj;

    crosscut1 c1 = obj;
    c1.one(0);
    int seq1_a = sequencer< method<first_tag, before_tag> >::value();
    int seq1_b = sequencer< method<first_tag, cancel_tag> >::value();
    int seq1_c = sequencer< method<cpointcut, func_one_tag> >::value();
    int seq1_d = sequencer< method<first_tag, after_tag> >::value();
    int seq1_e = sequencer< method<first_tag, finally_tag> >::value();
    BOOST_CHECK(seq1_a != -1);
    BOOST_CHECK(seq1_a < seq1_b);
    BOOST_CHECK(seq1_b < seq1_c);
    BOOST_CHECK(seq1_c < seq1_d);
    BOOST_CHECK(seq1_d < seq1_e);

    crosscut2 c2 = obj;
    c2.one(0);
    int seq2_a = sequencer< method<second_tag, before_tag> >::value();
    int seq2_b = sequencer< method<second_tag, cancel_tag> >::value();
    int seq2_c = sequencer< method<cpointcut, func_one_tag> >::value();
    int seq2_d = sequencer< method<second_tag, after_tag> >::value();
    int seq2_e = sequencer< method<second_tag, finally_tag> >::value();
    BOOST_CHECK(seq2_c != -1);
    BOOST_CHECK(seq2_a == -1);
    BOOST_CHECK(seq2_b == -1);
    BOOST_CHECK(seq2_d == -1);
    BOOST_CHECK(seq2_e == -1);

    crosscut3 c3 = obj;
    c3.one(0);
    int seq3_a = sequencer< method<third_tag, before_tag> >::value();
    int seq3_b = sequencer< method<third_tag, cancel_tag> >::value();
    int seq3_c = sequencer< method<cpointcut, func_one_tag> >::value();
    int seq3_d = sequencer< method<third_tag, after_tag> >::value();
    int seq3_e = sequencer< method<third_tag, finally_tag> >::value();
    BOOST_CHECK(seq3_a != -1);
    BOOST_CHECK(seq3_a < seq3_b);
    BOOST_CHECK(seq3_b < seq3_c);
    BOOST_CHECK(seq3_c < seq3_d);
    BOOST_CHECK(seq3_d < seq3_e);

    crosscut4 c4 = obj;
    c4.one(0);
    int seq4_a = sequencer< method<fourth_tag, before_tag> >::value();
    int seq4_b = sequencer< method<fourth_tag, cancel_tag> >::value();
    int seq4_c = sequencer< method<cpointcut, func_one_tag> >::value();
    int seq4_d = sequencer< method<fourth_tag, after_tag> >::value();
    int seq4_e = sequencer< method<fourth_tag, finally_tag> >::value();
    BOOST_CHECK(seq4_c != -1);
    BOOST_CHECK(seq4_a == -1);
    BOOST_CHECK(seq4_b == -1);
    BOOST_CHECK(seq4_d == -1);
    BOOST_CHECK(seq4_e == -1);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("crosscut test");
    test->add(BOOST_TEST_CASE(&crosscut_test));
    return test;
}
