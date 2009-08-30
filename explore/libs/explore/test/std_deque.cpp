// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <boost/explore/deque.hpp>
#include <boost/explore/iterator_range.hpp>
#include "boost_explore_test_tools.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE( basic_deque_stream_test, C, test_types )
{
    test_traits<C>::stream_type str_out;

    std::deque<int> di;
    str_out << di;
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    di.push_back(1);
    str_out << di;
    BOOST_CHECK_EQUAL(output(str_out), "[1]");

    reset(str_out);

    di.push_back(2);
    di.push_back(3);
    str_out << di;
    BOOST_CHECK_EQUAL(output(str_out), "[1, 2, 3]");

    reset(str_out);

    str_out << boost::explore::make_iterator_range(di.begin(), ++(++di.begin()));
    BOOST_CHECK_EQUAL(output(str_out), "[1, 2]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( deque_in_deque_stream_test, C, test_types )
{
    test_traits<C>::stream_type str_out;

    std::deque<int> di;
    di.push_back(1);
    di.push_back(2);
    di.push_back(3);

    std::deque<std::deque<int> > ddi;
    str_out << ddi;
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    ddi.push_back(di);
    str_out << ddi;
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3]]");

    reset(str_out);

    ddi.push_back(di);
    ddi.push_back(di);
    str_out << ddi;
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");

    reset(str_out);

    str_out << boost::explore::make_iterator_range(ddi.begin(), ++(++ddi.begin()));
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3], [1, 2, 3]]");
}
