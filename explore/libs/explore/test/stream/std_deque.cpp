// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <string>
#include <sstream>
#include <deque>
#include <boost/explore.hpp>

BOOST_AUTO_TEST_CASE( basic_deque_stream_test )
{
    std::stringstream str_out;

    std::deque<int> di;
    str_out << di;
    BOOST_CHECK_EQUAL(str_out.str(), "[]");

    str_out.str("");

    di.push_back(1);
    str_out << di;
    BOOST_CHECK_EQUAL(str_out.str(), "[1]");

    str_out.str("");

    di.push_back(2);
    di.push_back(3);
    str_out << di;
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2, 3]");

    str_out.str("");

    str_out << explore::make_iterator_range(di.begin(), ++(++di.begin()));
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2]");
}

BOOST_AUTO_TEST_CASE( deque_in_deque_stream_test )
{
    std::stringstream str_out;

    std::deque<int> di;
    di.push_back(1);
    di.push_back(2);
    di.push_back(3);

    std::deque<std::deque<int> > ddi;
    str_out << ddi;
    BOOST_CHECK_EQUAL(str_out.str(), "[]");

    str_out.str("");

    ddi.push_back(di);
    str_out << ddi;
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3]]");

    str_out.str("");

    ddi.push_back(di);
    ddi.push_back(di);
    str_out << ddi;
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");

    str_out.str("");

    str_out << explore::make_iterator_range(ddi.begin(), ++(++ddi.begin()));
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3], [1, 2, 3]]");
}
