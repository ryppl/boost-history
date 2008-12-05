// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_MANAGED_NETWORK_HPP
#define BOOST_DATAFLOW_MANAGED_NETWORK_HPP

#include <vector>
#include <set>

#include <boost/dataflow/generic/port.hpp>
#include <boost/dataflow/managed/component.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>


namespace boost { namespace asio { class io_service; }}

namespace boost { namespace dataflow { namespace managed {

template<typename T, typename PortCategory>
class port;

namespace detail {
    struct component_ptr_cmp
    {
        bool operator()(const component * const lhs, const component * const rhs) const
        {
            return lhs->topological_sort_index() < rhs->topological_sort_index();
        }
    };
    
    template<typename T>
    struct set_port_value
    {
        set_port_value(port<T, ports::producer> &port, const T &t)
            : port(port), t(t)
        {}
        
        void operator()()
        {
            port.set(t);
        }
    private:
        port<T, ports::producer> &port;
        T t;
    };
}

class network
{
    struct node_t
    {
        component *ptr;
    };
    typedef boost::adjacency_list<
        boost::vecS, boost::vecS, boost::bidirectionalS, node_t> graph_type;

public:
    network(boost::shared_ptr<asio::io_service> service=boost::shared_ptr<asio::io_service>())
        : m_io_service(service)
    {}
    
    void register_component(component *c);
    void unregister_component(component *c);
    void notify_connect(component &producer, component &consumer);
    void notify_change(component &changed)
    {
        m_changed.insert(&changed);
    }
    typedef std::set<component *, detail::component_ptr_cmp> changed_type;
    const changed_type &changed_components()
    {
        return m_changed;
    }

    void update_topological_sort();
    void update();
    
    void set_io_service(boost::shared_ptr<asio::io_service> service)
    {   m_io_service = service; }
    template<typename T>
    void async_set_port_value(port<T, ports::producer> &port, const T &t)
    {
        post_async(detail::set_port_value<T>(port, t));
    }
    asio::io_service &io_service()
    {
        return *m_io_service;
    }
private:
    void post_async(const boost::function<void()> &f);

    graph_type m_graph;
    typedef std::map<component *, graph_type::vertex_descriptor> descriptor_map_type;
    descriptor_map_type m_descriptor_map;
    changed_type m_changed;
    boost::shared_ptr<asio::io_service> m_io_service;
};

inline void notify_change(component &c)
{
    c.network_context().notify_change(c);
}

} } }

#endif // BOOST_DATAFLOW_MANAGED_PORT_HPP
