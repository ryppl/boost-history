// Boost.Print library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <sstream>
#include <vector>
#include <boost/range/iterator_range.hpp>
#include "../../../boost/explore/stream_container.hpp"

//what's up here?  I get a link error if I include explore.hpp
//#include "../../../boost/explore/explore.hpp"

BOOST_AUTO_TEST_CASE( boost_range_stream_test )
{
    using namespace explore;
    std::stringstream str_out;

    typedef std::vector<int> Cont;
    typedef boost::iterator_range<Cont::iterator> IterRange;
    typedef boost::iterator_range<Cont::reverse_iterator> RevIterRange;
    Cont vi;
    str_out << as_container(IterRange(vi));
    BOOST_CHECK_EQUAL(str_out.str(), "[]");

    str_out.str("");

    vi.push_back(1);
    str_out << as_container(IterRange(vi));
    BOOST_CHECK_EQUAL(str_out.str(), "[1]");

    str_out.str("");

    vi.push_back(2);
    vi.push_back(3);
    str_out << as_container(IterRange(vi));
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2, 3]");

    str_out.str("");
    str_out << as_container(IterRange(vi.begin()+2, vi.end()));
    BOOST_CHECK_EQUAL(str_out.str(), "[3]");

    str_out.str("");
    str_out << as_container(RevIterRange(vi.rbegin(), vi.rend()));
    BOOST_CHECK_EQUAL(str_out.str(), "[3, 2, 1]");
}

#include "../../../boost/explore/explore.hpp"
BOOST_AUTO_TEST_CASE( boost_range_print_test )
{
    std::stringstream str_out;

    typedef std::vector<int> Cont;
    typedef boost::iterator_range<Cont::iterator> IterRange;
    typedef boost::iterator_range<Cont::reverse_iterator> RevIterRange;
    Cont vi;
    explore::print(IterRange(vi), str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[]");

    str_out.str("");

    vi.push_back(1);
    explore::print(IterRange(vi), str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[1]");

    str_out.str("");

    vi.push_back(2);
    vi.push_back(3);
    explore::print(IterRange(vi), str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2, 3]");

    str_out.str("");
    explore::print(IterRange(vi.begin()+2, vi.end()), str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[3]");

    str_out.str("");
    explore::print(RevIterRange(vi.rbegin(), vi.rend()), str_out);
    BOOST_CHECK_EQUAL(str_out.str(), "[3, 2, 1]");
}
