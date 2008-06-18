// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/blueprint/port_t.hpp>
#include <boost/dataflow/signals/runtime_support.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

namespace df = boost::dataflow;
namespace blueprint = df::blueprint;
using namespace boost;

struct collect
{
    collect() : stored() {}
    void operator()(int x)
    {
        stored = x;
    };
    int stored;
};

int test_main(int, char* [])
{
    boost::signal<void(int)> source_signal;
    collect collector;
    
    blueprint::port_t<boost::signal<void(int)> &, df::signals::tag > source(source_signal);
    blueprint::port_t<boost::function<void(int)>, df::signals::tag > sink(boost::ref(collector));
    
    BOOST_CHECK((df::is_complemented_port<boost::signal<void(int)>, df::signals::tag>::value));
    BOOST_CHECK(source.is_complemented_port());
    BOOST_CHECK(!sink.is_complemented_port());
    BOOST_CHECK(blueprint::are_binary_operable<df::operations::connect>(source, sink));
    BOOST_CHECK(!blueprint::are_binary_operable<df::operations::extract>(source, sink));
    return 0;
}
