/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__TEST__GENERIC__MY_PORTS_WITH_CONTEXT_HPP
#define BOOST__DATAFLOW__TEST__GENERIC__MY_PORTS_WITH_CONTEXT_HPP

#include <boost/dataflow/generic/port/port.hpp>

#include "my_framework_with_context.hpp"

namespace df=boost::dataflow;

typedef df::port_traits<df::ports::producer_consumer, my_framework_with_context> my_port_with_context_traits;

struct my_port_with_context : public df::port<my_port_with_context_traits>
{
    my_port_with_context(my_object)
        : int_value(0)
        , double_value(0)
    {}
    my_port_with_context(my_object, int i)
        : int_value(i)
        , double_value(0)
    {}
    my_port_with_context(my_object, double d)
        : int_value(0)
        , double_value(d)
    {}    
    my_port_with_context(my_object, int i, double d)
        : int_value(i)
        , double_value(d)
    {}    
        
    int int_value;
    double double_value;
};

#endif // BOOST__DATAFLOW__TEST__GENERIC__MY_PORTS_WITH_CONTEXT_HPP