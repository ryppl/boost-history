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

namespace detail {

    template<typename Signals, typename InSignatures>
    struct default_port_map
    {
        typedef
            mpl::map<
                mpl::pair<dataflow::default_port_selector
                    <dataflow::args::left, dataflow::signals::connect_mechanism>,
                    mpl::int_<0> >,
                mpl::pair<dataflow::default_port_selector
                    <dataflow::args::right, dataflow::signals::connect_mechanism>,
                    mpl::int_<1> >
            > type;
    };
    
    template<typename InSignatures>
    struct default_port_map<void, InSignatures>
    {
        typedef
            mpl::map<
                mpl::pair<dataflow::default_port_selector
                    <dataflow::args::right, dataflow::signals::connect_mechanism>,
                    mpl::int_<0> >
            > type;
    };

}

/// ComponentTraits for a Dataflow.Signals filter.
/// \param[in] Filter Filter type (the class deriving filter)
/// \param[in] Signals A boost::signal type or void (will be: a fusion sequence of signals)
/// \param[in] InSignatures An mpl sequence of input signatures
template<typename Filter, typename Signals, typename InSignatures>
struct filter_component_traits
    : public dataflow::fusion_component_traits<
        fusion::vector<
            Signals &,
            dataflow::port_adapter<
                Filter,
                dataflow::signals::call_consumer<InSignatures>,
            dataflow::signals::tag> >,
        typename detail::default_port_map<Signals, InSignatures>::type,
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

template<typename Filter, typename InSignatures>
struct filter_component_traits<Filter, void, InSignatures>
    : public dataflow::fusion_component_traits<
        fusion::vector<
            dataflow::port_adapter<
                Filter,
                dataflow::signals::call_consumer<InSignatures>,
            dataflow::signals::tag> >,
        typename detail::default_port_map<void, InSignatures>::type,
        dataflow::signals::tag>
{
    template<typename Component>
    static typename filter_component_traits::fusion_ports get_ports(Component &component)
    {
        return typename filter_component_traits::fusion_ports(fusion::vector1<Component &>(component));
    };
};

template<typename Filter, typename Signals, typename InSignatures>
class filter_base : public dataflow::component<filter_component_traits<Filter, Signals, InSignatures> >
{
public:
    typedef InSignatures in_signatures_type;
};

} }

#endif // SIGNAL_NETWORK_FILTER_BASE_HPP
