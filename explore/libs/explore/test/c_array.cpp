// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <vector>
#include <boost/explore.hpp>
#include "boost_explore_test_tools.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE( basic_c_array_stream_test, C, test_types )
{
    using namespace boost::explore;
    test_traits<C>::stream_type str_out;

    int arri1[] = {1};
    str_out << arri1;
    BOOST_CHECK_EQUAL(output(str_out), "[1]");

    reset(str_out);

    int arri3[] = {1,2,3};
    str_out << arri3;
    BOOST_CHECK_EQUAL(output(str_out), "[1, 2, 3]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( vector_in_c_array_stream_test, C, test_types )
{
    using namespace boost::explore;
    test_traits<C>::stream_type str_out;

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);

    std::vector<int> arrvi1[] = {vi};
    str_out << arrvi1;
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3]]");

    reset(str_out);

    std::vector<int> arrvi3[] = {vi,vi,vi};
    str_out << arrvi3;
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");
}

BOOST_AUTO_TEST_CASE( c_style_string_stream_test )
{
    using namespace boost::explore;
    test_traits<char>::stream_type str_out;
    
    const char s[4] = "abc";
    str_out << s;
    BOOST_CHECK_EQUAL(output(str_out), "abc");
    
    reset(str_out);
     
    str_out << "def";
    BOOST_CHECK_EQUAL(output(str_out), "def");
}

BOOST_AUTO_TEST_CASE( c_style_wstring_stream_test )
{
    using namespace boost::explore;
    test_traits<wchar_t>::stream_type str_out;
    
    std::wstring s = L"abc";
    str_out << s;
    BOOST_CHECK_EQUAL(output(str_out), "abc");
    
    reset(str_out);
     
    str_out << std::wstring(L"def");
    BOOST_CHECK_EQUAL(output(str_out), "def");
}