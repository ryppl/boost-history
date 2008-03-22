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
#include <iomanip>
#include <boost/lexical_cast.hpp>
#include <boost/explore.hpp>

BOOST_AUTO_TEST_CASE( int_print_test )
{
    std::stringstream str_out;
    int i = 123;

    explore::print(i, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "123");

    str_out.str("");
    
    int* pi = NULL;
    explore::print(pi, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "null");
    
    str_out.str("");
    
    //pi = &i;
    //explore::print(pi, str_out);
    // technically, we should force this to be true, though
    // it is highly unlikely to be false. Otherwise this test is only
    // telling you whether the following is even valid.
    //BOOST_CHECK(0 != boost::lexical_cast<std::string>((long)pi).compare("123"));

    // this is not a useful test.  pointers should print out as pointers.
    //BOOST_CHECK_EQUAL(str_out.str(), "123");
}

BOOST_AUTO_TEST_CASE( float_print_test )
{
    std::stringstream str_out;
    float f = 1.234f;

    explore::print(f, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "1.234");

    // are these needed?
    //BOOST_CHECK_MESSAGE(false, "Test min precision");
    //BOOST_CHECK_MESSAGE(false, "Test max precision");
}

BOOST_AUTO_TEST_CASE( double_print_test )
{
    std::stringstream str_out;
    double d = 1.2341234f;

    str_out << std::setprecision(7);
    explore::print(d, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "1.234123");

    // are these needed?
    //BOOST_CHECK_MESSAGE(false, "Test min precision");
    //BOOST_CHECK_MESSAGE(false, "Test max precision");
}

BOOST_AUTO_TEST_CASE( bool_print_test )
{
    std::stringstream str_out;
    bool b  = true;

    str_out << std::boolalpha;
    explore::print(b, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "true");

    str_out.str("");
    b  = false;

    explore::print(b, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "false");
}

BOOST_AUTO_TEST_CASE( char_print_test )
{
    std::stringstream str_out;
    char c = 'c';

    explore::print(c, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "c");

    str_out.str("");

    char* pc = NULL;
    explore::print(pc, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "null");
    
    str_out.str("");
    
    pc = "c";
    explore::print(pc, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "c");
    
    str_out.str("");
    
    pc = "cat";
    explore::print(pc, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "cat");
}

BOOST_AUTO_TEST_CASE( string_print_test )
{
    std::stringstream str_out;
    std::string s = "some string";

    explore::print(s, str_out);

    BOOST_CHECK_EQUAL(str_out.str(), "some string");
}