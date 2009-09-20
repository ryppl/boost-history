// Boost.Explore library

// Copyright Jeffrey Faust 2009. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Tests explore against other standard stream functionality

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <boost/explore/vector.hpp>
#include "boost_explore_test_tools.hpp"

#include <iomanip>

BOOST_AUTO_TEST_CASE_TEMPLATE( std_fill, C, test_types )
{
    using namespace boost::explore;
    typename test_traits<C>::stream_type str_out;

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(100);
    vi.push_back(1000);

    str_out << std::setfill(C('x')) << vi;
    BOOST_CHECK_EQUAL(output(str_out), "[1, 100, 1000]");

    reset(str_out);

    str_out << std::setfill(C('x')) << item_width(5) << vi;
    BOOST_CHECK_EQUAL(output(str_out), "[xxxx1, xx100, x1000]");

    reset(str_out);

    str_out << std::setfill(C('x')) << item_width(5) << separator(str_to<C>(",")) << vi;
    BOOST_CHECK_EQUAL(output(str_out), "[xxxx1,xx100,x1000]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( std_boolalpha, C, test_types )
{
    using namespace boost::explore;
    typename test_traits<C>::stream_type str_out;

    std::vector<bool> vb;
    vb.push_back(true);
    vb.push_back(false);
    vb.push_back(true);

    str_out << std::boolalpha << vb;
    BOOST_CHECK_EQUAL(output(str_out), "[true, false, true]");

    reset(str_out);

    str_out << std::noboolalpha << vb;
    BOOST_CHECK_EQUAL(output(str_out), "[1, 0, 1]");

    reset(str_out);

    str_out << std::boolalpha << item_width(7) << separator(str_to<C>(",")) << vb;
    BOOST_CHECK_EQUAL(output(str_out), "[   true,  false,   true]");

    reset(str_out);

    str_out << std::noboolalpha << item_width(7) << separator(str_to<C>(",")) << vb;
    BOOST_CHECK_EQUAL(output(str_out), "[      1,      0,      1]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( std_copy_fmt, C, test_types )
{
    using namespace boost::explore;
    typename test_traits<C>::stream_type str_out1;
    typename test_traits<C>::stream_type str_out2;

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(100);
    vi.push_back(1000);

    str_out1 << item_width(10) << delimiters(str_to<C>("<"), str_to<C>("-"), str_to<C>(">"));
    str_out2.copyfmt(str_out1);

    str_out1 << vi;
    str_out2 << vi;

    BOOST_CHECK_EQUAL(output(str_out1), "<         1-       100-      1000>");
    BOOST_CHECK_EQUAL(output(str_out2), "<         1-       100-      1000>");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( std_imbue, C, test_types )
{
    using namespace boost::explore;
    typename test_traits<C>::stream_type str_out;

    std::vector<double> vd;
    vd.push_back(1.2);
    vd.push_back(2.3);
    vd.push_back(3.4);

    str_out << vd;
    BOOST_CHECK_EQUAL(output(str_out), "[1.2, 2.3, 3.4]");

    reset(str_out);

    str_out.imbue(std::locale("german_Germany"));
    str_out << vd;
    BOOST_CHECK_EQUAL(output(str_out), "[1,2, 2,3, 3,4]");
}
