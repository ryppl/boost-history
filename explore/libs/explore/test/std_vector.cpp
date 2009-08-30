// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <boost/explore/iterator_range.hpp>
#include <boost/explore/vector.hpp>
#include "boost_explore_test_tools.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE( basic_vector_stream_test, C, test_types )
{
    test_traits<C>::stream_type str_out;

    std::vector<int> vi;
    str_out << vi;
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    vi.push_back(1);
    str_out << vi;
    BOOST_CHECK_EQUAL(output(str_out), "[1]");

    reset(str_out);

    vi.push_back(2);
    vi.push_back(3);
    str_out << vi;
    BOOST_CHECK_EQUAL(output(str_out), "[1, 2, 3]");

    reset(str_out);

    str_out << boost::explore::make_iterator_range(vi.begin(), ++(++vi.begin()));
    BOOST_CHECK_EQUAL(output(str_out), "[1, 2]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( vector_in_vector_stream_test, C, test_types )
{
    test_traits<C>::stream_type str_out;

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);

    std::vector<std::vector<int> > vvi;
    str_out << vvi;
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    vvi.push_back(vi);
    str_out << vvi;
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3]]");

    reset(str_out);

    vvi.push_back(vi);
    vvi.push_back(vi);
    str_out << vvi;
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");

    reset(str_out);

    str_out << boost::explore::make_iterator_range(vvi.begin(), ++(++vvi.begin()));
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3], [1, 2, 3]]");
}

BOOST_AUTO_TEST_CASE( vector_with_ugly_string_case_stream_test )
{
    test_traits<char>::stream_type str_out;

    std::vector<std::string> vs;
    vs.push_back("[1, 2, 3], [1, 2, 3], [1, 2, 3]");
    str_out << vs;
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");
}

BOOST_AUTO_TEST_CASE( vector_with_ugly_wstring_case_stream_test )
{
    test_traits<wchar_t>::stream_type str_out;

    std::vector<std::wstring> vs;
    vs.push_back(L"[1, 2, 3], [1, 2, 3], [1, 2, 3]");
    str_out << vs;
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");
}
