// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <vector>
#include <boost/explore/set.hpp>
#include <boost/explore/vector.hpp>
#include <boost/explore/iterator_range.hpp>
#include <boost/explore.hpp>
#include "boost_explore_test_tools.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE( basic_set_stream_test, C, test_types )
{
    typename test_traits<C>::stream_type str_out;

    std::set<int> si;
    str_out << si;
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    si.insert(1);
    str_out << si;
    BOOST_CHECK_EQUAL(output(str_out), "[1]");

    reset(str_out);

    si.insert(2);
    si.insert(3);
    str_out << si;
    BOOST_CHECK_EQUAL(output(str_out), "[1, 2, 3]");

    reset(str_out);

    str_out << boost::explore::make_iterator_range(si.begin(), ++(++si.begin()));
    BOOST_CHECK_EQUAL(output(str_out), "[1, 2]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( basic_multiset_stream_test, C, test_types )
{
    typename test_traits<C>::stream_type str_out;

    std::multiset<int> msi;
    str_out << msi;
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    msi.insert(1);
    str_out << msi;
    BOOST_CHECK_EQUAL(output(str_out), "[1]");

    reset(str_out);

    msi.insert(1);
    str_out << msi;
    BOOST_CHECK_EQUAL(output(str_out), "[1, 1]");

    reset(str_out);

    msi.insert(2);
    msi.insert(3);
    msi.insert(3);
    str_out << msi;
    BOOST_CHECK_EQUAL(output(str_out), "[1, 1, 2, 3, 3]");

    reset(str_out);

    str_out << boost::explore::make_iterator_range(msi.begin(), ++(++msi.begin()));
    BOOST_CHECK_EQUAL(output(str_out), "[1, 1]");	
}

BOOST_AUTO_TEST_CASE_TEMPLATE( vector_in_set_stream_test, C, test_types )
{
    typename test_traits<C>::stream_type str_out;

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);

    std::multiset<std::vector<int> > svi;
    str_out << svi;
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    svi.insert(vi);
    str_out << svi;
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3]]");

    reset(str_out);

    svi.insert(vi);
    svi.insert(vi);
    str_out << svi;
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");

    reset(str_out);

    str_out << boost::explore::make_iterator_range(svi.begin(), ++(++svi.begin()));
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3], [1, 2, 3]]");
}
