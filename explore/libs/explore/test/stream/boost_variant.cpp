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
#include <boost/variant.hpp>
#include <boost/explore.hpp>

BOOST_AUTO_TEST_CASE( basic_variant_stream_test )
{
    std::stringstream str_out;

    boost::variant< int, std::string, std::vector<int> > varVal;

    varVal = 1;
    str_out << varVal;
    BOOST_CHECK_EQUAL(str_out.str(), "1");

    str_out.str("");

    varVal = std::string("some string");
    str_out << varVal;
    BOOST_CHECK_EQUAL(str_out.str(), "some string");

    str_out.str("");

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);
    varVal = vi;
    str_out << varVal;
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2, 3]");
}
