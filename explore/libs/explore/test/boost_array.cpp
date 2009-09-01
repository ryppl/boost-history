// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <boost/array.hpp>
#include <boost/explore/boost_array.hpp>
#include <boost/explore/vector.hpp>
#include <boost/explore/iterator_range.hpp>
#include "boost_explore_test_tools.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE(basic_array_stream_test, C, test_types)
{
    typename test_traits<C>::stream_type str_out;

    boost::array<int,0> ai0;
    str_out << ai0;
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    boost::array<int,1> ai1 = {1};
    str_out << ai1;
    BOOST_CHECK_EQUAL(output(str_out), "[1]");

    reset(str_out);

    boost::array<int,3> ai3 = {1, 2, 3};
    str_out << ai3;
    BOOST_CHECK_EQUAL(output(str_out), "[1, 2, 3]");

    reset(str_out);

    boost::array<int,3>::iterator itr = ai3.begin() + 2;

    str_out << boost::explore::make_iterator_range(ai3.begin(), itr);
    BOOST_CHECK_EQUAL(output(str_out), "[1, 2]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(vector_in_array_stream_test, C, test_types)
{
    typename test_traits<C>::stream_type str_out;

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);

    boost::array<std::vector<int>,0> av0;
    str_out << av0;
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    boost::array<std::vector<int>,1> av1 = {vi};
    str_out << av1;
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3]]");

    reset(str_out);

    boost::array<std::vector<int>,3> av3 = {vi, vi, vi};
    str_out << av3;
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");

    reset(str_out);

    boost::array<std::vector<int>,3>::iterator itr = av3.begin() + 2;
    str_out << boost::explore::make_iterator_range(av3.begin(), itr);
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3], [1, 2, 3]]");
}
