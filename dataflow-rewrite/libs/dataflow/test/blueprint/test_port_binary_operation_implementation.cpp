/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/blueprint/port_binary_operation_implementation_adapter.hpp>
#include <boost/dataflow/blueprint/operation_adapter.hpp>

#include "my_blueprint_ports.hpp"
#include "../generic/my_connect.hpp"

struct void_connect : public df::operation<df::semantics::connect>
{};

namespace boost { namespace dataflow { namespace extension {

template<>
struct port_binary_operation_impl<my_port_producer_traits, my_port_consumer_traits, void_connect>
{
    typedef void result_type;
    
    template<typename Port1, typename Port2>
    void operator()(Port1 &port1, Port2 &port2, void_connect op)
    {}
};

}}}

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


namespace df = boost::dataflow;


BOOST_AUTO_TEST_CASE( test ) 
{
    my_blueprint_framework_context fo;

    my_blueprint_port_producer producer(fo);
    my_blueprint_port_consumer consumer(fo);
    
    df::blueprint::operation_adapter<connect> c;
    df::blueprint::operation_adapter<connect> c1(1);
    df::blueprint::operation_adapter<connect> c11(11);

    std::auto_ptr<df::blueprint::port_binary_operation_implementation<my_blueprint_framework> >
        op(new df::blueprint::port_binary_operation_implementation_adapter<my_port_producer, my_port_consumer, connect, my_blueprint_framework>);
        
    BOOST_CHECK(( op->will_succeed(producer, consumer, c) ));
    BOOST_CHECK(( op->will_succeed(producer, consumer, c1) ));
    BOOST_CHECK(( !op->will_succeed(producer, consumer, c11) ));

    BOOST_CHECK_EQUAL( boost::any_cast<int>(op->invoke(producer, consumer, c)), 0 );
    BOOST_CHECK_EQUAL( boost::any_cast<int>(op->invoke(producer, consumer, c1)), 1 );
    
    std::auto_ptr<df::blueprint::port_binary_operation_implementation<my_blueprint_framework> >
        void_op(new df::blueprint::port_binary_operation_implementation_adapter<my_port_producer, my_port_consumer, void_connect, my_blueprint_framework>);
}
