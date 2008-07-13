// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/blueprint/port_t.hpp>
#include <boost/dataflow/managed/runtime_support.hpp>
#include <boost/dataflow/managed/fusion_component.hpp>
#include <boost/dataflow/managed/network.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

namespace df = boost::dataflow;
namespace blueprint = df::blueprint;
using namespace boost;

int test_main(int, char* [])
{
    df::managed::network network;
    df::managed::fusion_component<bool> component(network);
        
    blueprint::port_t<df::managed::port<bool, df::ports::producer> &, df::managed::tag > source(component.producer_port());
    blueprint::port_t<df::managed::port<bool, df::ports::consumer>, df::managed::tag > sink(component.consumer_port());
    
    BOOST_CHECK((df::is_complemented_port<df::managed::port<bool, df::ports::producer>, df::managed::tag>::value));
    BOOST_CHECK(source.is_complemented_port());
    BOOST_CHECK(sink.is_complemented_port());
    BOOST_CHECK(blueprint::are_binary_operable<df::operations::connect>(source, sink));
    return 0;
}
