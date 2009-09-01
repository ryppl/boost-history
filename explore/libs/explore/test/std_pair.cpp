// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <boost/explore/pair.hpp>
#include <boost/explore/vector.hpp>
#include <boost/explore/iterator_range.hpp>
#include "boost_explore_test_tools.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE( basic_pair_stream_test, C, test_types )
{
    typename test_traits<C>::stream_type str_out;

    std::pair<int,int> pi = std::make_pair(1,2);
    str_out << pi;
    BOOST_CHECK_EQUAL(output(str_out), "[1, 2]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( pair_in_vector_stream_test, C, test_types )
{
    typename test_traits<C>::stream_type str_out;

    std::vector<std::pair<int,int> > vpi;
    str_out << vpi;
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    std::pair<int,int> pi = std::make_pair(1,2);

    vpi.push_back(pi);
    str_out << vpi;
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2]]");

    reset(str_out);

    vpi.push_back(pi);
    vpi.push_back(pi);
    str_out << vpi;
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2], [1, 2], [1, 2]]");

    reset(str_out);

    str_out << boost::explore::make_iterator_range(vpi.begin(), ++(++vpi.begin()));
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2], [1, 2]]");
}
