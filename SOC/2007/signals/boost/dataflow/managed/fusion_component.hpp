// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_MANAGED_FUSION_COMPONENT_HPP
#define BOOST_DATAFLOW_MANAGED_FUSION_COMPONENT_HPP

#include <boost/dataflow/support/fusion_component.hpp>
#include <boost/dataflow/managed/port.hpp>
#include <boost/dataflow/managed/component.hpp>
#include <boost/dataflow/support/component_operation.hpp>
#include <boost/mpl/map.hpp>

namespace boost { namespace dataflow { namespace managed {

template<typename InOutType>
struct component_traits
    : public dataflow::fusion_component_traits<
        fusion::vector<
            managed::port<InOutType, ports::consumer> &,
            managed::port<InOutType, ports::producer> &
        >,
        mpl::map< >,
        managed::tag>
{
    template<typename Component>
    static typename component_traits::fusion_ports get_ports(Component &component)
    {
        return typename component_traits::fusion_ports(
            component.consumer_port(),
            component.producer_port());
    };
};

template<typename InOutType>
class fusion_component : public component
{
public:
    typedef component_traits<InOutType> dataflow_traits;
    
    fusion_component(network &n)
        : component(n)
        , m_consumer_port(*this)
        , m_producer_port(*this)
    {}
    port<InOutType, ports::consumer> &consumer_port()
    {   return m_consumer_port; }
    port<InOutType, ports::producer> &producer_port()
    {   return m_producer_port; }
protected:
    port<InOutType, ports::consumer> m_consumer_port;
    port<InOutType, ports::producer> m_producer_port;
};

}

namespace extension {

    template<typename T>
    struct component_operation_impl<managed::component_traits<T>, operations::invoke >
    {
        typedef void result_type;
        
        template<typename Component>
        void operator()(Component &component)
        {
            component.invoke();
        }
    };

}

} }

#endif // BOOST_DATAFLOW_MANAGED_FUSION_COMPONENT_HPP
