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
struct port_adapter : public port<PortTraits >
{
    port_adapter(T & object) : object(object) {}
    T & object;
};

template<typename T>
inline T &get_object(T &t)
{
    return t;
}

template<typename T>
inline const T&get_object(const T &t)
{
    return t;
}

template<typename T, typename PortTraits, typename Tag>
inline T&get_object(const port_adapter<T, PortTraits, Tag> &t)
{
    return t.object;
}

template<typename T, typename PortTraits, typename Tag>
inline T&get_object(port_adapter<T, PortTraits, Tag> &t)
{
    return t.object;
}

template<typename T>
struct get_object_type
{
    typedef T type;
};

template<typename T, typename PortTraits, typename Tag>
struct get_object_type<port_adapter<T, PortTraits, Tag> >
{
    typedef T type;
};

template<typename T, typename PortTraits, typename Tag>
struct get_object_type<const port_adapter<T, PortTraits, Tag> >
{
    typedef T type;
};

}} // namespace boost::dataflow

#endif // DATAFLOW_SUPPORT_PORT_ADAPTER_HPP
