// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "my_producer_consumer.hpp"
#include <boost/dataflow/support/keyed_port.hpp>
#include <boost/dataflow/connection/port_map.hpp>

#include <boost/fusion/sequence/container/map.hpp>
#include <boost/test/included/test_exec_monitor.hpp>

namespace df = boost::dataflow;

bool connected = false;
bool connected_other = false;

struct my_other_producer_traits
    : public df::port_traits<
        my_mechanism,
        df::ports::producer,
        df::concepts::producer>
{};

struct my_other_consumer_traits
    : public df::port_traits<
        my_mechanism,
        df::ports::consumer,
        df::concepts::consumer>
{};

struct my_other_producer : public df::port<my_other_producer_traits>
{};

struct my_other_consumer : public df::port<my_other_consumer_traits>
{};

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
struct binary_operation_impl<operations::connect, my_other_producer_traits, my_other_consumer_traits>
{
    template<typename Producer, typename Consumer>
    struct apply
    {
        static void call(Producer &, Consumer &)
        {
            connected_other = true;
        }
    };
};

}}}        

int test_main(int, char* [])
{
    my_consumer consumer;
    my_other_consumer other_consumer;
    my_producer producer;
    my_other_producer other_producer;
    
    typedef
        boost::fusion::map<
            boost::fusion::pair<my_consumer_traits, my_producer &>,
            boost::fusion::pair<my_other_consumer_traits, my_other_producer &>
        > map_type;
        
    df::port_map<
        my_mechanism,
        df::ports::producer,
        map_type
    > producer_map(map_type(producer, other_producer));
    
    df::binary_operation<df::operations::connect, my_mechanism>(producer_map, consumer);
    df::binary_operation<df::operations::connect, my_mechanism>(producer_map, other_consumer);

    return 0;
} // int test_main(int, char* [])

