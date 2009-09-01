// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <iomanip>
#include <boost/lexical_cast.hpp>
#include <boost/explore.hpp>
#include "boost_explore_test_tools.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE( int_stream_test, C, test_types )
{
    typename test_traits<C>::stream_type str_out;
    int i = 123;

    str_out << i;
    BOOST_CHECK_EQUAL(output(str_out), "123");

    reset(str_out);
    int* pi = &i;

    str_out << pi;
    // technically, we should force this to be true, though
    // it is highly unlikely to be false. Otherwise this test is only
    // telling you whether the following is even valid.
    //BOOST_CHECK(0 != boost::lexical_cast<std::string>((long)pi).compare("123"));
    //BOOST_CHECK_EQUAL(output(str_out), "123");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( float_stream_test, C, test_types )
{
    typename test_traits<C>::stream_type str_out;
    float f = 1.234f;

    str_out << std::setprecision(4) << f;
    BOOST_CHECK_EQUAL(output(str_out), "1.234");

    //BOOST_CHECK_MESSAGE(false, "Test min precision");
    //BOOST_CHECK_MESSAGE(false, "Test max precision");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( double_stream_test, C, test_types )
{
    typename test_traits<C>::stream_type str_out;
    double d = 1.2341234f;

    str_out << std::setprecision(7) << d;
    BOOST_CHECK_EQUAL(output(str_out), "1.234123");

    //BOOST_CHECK_MESSAGE(false, "Test min precision");
    //BOOST_CHECK_MESSAGE(false, "Test max precision");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( bool_stream_test, C, test_types )
{
    typename test_traits<C>::stream_type str_out;
    bool b  = true;

    str_out << std::boolalpha << b;
    BOOST_CHECK_EQUAL(output(str_out), "true");

    reset(str_out);
    b  = false;

    str_out << std::boolalpha << b;
    BOOST_CHECK_EQUAL(output(str_out), "false");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( char_stream_test, C, test_types )
{
    typename test_traits<C>::stream_type str_out;
    char c = 'c';

    str_out << c;
    BOOST_CHECK_EQUAL(output(str_out), "c");

    reset(str_out);
    char* pc = "c";

    str_out << pc;
    BOOST_CHECK_EQUAL(output(str_out), "c");
}

BOOST_AUTO_TEST_CASE( string_stream_test )
{
    test_traits<char>::stream_type str_out;
    std::string s = "some string";

    str_out << s;

    BOOST_CHECK_EQUAL(output(str_out), "some string");
}

BOOST_AUTO_TEST_CASE( wstring_stream_test )
{
    test_traits<wchar_t>::stream_type str_out;
    std::wstring s = L"some string";

    str_out << s;

    BOOST_CHECK_EQUAL(output(str_out), "some string");
}
