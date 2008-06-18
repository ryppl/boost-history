// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/support/complemented_port.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

namespace df = boost::dataflow;

typedef df::port_traits<df::ports::producer> some_traits;
typedef df::complemented_port_traits<df::ports::consumer, some_traits> my_traits;

struct entity
{
    typedef my_traits dataflow_traits;
};

int test_main(int, char* [])
{
    return 0;
}
