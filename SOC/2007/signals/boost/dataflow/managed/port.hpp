// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_MANAGED_PORT_HPP
#define BOOST_DATAFLOW_MANAGED_PORT_HPP

#include <boost/dataflow/managed/component.hpp>
#include <boost/dataflow/support/tags.hpp>
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
}

template<typename T>
void disconnect(port<T, ports::producer> &producer, port<T, ports::consumer> &consumer)
{
    producer.m_consumers.erase(&consumer);
    consumer.m_producer = 0;
}

template<typename T>
class port<T, ports::producer> : public port_base
{
public:
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
class port<T, ports::consumer> : public port_base
{
public:
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

} } }

#endif // BOOST_DATAFLOW_MANAGED_PORT_HPP