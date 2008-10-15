/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/generic/port_binary_operation.hpp>
#include <boost/dataflow/generic/operation.hpp>
#include "my_ports.hpp"


struct connect : public df::operation<df::semantics::connect>
{
    connect()
        : value()
    {}
    connect(int v)
        : value(v)
    {}
    int value;
};

namespace boost { namespace dataflow { namespace extension {

template<>
struct port_binary_operation_impl<my_port_producer_traits, my_port_consumer_traits, connect>
{
    typedef int result_type;
    
    template<typename Port1, typename Port2>
    int operator()(Port1 &port1, Port2 &port2, connect op)
    {
        return op.value;
    }
};

template<>
struct port_binary_operation_will_succeed_impl<my_port_producer_traits, my_port_consumer_traits, connect>
{
    typedef bool result_type;
    
    template<typename Port1, typename Port2>
    bool operator()(Port1 &port1, Port2 &port2, connect op)
    {
        return op.value < 10;
    }
};

}}}
