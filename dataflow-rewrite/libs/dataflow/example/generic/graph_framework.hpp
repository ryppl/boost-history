/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/generic/port.hpp>
#include <boost/dataflow/generic/port_binary_operation.hpp>
#include <boost/graph/adjacency_list.hpp>


// provide Dataflow support for an undirected BGL graph type
namespace boost { namespace dataflow { namespace ugraph {

// for this example, we support one specific BGL type.
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::undirectedS> graph_type;

// our framework will support only the connect operation.
struct framework : public dataflow::framework<operations::connect, graph_type>
{};

struct vertex_port_traits
    : public dataflow::port_traits<ports::producer_consumer, framework>
{};

struct vertex : public dataflow::port<vertex_port_traits>
{
    vertex(graph_type &graph)
        : m_graph(graph)
    {
        m_vertex_descriptor = add_vertex(graph);
    }
    graph_type &m_graph;
    graph_type::vertex_descriptor m_vertex_descriptor;
};

}

namespace extension {

    // how to connect two vertices
    template<>
    struct port_binary_operation_impl<ugraph::vertex_port_traits, ugraph::vertex_port_traits, operations::connect>
    {
        typedef void result_type;
        
        template<typename Port1, typename Port2>
        result_type operator()(Port1 &port1, Port2 &port2, const operations::connect &)
        {
            BOOST_ASSERT(&port1.m_graph == &port2.m_graph);
            add_edge(port1.m_vertex_descriptor, port2.m_vertex_descriptor, port1.m_graph);
        }
    };

}

} }
