// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <boost/range/iterator_range.hpp>
#include <string>
#include <sstream>
#include <vector>
#include <boost/explore.hpp>

BOOST_AUTO_TEST_CASE( vector_custom_format_print_test )
{
    std::stringstream str_out;

    std::vector<int> vi;
    explore::print(vi, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[]");

    str_out.str("");

    vi.push_back(1);
    explore::print(vi, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[1]");

    str_out.str("");

    vi.push_back(2);
    vi.push_back(3);
    explore::print(vi, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2, 3]");

    str_out.str("");

    explore::print(vi.begin(), ++(++vi.begin()), str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2]");

    str_out.str("");

    explore::print(boost::make_iterator_range(vi.begin(), ++(++vi.begin())), str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2]");

    str_out.str("");

    explore::print(vi, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2, 3]");
}

BOOST_AUTO_TEST_CASE( vector_in_vector_print_test )
{
    std::stringstream str_out;

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);

    std::vector<std::vector<int> > vvi;
    explore::print(vvi, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[]");

    str_out.str("");

    vvi.push_back(vi);
    explore::print(vvi, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3]]");

    str_out.str("");

    vvi.push_back(vi);
    vvi.push_back(vi);
    explore::print(vvi, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");

    str_out.str("");

    explore::print(vvi.begin(), ++(++vvi.begin()), str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3], [1, 2, 3]]");

    str_out.str("");

    explore::print(boost::make_iterator_range(vvi.begin(), ++(++vvi.begin())), str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3], [1, 2, 3]]");
}

// This test shows that you can use a string to cause the same outputs
// as other print statements.
BOOST_AUTO_TEST_CASE( vector_with_ugly_string_case_print_test )
{
    std::stringstream str_out;

    std::vector<std::string> vs;
    vs.push_back("[1, 2, 3], [1, 2, 3], [1, 2, 3]");
    explore::print(vs, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");
}