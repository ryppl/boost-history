// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/explore.hpp>
#include "boost_explore_test_tools.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE( basic_tuple_stream_test, C, test_types )
{
    typename test_traits<C>::stream_type str_out;

    boost::tuples::tuple<int, double> t(1, 3.14);
    str_out << t;
    // I think things that already have a way to print should not change
    //BOOST_CHECK_EQUAL(output(str_out), "[1, 3.14]");
    BOOST_CHECK_EQUAL(output(str_out), "(1 3.14)");
}
