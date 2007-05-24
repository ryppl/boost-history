// Boost.Print library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <string>
#include <sstream>
#include <boost/any.hpp>
#include "../../../boost/explore/explore.hpp"
#include "../../../boost/explore/stream_container.hpp"

BOOST_AUTO_TEST_CASE( basic_any_print_test )
{
    std::stringstream str_out;

    boost::any anyVal;
    explore::print(anyVal, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "");

    str_out.str("");

    anyVal = 1;
    explore::print(anyVal, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "1");

    str_out.str("");

    anyVal = std::string("some string");
    explore::print(anyVal, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "some string");

    str_out.str("");

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);
    anyVal = vi;
    explore::print(anyVal, str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[1:first, 2:second, 3:third]");
}

BOOST_AUTO_TEST_CASE( basic_any_stream_test )
{
    using namespace boost;
    std::stringstream str_out;

    boost::any anyVal;
    str_out << anyVal;
    BOOST_CHECK_EQUAL(str_out.str(), "");

    str_out.str("");

    anyVal = 1;
    str_out << anyVal;
    BOOST_CHECK_EQUAL(str_out.str(), "1");

    str_out.str("");

    anyVal = std::string("some string");
    str_out << anyVal;
    BOOST_CHECK_EQUAL(str_out.str(), "some string");

    str_out.str("");

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);
    anyVal = vi;
    str_out << anyVal;
    BOOST_CHECK_EQUAL(str_out.str(), "[1:first, 2:second, 3:third]");
}
