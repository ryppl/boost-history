// Boost.Explore library
//
// Copyright (C) 2009, Jeffrey Faust
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>

#include <boost/explore/vector.hpp>
#include <boost/explore/state_saver.hpp>
#include <iostream>

#include "boost_explore_test_tools.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE( save_state, C, test_types )
{
    typename test_traits<C>::stream_type str_out;

    using namespace boost::explore;

    std::vector<unsigned int> vui;
    vui.push_back('A');
    vui.push_back('B');

    container_stream_state<C> state_before = *get_stream_state<container_stream_state<C> >(str_out);
    container_common_stream_state common_state_before = *get_stream_state<container_common_stream_state>(str_out);

    {
        state_saver<C> ss(str_out);
        str_out << item_width(10) << delimiters(str_to<C>("\\"), str_to<C>(";"), str_to<C>("/"));
        str_out << vui;
        BOOST_CHECK_EQUAL(output(str_out), "\\        65;        66/");
    }

    str_out << vui;
    BOOST_CHECK_EQUAL(output(str_out), "\\        65;        66/[65, 66]");

    BOOST_CHECK(state_before == *get_stream_state<container_stream_state<C> >(str_out));
    BOOST_CHECK(common_state_before == *get_stream_state<container_common_stream_state>(str_out));
}
