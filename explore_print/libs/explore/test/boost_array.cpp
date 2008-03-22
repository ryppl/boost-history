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
#include <boost/array.hpp>
#include <boost/explore.hpp>

BOOST_AUTO_TEST_CASE( basic_array_print_test )
{
    std::stringstream str_out;

    boost::array<int,0> ai0;
    explore::print(ai0, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[]");

    str_out.str("");

    boost::array<int,1> ai1 = {1};
    explore::print(ai1, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[1]");

    str_out.str("");

    boost::array<int,3> ai3 = {1, 2, 3};
    explore::print(ai3, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2, 3]");

    str_out.str("");

    boost::array<int,3>::iterator itr = ai3.begin() + 2;

    explore::print(ai3.begin(), itr, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2]");

    str_out.str("");

    explore::print(boost::make_iterator_range(ai3.begin(), itr), str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2]");
}

BOOST_AUTO_TEST_CASE( vector_in_array_print_test )
{
    std::stringstream str_out;

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);

    boost::array<std::vector<int>,0> av0;
    explore::print(av0, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[]");

    str_out.str("");

    boost::array<std::vector<int>,1> av1 = {vi};
    explore::print(av1, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3]]");

    str_out.str("");

    boost::array<std::vector<int>,3> av3 = {vi, vi, vi};
    explore::print(av3, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");

    str_out.str("");

    boost::array<std::vector<int>,3>::iterator itr = av3.begin() + 2;

    explore::print(av3.begin(), itr, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3], [1, 2, 3]]");

    str_out.str("");

    explore::print(boost::make_iterator_range(av3.begin(), itr), str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3], [1, 2, 3]]");
}