// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/blueprint/component.hpp>
#include <boost/dataflow/signals/runtime_support.hpp>
#include <boost/dataflow/signals/component/storage.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

namespace df = boost::dataflow;
namespace blueprint = df::blueprint;
using namespace boost;


int test_main(int, char* [])
{
    blueprint::component_t<signals::storage<void(int)>, df::signals::tag > source, sink;
    
    BOOST_CHECK_EQUAL(source.num_ports(), 2u);
    BOOST_CHECK(source.get_port(0).traits().category().is<df::ports::producer>());
    BOOST_CHECK(source.get_port(1).traits().category().is<df::ports::consumer>());
    BOOST_CHECK(source.get_port(1).is_vector_port());
    BOOST_CHECK_EQUAL(source.get_port(1).as<blueprint::vector_port>().num_ports(), 2u);
    if(source.get_port(1).is_vector_port() && 
        source.get_port(1).as<blueprint::vector_port>().num_ports() == 2u)
    {
        BOOST_CHECK(source.get_port(1).as<blueprint::vector_port>().get_port(0)
            .traits().category().is<df::ports::consumer>());
        BOOST_CHECK(source.get_port(1).as<blueprint::vector_port>().get_port(1)
            .traits().category().is<df::ports::consumer>());
    }
    return 0;
}

