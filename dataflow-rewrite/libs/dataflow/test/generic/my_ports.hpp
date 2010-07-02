/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__TEST__GENERIC__MY_PORT_HPP
#define BOOST__DATAFLOW__TEST__GENERIC__MY_PORT_HPP

#include <boost/dataflow/generic/port/port.hpp>

#include "my_framework.hpp"

namespace df=boost::dataflow;

typedef df::port_traits<df::ports::producer, my_framework> my_port_producer_traits;

struct my_port_producer : public df::port<my_port_producer_traits>
{
    my_port_producer()
        : int_value(0)
        , double_value(0)
    {}
    my_port_producer(int i)
        : int_value(i)
        , double_value(0)
    {}
    my_port_producer(double d)
        : int_value(0)
        , double_value(d)
    {}    
    my_port_producer(int i, double d)
        : int_value(i)
        , double_value(d)
    {}    
        
    int int_value;
    double double_value;
};

typedef df::port_traits<df::ports::consumer, my_framework> my_port_consumer_traits;

struct my_port_consumer : public df::port<my_port_consumer_traits>
{};

#endif // BOOST__DATAFLOW__TEST__GENERIC__MY_PORT_HPP