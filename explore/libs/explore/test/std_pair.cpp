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
#include <boost/explore.hpp>

BOOST_AUTO_TEST_CASE( basic_pair_stream_test )
{
    std::stringstream str_out;

    std::pair<int,int> pi = std::make_pair(1,2);
    str_out << pi;
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2]");
}

BOOST_AUTO_TEST_CASE( pair_in_vector_stream_test )
{
    std::stringstream str_out;

    std::vector<std::pair<int,int> > vpi;
    str_out << vpi;
    BOOST_CHECK_EQUAL(str_out.str(), "[]");

    str_out.str("");

    std::pair<int,int> pi = std::make_pair(1,2);

    vpi.push_back(pi);
    str_out << vpi;
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2]]");

    str_out.str("");

    vpi.push_back(pi);
    vpi.push_back(pi);
    str_out << vpi;
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2], [1, 2], [1, 2]]");

    str_out.str("");

    str_out << explore::make_iterator_range(vpi.begin(), ++(++vpi.begin()));
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2], [1, 2]]");
}
