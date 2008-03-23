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

BOOST_AUTO_TEST_CASE( cool_variant_stream_test )
{
   using namespace explore;
   std::stringstream str_out;

   std::vector<boost::variant< int, std::string, std::vector<int> > > varVec;
   std::string name("Joe");

   std::vector<int> favoriteNumbers;
   favoriteNumbers.push_back(22);
   favoriteNumbers.push_back(42);
   favoriteNumbers.push_back(73);
   
   varVec.push_back("Hello, my name is ");
   varVec.push_back(name);
   varVec.push_back(".  I am ");
   varVec.push_back(34);
   varVec.push_back(" years old.  My favorite numbers are: ");
   varVec.push_back(favoriteNumbers);
   varVec.push_back(".");
   
   // level 0
   str_out << start("") << explore::end("") << separator("");
   
   // level 1 (for nested collection)
   str_out << start("[", 1) << explore::end("]", 1) << separator(", ", 1);
   
   str_out << varVec;
   BOOST_CHECK_EQUAL(str_out.str(), "Hello, my name is Joe.  I am 34 years old.  My favorite numbers are: [22, 42, 73].");
}
