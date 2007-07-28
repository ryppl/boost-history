// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <map>
#include <tr1/unordered_map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/connectivity.hpp>
#include <boost/foreach.hpp>

using namespace std;
using namespace boost;

template <typename Graph>
void build_graph(Graph& g)
{
    typedef typename Graph::vertex_descriptor Vertex;

    static const unsigned N = 5;
    vector<Vertex> v(N);

    // add some vertices
    for(size_t i = 0; i < N; ++i) {
        // v[i] = add_vertex(g);
        v[i] = add_vertex(g);
    }

    // add some edges
    add_edge(v[0], v[1], g);
    add_edge(v[1], v[2], g);
    add_edge(v[2], v[0], g);

    add_edge(v[3], v[4], g);
};

template <typename Graph>
void test_external()
{
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;

    typedef exterior_vertex_property<Graph, size_t> ComponentProperty;
    typedef typename ComponentProperty::container_type ComponentContainer;
    typedef typename ComponentProperty::map_type ComponentMap;

    Graph g;
    build_graph(g);


    ComponentContainer comps(num_vertices(g));
    ComponentMap comps_map(comps);

    size_t n = connected_components(g, comps_map);

    typedef vector<Vertex> VertexList;
    typedef vector<VertexList> Component;
    Component components;
    connectivity(
        _graph = g,
        _components = components,
        _number = n,
        _component_map = comps_map);

    size_t c = 0;
    BOOST_FOREACH(VertexList& vl, components) {
        cout << "component " << c++ << ": ";
        BOOST_FOREACH(Vertex v, vl) {
            cout << get(vertex_index, g, v) << " ";
        }
        cout << endl;
    }
}

template <typename Graph>
void test_internal()
{
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;

    typedef exterior_vertex_property<Graph, size_t> ComponentProperty;
    typedef typename ComponentProperty::container_type ComponentContainer;
    typedef typename ComponentProperty::map_type ComponentMap;

    Graph g;
    build_graph(g);

    typedef vector<Vertex> VertexList;
    typedef vector<VertexList> Component;
    Component components;
    connectivity(_graph = g, _components = components);

    size_t c = 0;
    BOOST_FOREACH(VertexList& vl, components) {
        cout << "component " << c++ << ": ";
        BOOST_FOREACH(Vertex v, vl) {
            cout << get(vertex_index, g, v) << " ";
        }
        cout << endl;
    }
}

int
main(int argc, char *argv[])
{
    typedef adjacency_list<vecS, vecS, undirectedS> AdjacencyList;
    typedef undirected_graph<> Graph;

    std::cout << "*** adjacency_list<vecS, vecS> (external) ***\n";
    test_external<AdjacencyList>();
    std::cout << "\n\n";

    std::cout << "*** undirected_graph<> (external) ***\n";
    test_external<Graph>();
    std::cout << "\n\n";

    std::cout << "*** adjacency_list<vecS, vecS> (internal) ***\n";
    test_external<AdjacencyList>();
    std::cout << "\n\n";

    std::cout << "*** undirected_graph<> (internal) ***\n";
    test_external<Graph>();
    std::cout << "\n\n";

    return 0;
}
