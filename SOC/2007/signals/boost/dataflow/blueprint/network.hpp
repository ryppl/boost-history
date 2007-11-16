// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_NETWORK_HPP
#define BOOST_DATAFLOW_BLUEPRINT_NETWORK_HPP

#include <boost/dataflow/blueprint/component.hpp>
#include <boost/dataflow/blueprint/connection_t.hpp>

#include <boost/graph/adjacency_list.hpp>

#include <exception>
#include <iostream>

namespace boost { namespace dataflow { namespace blueprint {

template<typename Mechanism>
class network
{
    struct node_t
    {
        boost::shared_ptr<component> ptr;
    };
    struct edge_t
    {
        boost::shared_ptr<connection> ptr;
        int producer_port;
        int consumer_port;
    };
public:
    typedef boost::adjacency_list<
        boost::vecS, boost::vecS, boost::bidirectionalS, node_t, edge_t> graph_type;

    typedef typename graph_type::vertex_descriptor component_type;

    template<typename Component>
    component_type add_component()
    {
        typename graph_type::vertex_descriptor v = add_vertex(g);
        g[v].ptr.reset(new component_t<Mechanism, Component>());
        return v;
    }
    template<typename Component, typename T0>
    component_type add_component(const T0 &t0)
    {
        typename graph_type::vertex_descriptor v = add_vertex(g);
        g[v].ptr.reset(new component_t<Mechanism, Component>(t0));
        return v;
    }
    template<typename ProducerPort, typename ConsumerPort>
    void add_connection(component_type p, int p_port, component_type c, int c_port)
    {
        add_connection_(p, p_port, c, c_port, new connection_t<Mechanism, ProducerPort, ConsumerPort>());
    }
    void add_connection(component_type p, int p_port, component_type c, int c_port)
    {
        if (!are_connectable(p, p_port, c, c_port))
            throw(std::exception());

        add_connection_(p, p_port, c, c_port, get_port(p, p_port)->connector());
    }
    bool are_connectable(component_type p, int p_port, component_type c, int c_port)
    {
        return (
            (typeid(*get_port(p, p_port)->connector().get())
                == typeid(*get_port(c, c_port)->connector().get()))
            && (get_port(p, p_port)->category_uuid() == 0)
            && (get_port(c, c_port)->category_uuid() == 1));
    }
    void connect()
    {
        typename graph_type::edge_iterator it;
        for (it = edges(g).first; it!=edges(g).second; it++)
            g[*it].ptr->connect(
                *g[source(*it, g)].ptr->get_port(g[*it].producer_port),
                *g[target(*it, g)].ptr->get_port(g[*it].consumer_port));
    }
    component &operator[](component_type c)
    {
        return *g[c].ptr;
    }
    std::auto_ptr<port> get_port(component_type c, int port)
    {
        return g[c].ptr->get_port(port);
    }
    const graph_type &graph()
    {
        return g;
    }
private:
    void add_connection_(component_type p, int p_port, component_type c, int c_port,
        boost::shared_ptr<connection>)
    {
        typename graph_type::edge_descriptor e = add_edge(p, c, g).first;
        g[e].ptr = g[p].ptr->get_port(p_port)->connector();
        g[e].producer_port = p_port;
        g[e].consumer_port = c_port;        
    }
    graph_type g;
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST_DATAFLOW_BLUEPRINT_NETWORK_HPP