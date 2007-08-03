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
#include "basic_lg_format.hpp"

BOOST_AUTO_TEST_CASE( basic_vector_custom_format_print_test )
{
    std::stringstream str_out;

    std::vector<int> vi;
    explore::print(vi, str_out, basic_lg_format(), explore::default_container_policy());
    BOOST_CHECK_EQUAL(str_out.str(), "<==>");

    str_out.str("");

    vi.push_back(1);
    explore::print(vi, str_out, basic_lg_format(), explore::default_container_policy());
    BOOST_CHECK_EQUAL(str_out.str(), "<=1=>");

    str_out.str("");

    vi.push_back(2);
    vi.push_back(3);
    explore::print(vi, str_out, basic_lg_format(), explore::default_container_policy());
    BOOST_CHECK_EQUAL(str_out.str(), "<=1#2#3=>");

    str_out.str("");

    explore::print(vi.begin(), ++(++vi.begin()), str_out, basic_lg_format(), explore::default_container_policy());
    BOOST_CHECK_EQUAL(str_out.str(), "<=1#2=>");

    str_out.str("");

    explore::print(boost::make_iterator_range(vi.begin(), ++(++vi.begin())), str_out, basic_lg_format(), explore::default_container_policy());
    BOOST_CHECK_EQUAL(str_out.str(), "<=1#2=>");

    str_out.str("");

    explore::print(vi, str_out, basic_lg_format(), explore::default_container_policy());
    BOOST_CHECK_EQUAL(str_out.str(), "<=1#2#3=>");
}

BOOST_AUTO_TEST_CASE( basic_map_custom_format_print_test )
{
    std::stringstream str_out;

    std::map<int,std::string> mis;
    explore::print(mis, str_out, basic_lg_format(), explore::default_container_policy());
    BOOST_CHECK_EQUAL(str_out.str(), "<==>");

    str_out.str("");

    mis.insert(std::make_pair(1, "first"));
    explore::print(mis, str_out, basic_lg_format(), explore::default_container_policy());
    BOOST_CHECK_EQUAL(str_out.str(), "<=<=1#first=>=>");

    str_out.str("");

    mis.insert(std::make_pair(2, "second"));
    mis.insert(std::make_pair(3, "third"));
    explore::print(mis, str_out, basic_lg_format(), explore::default_container_policy());
    BOOST_CHECK_EQUAL(str_out.str(), "<=<=1#first=>#<=2#second=>#<=3#third=>=>");

    str_out.str("");

    explore::print(mis.begin(), ++(++mis.begin()), str_out, basic_lg_format(), explore::default_container_policy());
    BOOST_CHECK_EQUAL(str_out.str(), "<=<=1#first=>#<=2#second=>=>");

    str_out.str("");

    explore::print(boost::make_iterator_range(mis.begin(), ++(++mis.begin())), str_out, basic_lg_format(), explore::default_container_policy());
    BOOST_CHECK_EQUAL(str_out.str(), "<=<=1#first=>#<=2#second=>=>");
}