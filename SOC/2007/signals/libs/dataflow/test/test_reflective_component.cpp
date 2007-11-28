// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/support/component.hpp>
//#include <boost/fusion/container/vector/vector.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>

#include "my_producer_consumer.hpp"

struct my_component_traits
{
    typedef my_mechanism mechanism;
    typedef
        boost::mpl::vector<
            my_consumer_traits,
            my_producer_traits
        > ports;
};

struct my_component
{
    typedef my_component_traits component_traits;

    my_consumer consumer;
    my_producer producer;
};

namespace boost { namespace dataflow { namespace extension {

template<>
struct get_component_port_impl<my_component_traits>
{
    template<typename FArgs> struct result;

    template<typename F, typename Component>
    struct result<F(Component &, boost::mpl::int_<0>)>
    {
        typedef my_consumer & type;
    };
    
    template<typename F, typename Component>
    struct result<F(Component &, boost::mpl::int_<1>)>
    {
        typedef my_producer & type;
    };
    
    template<typename Component>
    my_consumer &
    operator()(Component &component, boost::mpl::int_<0>)
    {
        return component.consumer;
    }

    template<typename Component>
    my_producer &
    operator()(Component &component, boost::mpl::int_<1>)
    {
        return component.producer;
    }
};

}}}

#include <boost/test/included/test_exec_monitor.hpp>

namespace df = boost::dataflow;

int test_main(int, char* [])
{
    my_component c;
    
    my_consumer &cc = df::get_component_port<boost::mpl::int_<0> >(c);
    my_producer &cp = df::get_component_port<boost::mpl::int_<1> >(c);
    
    BOOST_CHECK_EQUAL(&cc, &c.consumer);
    BOOST_CHECK_EQUAL(&cp, &c.producer);
        
    return 0;
} // int test_main(int, char* [])

