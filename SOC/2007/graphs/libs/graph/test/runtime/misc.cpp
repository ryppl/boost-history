// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <vector>
#include <tr1/unordered_map>
#include <boost/utility.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/strong_components.hpp>

using namespace std;
using namespace boost;

namespace std
{
    namespace tr1
    {
        template <>
        struct hash<boost::detail::edge_desc_impl<undirected_tag, void*> >
            : public std::unary_function<
                boost::detail::edge_desc_impl<undirected_tag, void*>,
                std::size_t>
        {
            typedef boost::detail::edge_desc_impl<undirected_tag, void*> Edge;
            std::size_t operator ()(Edge e)
            {
                std::tr1::hash<Edge::property_type*> hasher;
                return hasher(e.get_property());
            }
        };
    }
}

static void undirected_test()
{
    typedef undirected_graph<> Graph;
    typedef Graph::vertex_descriptor Vertex;
    typedef Graph::edge_descriptor Edge;
    typedef tr1::unordered_map<Vertex, int> component_map_type;
    typedef associative_property_map<component_map_type> component_map;

    const size_t N = 5;
    undirected_graph<> g;

    vector<Vertex> verts(N);
    for(size_t i = 0; i < N; ++i) {
        verts[i] = add_vertex(g);
    }
    add_edge(verts[0], verts[1], g);
    add_edge(verts[1], verts[2], g);
    add_edge(verts[3], verts[4], g);

    component_map_type mapping(num_vertices(g));
    component_map comps(mapping);
    size_t c = connected_components(g, comps);

    cout << c << "\n";

    Graph::vertex_iterator i, end;
    for(tie(i, end) = vertices(g); i != end; ++i) {
        cout << *i << "\t" << comps[*i] << "\n";
    }
}

static void directed_test()
{
    typedef directed_graph<> Graph;
    typedef Graph::vertex_descriptor Vertex;
    typedef Graph::edge_descriptor Edge;
    typedef tr1::unordered_map<Vertex, int> component_map_type;
    typedef associative_property_map<component_map_type> component_map;

    Graph g;
    Vertex u = add_vertex(g);
    Vertex v = add_vertex(g);
    add_edge(u, v, g);

    component_map_type mapping(num_vertices(g));
    component_map comps(mapping);
    strong_components(g, comps);
}

int
main()
{
    undirected_test();
    directed_test();
}
