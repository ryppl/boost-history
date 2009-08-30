// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <vector>
#include <boost/explore/map.hpp>
#include <boost/explore/vector.hpp>
#include <boost/explore/iterator_range.hpp>
#include "boost_explore_test_tools.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE( basic_map_stream_test, C, test_types )
{
    test_traits<C>::stream_type str_out;

    std::map<int, test_traits<C>::string_type> mis;
    str_out << mis;
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    mis.insert(std::make_pair(1, str_to<C>("first")));
    str_out << mis;
    BOOST_CHECK_EQUAL(output(str_out), "[1:first]");

    reset(str_out);

    mis.insert(std::make_pair(2, str_to<C>("second")));
    mis.insert(std::make_pair(3, str_to<C>("third")));
    str_out << mis;
    BOOST_CHECK_EQUAL(output(str_out), "[1:first, 2:second, 3:third]");

    reset(str_out);

    str_out << boost::explore::make_iterator_range(mis.begin(), ++(++mis.begin()));
    BOOST_CHECK_EQUAL(output(str_out), "[1:first, 2:second]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( basic_multimap_stream_test, C, test_types )
{
    test_traits<C>::stream_type str_out;

    std::multimap<int, test_traits<C>::string_type> mmis;
    str_out << mmis;;
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    mmis.insert(std::make_pair(1, str_to<C>("first")));
    str_out << mmis;
    BOOST_CHECK_EQUAL(output(str_out), "[1:first]");

    reset(str_out);

    mmis.insert(std::make_pair(1, str_to<C>("single")));
    str_out << mmis;
    BOOST_CHECK_EQUAL(output(str_out), "[1:first, 1:single]");

    reset(str_out);

    mmis.insert(std::make_pair(2, str_to<C>("second")));
    mmis.insert(std::make_pair(3, str_to<C>("third")));
    mmis.insert(std::make_pair(3, str_to<C>("triple")));
    str_out << mmis;
    BOOST_CHECK_EQUAL(output(str_out), "[1:first, 1:single, 2:second, 3:third, 3:triple]");

    reset(str_out);

    str_out << boost::explore::make_iterator_range(mmis.begin(), ++(++mmis.begin()));
    BOOST_CHECK_EQUAL(output(str_out), "[1:first, 1:single]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( vector_in_map_stream_test, C, test_types )
{
    test_traits<C>::stream_type str_out;

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);

    std::map<int,std::vector<int> > mivi;
    str_out << mivi;
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    mivi.insert(std::make_pair(1, vi));
    str_out << mivi;
    BOOST_CHECK_EQUAL(output(str_out), "[1:[1, 2, 3]]");

    reset(str_out);

    mivi.insert(std::make_pair(2, vi));
    mivi.insert(std::make_pair(3, vi));
    str_out << mivi;
    BOOST_CHECK_EQUAL(output(str_out), "[1:[1, 2, 3], 2:[1, 2, 3], 3:[1, 2, 3]]");

    reset(str_out);

    str_out << boost::explore::make_iterator_range(mivi.begin(), ++(++mivi.begin()));
    BOOST_CHECK_EQUAL(output(str_out), "[1:[1, 2, 3], 2:[1, 2, 3]]");
}
