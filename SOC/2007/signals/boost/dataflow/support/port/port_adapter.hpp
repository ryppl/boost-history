// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef DATAFLOW_SUPPORT_PORT_ADAPTER_HPP
#define DATAFLOW_SUPPORT_PORT_ADAPTER_HPP

#include <boost/dataflow/support/tags.hpp>
#include <boost/dataflow/support/keyed_port.hpp>

namespace boost { namespace dataflow {

template<typename T, typename PortTraits, typename Tag=default_tag>
struct port_adapter_traits : public PortTraits
{};

template<typename T, typename PortTraits, typename Tag=default_tag>
struct port_adapter : public port<port_adapter_traits<T, PortTraits, Tag> >
{
    port_adapter(T & object) : object(object) {}
    T & object;
};

namespace extension
{
    template<typename T, typename PortTraits, typename Tag, typename KeyPortTraits>
    struct get_keyed_port_impl<port_adapter_traits<T, PortTraits, Tag>, KeyPortTraits >
    {
       typedef typename boost::result_of<
                get_keyed_port_impl<PortTraits, KeyPortTraits>(T &)
            >::type result_type;
        
        template<typename PortAdapter>
        result_type operator()(PortAdapter &adapter)
        {
            return get_keyed_port_impl<PortTraits, KeyPortTraits>()(adapter.object);
        };
    };
}

}} // namespace boost::dataflow

#endif // DATAFLOW_SUPPORT_PORT_ADAPTER_HPP