// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_NETWORK_HPP
#define BOOST_DATAFLOW_BLUEPRINT_NETWORK_HPP

#include <boost/dataflow/blueprint/component.hpp>
#include <boost/dataflow/blueprint/binary_operation_t.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>

#include <exception>

namespace boost { namespace dataflow { namespace blueprint {

template<typename Tag>
class network
{
    struct node_t
    {
        boost::shared_ptr<component> ptr;
    };
    struct edge_t
    {
        boost::shared_ptr<binary_operation<operations::connect> > ptr;
        int producer_port;
        int consumer_port;
    };
public:
    typedef boost::adjacency_list<
        boost::vecS, boost::vecS, boost::bidirectionalS, node_t, edge_t> graph_type;
    typedef typename graph_type::vertex_descriptor component_type;
    typedef typename graph_type::edge_descriptor connection_type;
private:
    struct vc
    {
        vc(const graph_type &g_src, graph_type &g_dst)
            : g_src(g_src), g_dst(g_dst)
        {}
        void operator()(component_type src, component_type &dst)
        {
            g_dst[dst].ptr = g_src[src].ptr->copy();
        }
        const graph_type &g_src;
        graph_type &g_dst;
    };
    struct ec
    {
        ec(const graph_type &g_src, graph_type &g_dst)
            : g_src(g_src), g_dst(g_dst)
        {}
        void operator()(connection_type src, connection_type &dst)
        {
            g_dst[dst].ptr = g_src[src].ptr->copy();
            g_dst[dst].producer_port = g_src[src].producer_port;
            g_dst[dst].consumer_port = g_src[src].consumer_port;
        }
        const graph_type &g_src;
        graph_type &g_dst;
    };
public:
    network() {}
    network(const network &n)
    {
        copy_graph(n.g, g, vertex_copy(vc(n.g, g)).edge_copy(ec(n.g, g)));
    }
    
    template<typename Component>
    component_type add_component()
    {
        typename graph_type::vertex_descriptor v = add_vertex(g);
        g[v].ptr.reset(new component_t<Component, Tag>());
        return v;
    }
    template<typename Component, typename T0>
    component_type add_component(const T0 &t0)
    {
        typename graph_type::vertex_descriptor v = add_vertex(g);
        g[v].ptr.reset(new component_t<Component, Tag>(t0));
        return v;
    }
    template<typename Mechanism, typename ProducerPort, typename ConsumerPort>
    void add_connection(component_type p, int p_port, component_type c, int c_port)
    {
        add_connection_(p, p_port, c, c_port, new binary_operation_t<operations::connect, Mechanism, ProducerPort, ConsumerPort>());
    }
    void add_connection(component_type p, int p_port, component_type c, int c_port)
    {
        if (!are_connectable(p, p_port, c, c_port))
            throw(std::exception());

        add_connection_(p, p_port, c, c_port,
            get_binary_operation<operations::connect>(get_port(p, p_port), get_port(c, c_port)));
    }
    bool are_connectable(component_type p, int p_port, component_type c, int c_port)
    {
        return are_binary_operable<operations::connect>(get_port(p, p_port), get_port(c, c_port));
    }
    void connect()
    {
        typename graph_type::edge_iterator it;
        for (it = edges(g).first; it!=edges(g).second; it++)
            g[*it].ptr->invoke(
                g[source(*it, g)].ptr->get_port(g[*it].producer_port),
                g[target(*it, g)].ptr->get_port(g[*it].consumer_port));
    }
    component &operator[](component_type c)
    {
        return *g[c].ptr;
    }
    port & get_port(component_type c, int port)
    {
        return g[c].ptr->get_port(port);
    }
    const graph_type &graph()
    {
        return g;
    }
    
private:
    void add_connection_(component_type p, int p_port, component_type c, int c_port,
        boost::shared_ptr<binary_operation<operations::connect> >)
    {
        typename graph_type::edge_descriptor e = add_edge(p, c, g).first;
        g[e].ptr = get_binary_operation<operations::connect>(get_port(p, p_port), get_port(c, c_port));
        g[e].producer_port = p_port;
        g[e].consumer_port = c_port;        
    }
    graph_type g;
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST_DATAFLOW_BLUEPRINT_NETWORK_HPP