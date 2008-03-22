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
#include <vector>
#include <boost/explore.hpp>

BOOST_AUTO_TEST_CASE( basic_c_array_print_test )
{
    std::stringstream str_out;

    int arri1[] = {1};
    explore::print(arri1, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[1]");

    str_out.str("");

    int arri3[] = {1,2,3};
    explore::print(arri3, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2, 3]");
}

BOOST_AUTO_TEST_CASE( vector_in_c_array_print_test )
{
    std::stringstream str_out;

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);

    std::vector<int> arrvi1[] = {vi};
    explore::print(arrvi1, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3]]");

    str_out.str("");

    std::vector<int> arrvi3[] = {vi,vi,vi};
    explore::print(arrvi3, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");
}