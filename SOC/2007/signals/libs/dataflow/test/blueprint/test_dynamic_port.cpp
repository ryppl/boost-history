// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/support/component_operation.hpp>
#include <boost/dataflow/support/fusion_component.hpp>
#include <boost/dataflow/blueprint/component.hpp>
#include <boost/dataflow/signals/runtime_support.hpp>
#include <boost/mpl/map.hpp>

#include "../dynamic_multi_port.hpp"

#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;


struct my_component_traits : public boost::dataflow::fusion_component_traits<
    boost::fusion::vector<
        multi_port &,
        my_consumer &
    >,
    mpl::map<
        mpl::pair<
            df::default_port_selector<df::args::left>,
            mpl::int_<0> >,
        mpl::pair<
            df::default_port_selector<dataflow::args::right>,
            mpl::int_<1> >
    >
>
{
    template<typename Component>
    static typename my_component_traits::fusion_ports get_ports(Component &c)
    {
        return typename my_component_traits::fusion_ports(c.p, c.c);
    }
};

struct my_component : public boost::dataflow::component<my_component_traits>
{
    multi_port p;
    my_consumer c;
};

namespace df = boost::dataflow;
namespace blueprint = df::blueprint;

int test_main(int, char* [])
{
    blueprint::component_t<my_component> mpc;

    BOOST_CHECK(df::is_dynamic_port<multi_port>::value);
    BOOST_CHECK(mpc.get_port(0).is_vector_port());
    if(mpc.get_port(0).is_vector_port())
        BOOST_CHECK_EQUAL(mpc.get_port(0).as<blueprint::vector_port>().num_ports(),2u);
    return 0;
}
