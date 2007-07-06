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
#include <typeinfo>
#include <cxxabi.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/connectivity.hpp>

using namespace std;
using namespace boost;
using namespace __cxxabiv1;

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
    // add_edge(v[0], v[4], g); // this makes it fully connected
};

void test_1()
{
    typedef adjacency_list<vecS, vecS, undirectedS> Graph;
    Graph g;
    build_graph(g);

    vector<int> comps(num_vertices(g));
    connectivity(g, &comps[0]);
}

void test_2()
{
    typedef adjacency_list<vecS, vecS, undirectedS> Graph;
    Graph g;
    build_graph(g);

    vector<int> comps(num_vertices(g));
    vector<default_color_type> colors(num_vertices(g));
    connectivity(g, &comps[0],
                           _color_map = &colors[0]);
}

void test_3()
{
    typedef adjacency_list<listS, listS, undirectedS> Graph;
    typedef map<Graph::vertex_descriptor, int> IndexMap;
    typedef map<Graph::vertex_descriptor, int> CompMap;
    typedef associative_property_map<IndexMap> IndexProperties;
    typedef associative_property_map<CompMap> CompProperties;

    Graph g;
    build_graph(g);

    IndexMap indices;
    CompMap comps;

    int x = 0;
    Graph::vertex_iterator i, end;
    for(tie(i, end) = vertices(g); i != end; ++i) {
        indices[*i] = x++;
    }

    CompProperties comp_map(comps);
    IndexProperties index_map(indices);
    connectivity(g, comp_map,
                           _vertex_index_map = index_map);
}

void test_4()
{
    typedef undirected_graph<> Graph;
    typedef tr1::unordered_map<Graph::vertex_descriptor, int> CompMap;
    typedef associative_property_map<CompMap> CompProperties;
    typedef std::vector<std::vector<Graph::vertex_descriptor> > Components;

    Graph g;
    build_graph(g);

    CompMap comps;
    CompProperties comp_map(comps);
    Components ccomps;
    connectivity(g, comp_map, _components = ccomps);

    for(size_t i = 0; i < ccomps.size(); ++i) {
        cout << i << ": " << ccomps[i].size() << "\n";
    }
}


int
main(int argc, char *argv[])
{
    test_1();
    test_2();
    test_3();
    test_4();
}
