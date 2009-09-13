// Boost.Explore library

// Copyright Jeffrey Faust 2009. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <boost/explore.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include "boost_explore_test_tools.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE( strings_in_map, C, test_types )
{
    using namespace boost::explore;
    typename test_traits<C>::stream_type str_out;

    std::map<int, typename test_traits<C>::string_type> mis;
    str_out << mis;
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    mis[1] = str_to<C>("first");
    mis[2] = str_to<C>("second");
    str_out << mis;
    BOOST_CHECK_EQUAL(output(str_out), "[1:first, 2:second]");

    reset(str_out);

    str_out << quote_strings << mis;
    BOOST_CHECK_EQUAL(output(str_out), "[1:\"first\", 2:\"second\"]");
    str_out << no_quote_strings << mis;
    BOOST_CHECK_EQUAL(output(str_out), "[1:\"first\", 2:\"second\"][1:first, 2:second]");

    reset(str_out);

    str_out << item_width(12) << mis;
    BOOST_CHECK_EQUAL(output(str_out), "[     1:first,     2:second]");

    reset(str_out);
    str_out << item_width(12) << quote_strings << mis;
    BOOST_CHECK_EQUAL(output(str_out), "[   1:\"first\",   2:\"second\"]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( strings_in_vector, C, test_types )
{
    using namespace boost::explore;
    typedef typename test_traits<C>::string_type string_type;

    typename test_traits<C>::stream_type str_out;

    std::vector<string_type> vs;
    vs.push_back(str_to<C>("1234"));
    vs.push_back(str_to<C>("5678"));
    str_out << quote_strings << item_width(7) << vs;

    BOOST_CHECK_EQUAL(output(str_out), "[ \"1234\",  \"5678\"]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( strings_in_boost_array, C, test_types )
{
    using namespace boost::explore;
    typedef typename test_traits<C>::string_type string_type;
    typename test_traits<C>::stream_type str_out;

    boost::array<string_type, 2> as;
    as[0] = str_to<C>("first");
    as[1] = str_to<C>("second");

    str_out << quote_strings << item_width(10) << as;
    BOOST_CHECK_EQUAL(output(str_out), "[   \"first\",   \"second\"]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( strings_in_boost_range, C, test_types )
{
    using namespace boost::explore;
    typedef typename test_traits<C>::string_type string_type;
    typename test_traits<C>::stream_type str_out;

    std::vector<string_type> vs;

    vs.push_back(str_to<C>("Hello"));
    vs.push_back(str_to<C>("World"));

    str_out << quote_strings << item_width(17) << make_iterator_range(vs);
    BOOST_CHECK_EQUAL(output(str_out), "[          \"Hello\",           \"World\"]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( strings_in_tuple, C, test_types )
{
    using namespace boost::explore;
    typedef typename test_traits<C>::string_type string_type;
    typename test_traits<C>::stream_type str_out;

    boost::tuples::tuple<string_type, double> t(str_to<C>("Hello"), 3.14);
    str_out << quote_strings << item_width(20) << t;
    // I think things that already have a way to print should not change
    //BOOST_CHECK_EQUAL(output(str_out), "[1, 3.14]");
    BOOST_CHECK_EQUAL(output(str_out), "(Hello 3.14)");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( strings_in_c_array, C, test_types )
{
    using namespace boost::explore;
    typedef typename test_traits<C>::string_type string_type;
    typename test_traits<C>::stream_type str_out;

    string_type arrs3[] = {str_to<C>("One"), str_to<C>("Two"), str_to<C>("Three")};
    str_out << quote_strings << item_width(10) << arrs3;
    BOOST_CHECK_EQUAL(output(str_out), "[     \"One\",      \"Two\",    \"Three\"]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( strings_in_deque, C, test_types )
{
    using namespace boost::explore;
    typedef typename test_traits<C>::string_type string_type;

    typename test_traits<C>::stream_type str_out;

    std::deque<string_type> ds;
    ds.push_back(str_to<C>("1234"));
    ds.push_back(str_to<C>("5678"));
    str_out << quote_strings << item_width(7) << ds;

    BOOST_CHECK_EQUAL(output(str_out), "[ \"1234\",  \"5678\"]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( strings_in_list, C, test_types )
{
    using namespace boost::explore;
    typedef typename test_traits<C>::string_type string_type;

    typename test_traits<C>::stream_type str_out;

    std::list<string_type> ls;
    ls.push_back(str_to<C>("1234"));
    ls.push_back(str_to<C>("5678"));
    str_out << quote_strings << item_width(7) << ls;

    BOOST_CHECK_EQUAL(output(str_out), "[ \"1234\",  \"5678\"]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( strings_in_pair, C, test_types )
{
    using namespace boost::explore;
    typedef typename test_traits<C>::string_type string_type;
    typename test_traits<C>::stream_type str_out;

    std::pair<string_type, double> p(str_to<C>("Hello"), 3.14);
    str_out << quote_strings << item_width(10) << p;
    BOOST_CHECK_EQUAL(output(str_out), "[   \"Hello\",       3.14]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( strings_in_set, C, test_types )
{
    using namespace boost::explore;
    typedef typename test_traits<C>::string_type string_type;
    typename test_traits<C>::stream_type str_out;

    std::set<string_type> ss;
    ss.insert(str_to<C>("Hello"));
    ss.insert(str_to<C>("World"));

    str_out << quote_strings << item_width(10) << ss;
    BOOST_CHECK_EQUAL(output(str_out), "[   \"Hello\",    \"World\"]");
}
