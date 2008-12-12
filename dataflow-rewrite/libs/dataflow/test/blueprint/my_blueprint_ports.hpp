/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/blueprint/port_adapter.hpp>
#include <boost/dataflow/utility/containing_ptr.hpp>

#include "my_blueprint_framework.hpp"
#include "../generic/my_ports.hpp"

typedef
    df::blueprint::port_adapter
    <
        my_blueprint_framework,
        df::utility::containing_ptr<my_port_producer>
    >
        my_blueprint_port_producer;

typedef
    df::blueprint::port_adapter
    <
        my_blueprint_framework,
        df::utility::containing_ptr<my_port_consumer>
    >
        my_blueprint_port_consumer;
