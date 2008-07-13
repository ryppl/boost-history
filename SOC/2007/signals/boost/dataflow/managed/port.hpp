// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_MANAGED_PORT_HPP
#define BOOST_DATAFLOW_MANAGED_PORT_HPP

#include <boost/dataflow/support/binary_operation.hpp>
#include <boost/dataflow/support/complemented_port.hpp>
#include <boost/dataflow/managed/component.hpp>
#include <boost/dataflow/managed/support.hpp>
#include <set>
#include <stdexcept>

namespace boost { namespace dataflow { namespace managed {

class port_base
{
public:
    port_base(component &component_context) : m_component_context(component_context)
    {
        
    };
    component &component_context()
    {   return m_component_context; }
private:
    component &m_component_context;
};

template<typename T, typename PortCategory>
class port;

template<typename T>
void connect(port<T, ports::producer> &producer, port<T, ports::consumer> &consumer)
{
    if(consumer.connected())
        disconnect(consumer.connected_producer(), consumer);
    producer.m_consumers.insert(&consumer);
    consumer.m_producer = &producer;
    producer.component_context().network_context().notify_connect(producer.component_context(), consumer.component_context());
}

template<typename T>
void disconnect(port<T, ports::producer> &producer, port<T, ports::consumer> &consumer)
{
    producer.m_consumers.erase(&consumer);
    consumer.m_producer = 0;
}

template<typename T>
struct producer_port_traits
    : public dataflow::complemented_port_traits<ports::producer, port<T, ports::consumer>, tag>
{};

template<typename T>
class port<T, ports::producer> : public port_base
{
public:
    typedef producer_port_traits<T> dataflow_traits;
    
    port(component &component_context) : port_base(component_context)
    {
    };
    void set(const T &value)
    {
        m_value = value;
        for(typename consumers_type::iterator it=m_consumers.begin(); it!=m_consumers.end(); it++)
            notify_change((*it)->component_context());
    }
    T &get()
    {   return m_value; }
private:
    T m_value;
    typedef std::set<port<T, ports::consumer> *> consumers_type;
    consumers_type m_consumers;

    friend void connect<T>(port<T, ports::producer> &producer, port<T, ports::consumer> &consumer);
    friend void disconnect<T>(port<T, ports::producer> &producer, port<T, ports::consumer> &consumer);
};

template<typename T>
struct consumer_port_traits
    : public dataflow::complemented_port_traits<ports::consumer, port<T, ports::producer>, tag>
{};

template<typename T>
class port<T, ports::consumer> : public port_base
{
public:
    typedef consumer_port_traits<T> dataflow_traits;
    port(component &component_context)
        : port_base(component_context), m_producer(0)
    {
    };
    bool connected() const
    {   return m_producer != 0; }
    port<T, ports::producer> &connected_producer()
    {   return *m_producer; }
    T &get()
    {
        if (!m_producer)
            throw(std::runtime_error("consumer port has no producer"));
        return m_producer->get();
    }
private:
    port<T, ports::producer> *m_producer;

    friend void connect<T>(port<T, ports::producer> &producer, port<T, ports::consumer> &consumer);
    friend void disconnect<T>(port<T, ports::producer> &producer, port<T, ports::consumer> &consumer);
};

} // namespace managed

namespace extension {

    template<typename T>
    struct binary_operation_impl<managed::producer_port_traits<T>, managed::consumer_port_traits<T>, operations::connect>
    {
        typedef void result_type;
        
        template<typename Producer, typename Consumer>
        result_type operator()(Producer &producer, Consumer &consumer)
        {
            managed::connect(producer, consumer);
        }
    };
}

} }

#endif // BOOST_DATAFLOW_MANAGED_PORT_HPP
