// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "my_producer_consumer.hpp"
#include <boost/dataflow/support/keyed_port.hpp>
#include <boost/dataflow/support/fusion_keyed_port.hpp>
#include <boost/dataflow/support/port_vector.hpp>

#include <boost/fusion/container/map.hpp>
#include <boost/test/included/test_exec_monitor.hpp>

namespace df = boost::dataflow;

int connected = 0;
bool connected_other = false;

struct my_other_producer_traits
    : public df::port_traits<df::ports::producer>
{};

struct my_other_consumer_traits
    : public df::port_traits<df::ports::consumer>
{};

struct my_other_producer : public df::port<my_other_producer_traits>
{};

struct my_other_consumer : public df::port<my_other_consumer_traits>
{};

namespace boost { namespace dataflow { namespace extension {

template<>
struct binary_operation_impl<my_producer_traits, my_consumer_traits, operations::connect>
{
    typedef void result_type;
    
    template<typename Producer, typename Consumer>
    void operator()(Producer &, Consumer &)
    {
        connected++;
    }
};

template<>
struct binary_operation_impl<my_other_producer_traits, my_other_consumer_traits, operations::connect>
{
    typedef void result_type;

    template<typename Producer, typename Consumer>
    void operator()(Producer &, Consumer &)
    {
        connected_other = true;
    }
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
        
    df::fusion_keyed_port<
        df::ports::producer,
        map_type
    > producer_map(map_type(producer, other_producer));
    
    connect(producer_map, consumer);
    connect(producer_map, other_consumer);
    
    BOOST_CHECK_EQUAL(connected, 1);
    BOOST_CHECK(connected_other);
    
    BOOST_CHECK_EQUAL(&producer, &df::get_port_c<0>(producer_map));
    BOOST_CHECK_EQUAL(&other_producer, &df::get_port_c<1>(producer_map));

    return 0;
} // int test_main(int, char* [])

