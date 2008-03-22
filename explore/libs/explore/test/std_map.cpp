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
#include <vector>
#include <map>
#include <boost/explore.hpp>

BOOST_AUTO_TEST_CASE( basic_map_stream_test )
{
    std::stringstream str_out;

    std::map<int,std::string> mis;
    str_out << mis;
    BOOST_CHECK_EQUAL(str_out.str(), "[]");

    str_out.str("");

    mis.insert(std::make_pair(1, "first"));
    str_out << mis;
    BOOST_CHECK_EQUAL(str_out.str(), "[1:first]");

    str_out.str("");

    mis.insert(std::make_pair(2, "second"));
    mis.insert(std::make_pair(3, "third"));
    str_out << mis;
    BOOST_CHECK_EQUAL(str_out.str(), "[1:first, 2:second, 3:third]");

    str_out.str("");

    str_out << explore::make_iterator_range(mis.begin(), ++(++mis.begin()));
    BOOST_CHECK_EQUAL(str_out.str(), "[1:first, 2:second]");
}

BOOST_AUTO_TEST_CASE( basic_multimap_stream_test )
{
    std::stringstream str_out;

    std::multimap<int,std::string> mmis;
    str_out << mmis;;
    BOOST_CHECK_EQUAL(str_out.str(), "[]");

    str_out.str("");

    mmis.insert(std::make_pair(1, "first"));
    str_out << mmis;
    BOOST_CHECK_EQUAL(str_out.str(), "[1:first]");

    str_out.str("");

    mmis.insert(std::make_pair(1, "single"));
    str_out << mmis;
    BOOST_CHECK_EQUAL(str_out.str(), "[1:first, 1:single]");

    str_out.str("");

    mmis.insert(std::make_pair(2, "second"));
    mmis.insert(std::make_pair(3, "third"));
    mmis.insert(std::make_pair(3, "triple"));
    str_out << mmis;
    BOOST_CHECK_EQUAL(str_out.str(), "[1:first, 1:single, 2:second, 3:third, 3:triple]");

    str_out.str("");

    str_out << explore::make_iterator_range(mmis.begin(), ++(++mmis.begin()));
    BOOST_CHECK_EQUAL(str_out.str(), "[1:first, 1:single]");
}

BOOST_AUTO_TEST_CASE( vector_in_map_stream_test )
{
    std::stringstream str_out;

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);

    std::map<int,std::vector<int> > mivi;
    str_out << mivi;
    BOOST_CHECK_EQUAL(str_out.str(), "[]");

    str_out.str("");

    mivi.insert(std::make_pair(1, vi));
    str_out << mivi;
    BOOST_CHECK_EQUAL(str_out.str(), "[1:[1, 2, 3]]");

    str_out.str("");

    mivi.insert(std::make_pair(2, vi));
    mivi.insert(std::make_pair(3, vi));
    str_out << mivi;
    BOOST_CHECK_EQUAL(str_out.str(), "[1:[1, 2, 3], 2:[1, 2, 3], 3:[1, 2, 3]]");

    str_out.str("");

    str_out << explore::make_iterator_range(mivi.begin(), ++(++mivi.begin()));
    BOOST_CHECK_EQUAL(str_out.str(), "[1:[1, 2, 3], 2:[1, 2, 3]]");
}
