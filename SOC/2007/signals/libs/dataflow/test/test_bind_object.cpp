// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signal/connection/detail/bind_object.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

struct adder
{
    adder(int z) : z(z) {}
    adder() : z(0) {}
    
    int add(int x, float y)
    {
        return x + y + z;
    }
    
    int z;
};

int test_main(int, char* [])
{
    adder a(10);
    
    boost::function<int(int, float)> f;
    
    f = boost::signals::detail::bind_object<int(int, float), adder>()(&adder::add, a);
    
    BOOST_CHECK_EQUAL(f(4, 2.1), 16);
    
    return 0;
}

