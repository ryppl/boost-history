// Boost.Explore library
//
// Copyright (C) 2007, Jared McIntyre
// Copyright (C) 2009, Jeffrey Faust
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <complex>
#include <boost/explore/list.hpp>
#include <boost/explore/iterator_range.hpp>
#include "boost_explore_test_tools.hpp"

// safe_test_types: trouble streaming complex to wchar_t stream with no intrinsic wchar_t
BOOST_AUTO_TEST_CASE_TEMPLATE( basic_list_stream_test, C, safe_test_types )
{
    typename test_traits<C>::stream_type str_out;

    std::list<std::complex<double> > lc;
    str_out << lc;
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    lc.push_back(std::complex<double>(0,1));
    str_out << lc;
    BOOST_CHECK_EQUAL(output(str_out), "[(0,1)]");

    reset(str_out);

    lc.push_back(std::complex<double>(2,3));
    lc.push_back(std::complex<double>(4,5));
    str_out << lc;
    BOOST_CHECK_EQUAL(output(str_out), "[(0,1), (2,3), (4,5)]");

    reset(str_out);

    str_out << boost::explore::make_iterator_range(lc.begin(), ++(++lc.begin()));
    BOOST_CHECK_EQUAL(output(str_out), "[(0,1), (2,3)]");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( list_in_list_stream_test, C, test_types )
{
    typename test_traits<C>::stream_type str_out;

    std::list<int> li;
    li.push_back(1);
    li.push_back(2);
    li.push_back(3);

    std::list<std::list<int> > lli;
    str_out << lli;
    BOOST_CHECK_EQUAL(output(str_out), "[]");

    reset(str_out);

    lli.push_back(li);
    str_out << lli;
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3]]");

    reset(str_out);

    lli.push_back(li);
    lli.push_back(li);
    str_out << lli;
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");

    reset(str_out);

    str_out << boost::explore::make_iterator_range(lli.begin(), ++(++lli.begin()));
    BOOST_CHECK_EQUAL(output(str_out), "[[1, 2, 3], [1, 2, 3]]");
}