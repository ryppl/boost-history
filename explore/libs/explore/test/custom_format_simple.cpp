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
#include <vector>
#include "boost_explore_test_tools.hpp"

std::ostream& basic_stream_format(std::ostream& ostr)
{
    using namespace boost::explore;
    return ostr << start("<=") << separator("#") << end("=>")
        << assoc_start("<=") << assoc_separator("#") << assoc_end("=>");
}

std::wostream& basic_stream_format(std::wostream& ostr)
{
    using namespace boost::explore;
    return ostr << start(L"<=") << separator(L"#") << end(L"=>")
        << assoc_start(L"<=") << assoc_separator(L"#") << assoc_end(L"=>");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( basic_vector_custom_format_stream_test, C, test_types )
{
    typename test_traits<C>::stream_type str_out;
    str_out << basic_stream_format;

    std::vector<int> vi;
    str_out << vi;
    BOOST_CHECK_EQUAL(output(str_out), "<==>");

    reset(str_out);

    vi.push_back(1);
    str_out << vi;
    BOOST_CHECK_EQUAL(output(str_out), "<=1=>");

    reset(str_out);

    vi.push_back(2);
    vi.push_back(3);
    str_out << vi;
    BOOST_CHECK_EQUAL(output(str_out), "<=1#2#3=>");

    reset(str_out);

    str_out << boost::explore::make_iterator_range(vi.begin(), ++(++vi.begin()));
    BOOST_CHECK_EQUAL(output(str_out), "<=1#2=>");

    reset(str_out);

    str_out << vi;
    BOOST_CHECK_EQUAL(output(str_out), "<=1#2#3=>");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( basic_map_custom_format_stream_test, C, test_types )
{
    typedef typename test_traits<C>::string_type string_type;
    typename test_traits<C>::stream_type str_out;
    str_out << basic_stream_format;

    std::map<int, string_type> mis;
    str_out << mis;
    BOOST_CHECK_EQUAL(output(str_out), "<==>");

    reset(str_out);

    mis.insert(std::make_pair(1, str_to<C>("first")));
    str_out << mis;
    BOOST_CHECK_EQUAL(output(str_out), "<=<=1#first=>=>");

    reset(str_out);

    mis.insert(std::make_pair(2, str_to<C>("second")));
    mis.insert(std::make_pair(3, str_to<C>("third")));

    str_out << mis;
    BOOST_CHECK_EQUAL(output(str_out), "<=<=1#first=>#<=2#second=>#<=3#third=>=>");

    reset(str_out);

    str_out << boost::explore::make_iterator_range(mis.begin(), ++(++mis.begin()));
    BOOST_CHECK_EQUAL(output(str_out), "<=<=1#first=>#<=2#second=>=>");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( begin_end_helper_test, C, test_types )
{
    typename test_traits<C>::stream_type str_out;
    
    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);
    str_out << boost::explore::begin_end(str_to<C>("B "), str_to<C>(" E")) << vi;
    BOOST_CHECK_EQUAL(output(str_out), "B 1, 2, 3 E");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( delimeters_helper_test, C, test_types )
{
    typename test_traits<C>::stream_type str_out;
    
    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);
    str_out << boost::explore::delimiters(str_to<C>("F "), str_to<C>(" - "), str_to<C>(" L")) << vi;
    BOOST_CHECK_EQUAL(output(str_out), "F 1 - 2 - 3 L");
}
