// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <sstream>
#include <vector>
#include <boost/range/iterator_range.hpp>
#include <boost/explore.hpp>

BOOST_AUTO_TEST_CASE( boost_range_print_test )
{
    std::stringstream str_out;

    std::vector<int> vi;
    explore::print(boost::make_iterator_range(vi), str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[]");

    str_out.str("");

    vi.push_back(1);
    explore::print(boost::make_iterator_range(vi), str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[1]");

    str_out.str("");

    vi.push_back(2);
    vi.push_back(3);
    explore::print(boost::make_iterator_range(vi), str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2, 3]");

    str_out.str("");
    explore::print(boost::make_iterator_range(vi.begin()+2, vi.end()), str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[3]");

    str_out.str("");
    explore::print(boost::make_iterator_range(vi.rbegin(), vi.rend()), str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[3, 2, 1]");
}