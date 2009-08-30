// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/explore.hpp>
#include "boost_explore_test_tools.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE( boost_range_stream_test, C, test_types )
{
    test_traits<C>::stream_type str_out;

    std::vector<int> vi;
    str_out << boost::explore::make_iterator_range(vi);
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    vi.push_back(1);
    str_out << boost::explore::make_iterator_range(vi);
    BOOST_CHECK_EQUAL(output(str_out), "[1]");

    reset(str_out);

    vi.push_back(2);
    vi.push_back(3);
    str_out << boost::explore::make_iterator_range(vi);
    BOOST_CHECK_EQUAL(output(str_out), "[1, 2, 3]");

    reset(str_out);
    str_out << boost::explore::make_iterator_range(vi.begin()+2, vi.end());
    BOOST_CHECK_EQUAL(output(str_out), "[3]");

    reset(str_out);
    str_out << boost::explore::make_iterator_range(vi.rbegin(), vi.rend());
    BOOST_CHECK_EQUAL(output(str_out), "[3, 2, 1]");
}
