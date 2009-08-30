// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <boost/variant.hpp>
#include <boost/explore.hpp>
#include "boost_explore_test_tools.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE( basic_variant_stream_test, C, test_types )
{
    typedef test_traits<C>::string_type string_type;
    test_traits<C>::stream_type str_out;

    boost::variant< int, string_type, std::vector<int> > varVal;

    varVal = 1;
    str_out << varVal;
    BOOST_CHECK_EQUAL(output(str_out), "1");

    reset(str_out);

    varVal = string_type(str_to<C>("some string"));
    str_out << varVal;
    BOOST_CHECK_EQUAL(output(str_out), "some string");

    reset(str_out);

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);
    varVal = vi;
    str_out << varVal;
    BOOST_CHECK_EQUAL(output(str_out), "[1, 2, 3]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( cool_variant_stream_test, C, test_types )
{
    using namespace boost::explore;
    typedef test_traits<C>::string_type string_type;
    test_traits<C>::stream_type str_out;

    std::vector<boost::variant< int, string_type, std::vector<int> > > varVec;
    string_type name(str_to<C>("Joe"));

    std::vector<int> favoriteNumbers;
    favoriteNumbers.push_back(22);
    favoriteNumbers.push_back(42);
    favoriteNumbers.push_back(73);

    varVec.push_back(str_to<C>("Hello, my name is "));
    varVec.push_back(name);
    varVec.push_back(str_to<C>(".  I am "));
    varVec.push_back(34);
    varVec.push_back(str_to<C>(" years old.  My favorite numbers are: "));
    varVec.push_back(favoriteNumbers);
    varVec.push_back(str_to<C>("."));

    str_out << level(0);
    str_out << start(str_to<C>("")) << boost::explore::end(str_to<C>("")) << separator(str_to<C>(""));

    // level 1 (for nested collection)
    str_out << level(1);
    str_out << start(str_to<C>("[")) << boost::explore::end(str_to<C>("]")) << separator(str_to<C>(", "));

    str_out << varVec;
    BOOST_CHECK_EQUAL(output(str_out), "Hello, my name is Joe.  I am 34 years old.  My favorite numbers are: [22, 42, 73].");
}
