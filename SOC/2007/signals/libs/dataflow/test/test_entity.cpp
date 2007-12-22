// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/support/traits.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

namespace df = boost::dataflow;

typedef df::traits<df::default_tag> my_traits;

struct entity
{
    typedef my_traits dataflow_traits;
};

int test_main(int, char* [])
{
    BOOST_CHECK(df::is_traits<my_traits>::value);
    BOOST_CHECK(df::is_entity<entity>::value);

    return 0;
} // int test_main(int, char* [])
