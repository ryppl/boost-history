// Boost.Explore library

// Copyright Jeffrey Faust 2009. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <boost/explore/map.hpp>
#include <boost/explore/vector.hpp>
#include "boost_explore_test_tools.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE( strings_in_map, C, test_types )
{
    using namespace boost::explore;
    typename test_traits<C>::stream_type str_out;

    std::map<int, typename test_traits<C>::string_type> mis;
    str_out << mis;
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    mis[1] = str_to<C>("first");
    mis[2] = str_to<C>("second");
    str_out << mis;
    BOOST_CHECK_EQUAL(output(str_out), "[1:first, 2:second]");

    reset(str_out);

    str_out << quote_strings << mis;
    BOOST_CHECK_EQUAL(output(str_out), "[1:\"first\", 2:\"second\"]");
    str_out << no_quote_strings << mis;
    BOOST_CHECK_EQUAL(output(str_out), "[1:\"first\", 2:\"second\"][1:first, 2:second]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( strings_with_item_width, C, test_types )
{
    using namespace boost::explore;
    typedef typename test_traits<C>::string_type string_type;

    typename test_traits<C>::stream_type str_out;

    std::vector<string_type> vs;
    vs.push_back(str_to<C>("1234"));
    vs.push_back(str_to<C>("5678"));
    str_out << quote_strings << item_width(7) << vs;

    BOOST_CHECK_EQUAL(output(str_out), "[ \"1234\",  \"5678\"]");
}
