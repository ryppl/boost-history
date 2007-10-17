// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "my_producer_consumer.hpp"
#include <boost/dataflow/support/binary_operation.hpp>

bool connected = false;
bool only_connected = false;
bool disconnected = false;

namespace boost { namespace dataflow { namespace extension {

template<>
struct binary_operation_impl<operations::connect, my_producer_traits, my_consumer_traits>
{
    template<typename Producer, typename Consumer>
    struct apply
    {
        static void call(Producer &, Consumer &)
        {
            connected = true;
        }
    };
};

template<>
struct binary_operation_impl<operations::connect_only, my_producer_traits, my_consumer_traits>
{
    template<typename Producer, typename Consumer>
    struct apply
    {
        static void call(Producer &, Consumer &)
        {
            only_connected = true;
        }
    };
};

template<>
struct binary_operation_impl<operations::disconnect, my_producer_traits, my_consumer_traits>
{
    template<typename Producer, typename Consumer>
    struct apply
    {
        static void call(Producer &, Consumer &)
        {
            disconnected = true;
        }
    };
};

}}}

#include <boost/test/included/test_exec_monitor.hpp>

namespace df = boost::dataflow;

int test_main(int, char* [])
{
    my_producer p;
    my_consumer c;
    
    df::binary_operation<df::operations::connect, my_mechanism>(p, c);
    BOOST_CHECK(connected);
    
    df::binary_operation<df::operations::connect_only, my_mechanism>(p, c);
    BOOST_CHECK(only_connected);

    df::binary_operation<df::operations::disconnect, my_mechanism>(p, c);
    BOOST_CHECK(disconnected);    
    
    return 0;
} // int test_main(int, char* [])

