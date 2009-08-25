// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/explore/manipulators.hpp>
#include <boost/explore/vector.hpp>
#include <boost/explore/map.hpp>
#include <boost/explore/iterator_range.hpp>
#include <string>
#include <sstream>
#include <vector>

std::ostream& basic_stream_format(std::ostream& ostr)
{
    using namespace boost::explore;
    return ostr << start("<=") << separator("#") << end("=>")
    << assoc_start("<=") << assoc_separator("#") << assoc_end("=>");
}

BOOST_AUTO_TEST_CASE( basic_vector_custom_format_stream_test )
{
    std::stringstream str_out;
    str_out << basic_stream_format;

    std::vector<int> vi;
    str_out << vi;
    BOOST_CHECK_EQUAL(str_out.str(), "<==>");

    str_out.str("");

    vi.push_back(1);
    str_out << vi;
    BOOST_CHECK_EQUAL(str_out.str(), "<=1=>");

    str_out.str("");

    vi.push_back(2);
    vi.push_back(3);
    str_out << vi;
    BOOST_CHECK_EQUAL(str_out.str(), "<=1#2#3=>");

    str_out.str("");

    str_out << boost::explore::make_iterator_range(vi.begin(), ++(++vi.begin()));
    BOOST_CHECK_EQUAL(str_out.str(), "<=1#2=>");

    str_out.str("");

    str_out << vi;
    BOOST_CHECK_EQUAL(str_out.str(), "<=1#2#3=>");
}

BOOST_AUTO_TEST_CASE( basic_map_custom_format_stream_test )
{
    std::stringstream str_out;
    str_out << basic_stream_format;

    std::map<int,std::string> mis;
    str_out << mis;
    BOOST_CHECK_EQUAL(str_out.str(), "<==>");

    str_out.str("");

    mis.insert(std::make_pair(1, "first"));
    str_out << mis;
    BOOST_CHECK_EQUAL(str_out.str(), "<=<=1#first=>=>");

    str_out.str("");

    mis.insert(std::make_pair(2, "second"));
    mis.insert(std::make_pair(3, "third"));

    str_out << mis;
    BOOST_CHECK_EQUAL(str_out.str(), "<=<=1#first=>#<=2#second=>#<=3#third=>=>");

    str_out.str("");

    str_out << boost::explore::make_iterator_range(mis.begin(), ++(++mis.begin()));
    BOOST_CHECK_EQUAL(str_out.str(), "<=<=1#first=>#<=2#second=>=>");
}

BOOST_AUTO_TEST_CASE( begin_end_helper_test )
{
    std::stringstream str_out;
    
    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);
    str_out << boost::explore::begin_end("B ", " E") << vi;
    BOOST_CHECK_EQUAL(str_out.str(), "B 1, 2, 3 E");
}

BOOST_AUTO_TEST_CASE( delimeters_helper_test )
{
    std::stringstream str_out;
    
    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);
    str_out << boost::explore::delimiters("F ", " - ", " L") << vi;
    BOOST_CHECK_EQUAL(str_out.str(), "F 1 - 2 - 3 L");
}
