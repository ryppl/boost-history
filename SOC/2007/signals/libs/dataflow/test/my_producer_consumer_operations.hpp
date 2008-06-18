// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef DATAFLOW_TEST_MY_PRODUCER_CONSUMER_OPERATIONS_HPP
#define DATAFLOW_TEST_MY_PRODUCER_CONSUMER_OPERATIONS_HPP

#include <boost/dataflow/support/binary_operation.hpp>


bool connected = false;
bool only_connected = false;
bool disconnected = false;

namespace boost { namespace dataflow { namespace extension {

template<>
struct binary_operation_impl<my_producer_traits, my_consumer_traits, operations::connect>
{
    typedef void result_type;
    
    template<typename Producer, typename Consumer>
    void operator()(Producer &, Consumer &)
    {
        connected = true;
    }
};

template<>
struct binary_operation_impl<my_producer_traits, my_consumer_traits, operations::connect_only>
{
    typedef void result_type;

    template<typename Producer, typename Consumer>
    void operator()(Producer &, Consumer &)
    {
        only_connected = true;
    }
};

template<>
struct binary_operation_impl<my_producer_traits, my_consumer_traits, operations::disconnect>
{
    typedef void result_type;

    template<typename Producer, typename Consumer>
    void operator()(Producer &, Consumer &)
    {
        disconnected = true;
    }
};

}}}


#endif
