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

} } } // End namespaces.

void crosscut_test()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;

    cpointcut obj;

        // Test single crosscut.

    typedef crosscut_interface<
                ipointcut,
                aspect<ipointcut, advice1<first_tag> >
            > crosscut1;
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

        // Test double crosscut.

    typedef crosscut_interface<
                crosscut1,
                aspect<crosscut1, advice1<second_tag> >
            > crosscut2;

    crosscut2 c2 = obj;
    c2.one(0);
    int seq2_a = sequencer< method<second_tag, before_tag> >::value();
    int seq2_b = sequencer< method<first_tag, before_tag> >::value();
    int seq2_c = sequencer< method<second_tag, cancel_tag> >::value();
    int seq2_d = sequencer< method<first_tag, cancel_tag> >::value();
    int seq2_e = sequencer< method<cpointcut, func_one_tag> >::value();
    int seq2_f = sequencer< method<first_tag, after_tag> >::value();
    int seq2_g = sequencer< method<second_tag, after_tag> >::value();
    int seq2_h = sequencer< method<first_tag, finally_tag> >::value();
    int seq2_i = sequencer< method<second_tag, finally_tag> >::value();
    BOOST_CHECK(seq2_a != -1);
    BOOST_CHECK(seq2_a < seq2_b);
    BOOST_CHECK(seq2_b < seq2_c);
    BOOST_CHECK(seq2_c < seq2_d);
    BOOST_CHECK(seq2_d < seq2_e);
    BOOST_CHECK(seq2_e < seq2_f);
    BOOST_CHECK(seq2_f < seq2_g);
    BOOST_CHECK(seq2_g < seq2_h);
    BOOST_CHECK(seq2_h < seq2_i);

        // Test triple crosscut.

    typedef crosscut_interface<
                crosscut2,
                aspect<crosscut2, advice1<third_tag> >
            > crosscut3;

    crosscut3 c3 = obj;
    c3.one(0);
    int seq3_a = sequencer< method<third_tag, before_tag> >::value();
    int seq3_b = sequencer< method<second_tag, before_tag> >::value();
    int seq3_c = sequencer< method<first_tag, before_tag> >::value();
    int seq3_d = sequencer< method<third_tag, cancel_tag> >::value();
    int seq3_e = sequencer< method<second_tag, cancel_tag> >::value();
    int seq3_f = sequencer< method<first_tag, cancel_tag> >::value();
    int seq3_g = sequencer< method<cpointcut, func_one_tag> >::value();
    int seq3_h = sequencer< method<first_tag, after_tag> >::value();
    int seq3_i = sequencer< method<second_tag, after_tag> >::value();
    int seq3_j = sequencer< method<third_tag, after_tag> >::value();
    int seq3_k = sequencer< method<first_tag, finally_tag> >::value();
    int seq3_l = sequencer< method<second_tag, finally_tag> >::value();
    int seq3_m = sequencer< method<third_tag, finally_tag> >::value();
    BOOST_CHECK(seq3_a != -1);
    BOOST_CHECK(seq3_a < seq3_b);
    BOOST_CHECK(seq3_b < seq3_c);
    BOOST_CHECK(seq3_c < seq3_d);
    BOOST_CHECK(seq3_d < seq3_e);
    BOOST_CHECK(seq3_e < seq3_f);
    BOOST_CHECK(seq3_f < seq3_g);
    BOOST_CHECK(seq3_g < seq3_h);
    BOOST_CHECK(seq3_h < seq3_i);
    BOOST_CHECK(seq3_i < seq3_j);
    BOOST_CHECK(seq3_j < seq3_k);
    BOOST_CHECK(seq3_k < seq3_l);
    BOOST_CHECK(seq3_l < seq3_m);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("crosscut test");
    test->add(BOOST_TEST_CASE(&crosscut_test));
    return test;
}
