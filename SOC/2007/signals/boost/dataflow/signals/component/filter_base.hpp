// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_FILTER_BASE_HPP
#define SIGNAL_NETWORK_FILTER_BASE_HPP

#include <boost/dataflow/signals/support.hpp>
#include <boost/dataflow/support/fusion_component.hpp>
#include <boost/dataflow/support/port/port_adapter.hpp>

#include <boost/signal.hpp>


namespace boost {  namespace signals {
    
template<typename Filter, typename Signal, typename OutSignatures>
struct filter_component_traits
    : public dataflow::fusion_component_traits<
        fusion::vector<
            Signal &,
            dataflow::port_adapter<
                Filter,
                dataflow::signals::call_consumer<OutSignatures>,
            dataflow::signals::tag> >,
        mpl::map<
            mpl::pair<dataflow::default_port_selector
                <dataflow::directions::outgoing, dataflow::signals::connect_mechanism>,
                mpl::int_<0> >,
            mpl::pair<dataflow::default_port_selector
                <dataflow::directions::incoming, dataflow::signals::connect_mechanism>,
                mpl::int_<1> >
        >,
        dataflow::signals::tag>
{
    template<typename Component>
    static typename filter_component_traits::fusion_ports get_ports(Component &component)
    {
        return typename filter_component_traits::fusion_ports(
            component.default_signal(),
            component);
    };
};

template<typename Filter, typename Signal, typename OutSignatures>
class filter_base : public dataflow::component<filter_component_traits<Filter, Signal, OutSignatures> >
{
};

} }

#endif // SIGNAL_NETWORK_FILTER_BASE_HPP
