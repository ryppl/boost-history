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
#include <set>
#include <boost/explore.hpp>

BOOST_AUTO_TEST_CASE( basic_set_stream_test )
{
    std::stringstream str_out;

    std::set<int> si;
    str_out << si;
    BOOST_CHECK_EQUAL(str_out.str(), "[]");

    str_out.str("");

    si.insert(1);
    str_out << si;
    BOOST_CHECK_EQUAL(str_out.str(), "[1]");

    str_out.str("");

    si.insert(2);
    si.insert(3);
    str_out << si;
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2, 3]");

    str_out.str("");

    str_out << explore::make_iterator_range(si.begin(), ++(++si.begin()));
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2]");
}

BOOST_AUTO_TEST_CASE( basic_multiset_stream_test )
{
    std::stringstream str_out;

    std::multiset<int> msi;
    str_out << msi;
    BOOST_CHECK_EQUAL(str_out.str(), "[]");

    str_out.str("");

    msi.insert(1);
    str_out << msi;
    BOOST_CHECK_EQUAL(str_out.str(), "[1]");

    str_out.str("");

    msi.insert(1);
    str_out << msi;
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 1]");

    str_out.str("");

    msi.insert(2);
    msi.insert(3);
    msi.insert(3);
    str_out << msi;
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 1, 2, 3, 3]");

    str_out.str("");

    str_out << explore::make_iterator_range(msi.begin(), ++(++msi.begin()));
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 1]");	
}

BOOST_AUTO_TEST_CASE( vector_in_set_stream_test )
{
    std::stringstream str_out;

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);

    std::multiset<std::vector<int> > svi;
    str_out << svi;
    BOOST_CHECK_EQUAL(str_out.str(), "[]");

    str_out.str("");

    svi.insert(vi);
    str_out << svi;
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3]]");

    str_out.str("");

    svi.insert(vi);
    svi.insert(vi);
    str_out << svi;
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");

    str_out.str("");

    str_out << explore::make_iterator_range(svi.begin(), ++(++svi.begin()));
    BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3], [1, 2, 3]]");
}
