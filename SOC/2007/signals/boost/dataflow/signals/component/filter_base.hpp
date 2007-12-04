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


namespace boost { namespace signals {

template<typename Filter, typename Signal>
struct filter_component_traits
    : public dataflow::fusion_component_traits<
        fusion::vector<
            Signal &,
            const dataflow::port_adapter<Filter, dataflow::signals::call_consumer, dataflow::signals::tag> >,
        mpl::map<
            mpl::pair<dataflow::default_port_selector
                <dataflow::directions::outgoing, dataflow::signals::connect_mechanism>,
                mpl::int_<0> >,
            mpl::pair<dataflow::default_port_selector
                <dataflow::directions::incoming, dataflow::signals::connect_mechanism>,
                mpl::int_<1> >
        > >
{};

template<typename Filter, typename Signal>
class filter_base : public dataflow::component<filter_component_traits<Filter, Signal> >
{
public:
//    using dataflow::component<filter_component_traits<Filter, Signal> >::component_traits;

    template<typename Component>
    typename filter_base::component_traits::port_result_types get_ports(Component &component)
    {
        return typename filter_base::component_traits::port_result_types(
            component.default_signal(),
            component);
    };
};

} }

#endif // SIGNAL_NETWORK_FILTER_BASE_HPP
