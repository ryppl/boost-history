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
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/exterior_property.hpp>

#include <boost/graph/constant_property_map.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/visitors.hpp>

using namespace std;
using namespace boost;
using namespace __cxxabiv1;

template <typename T>
const char *
typename_of(const T&)
{
    return __cxa_demangle(typeid(T).name(), 0, 0, 0);
}

template <typename T>
const char *
typename_of()
{
    return __cxa_demangle(typeid(T).name(), 0, 0, 0);
}

struct VertexProp
{
    int x;
};

struct EdgeProp
{
    EdgeProp() : weight(1) { }

    int weight;
};

template <typename Graph>
void build_graph(Graph& g)
{
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    typedef typename graph_traits<Graph>::edge_descriptor Edge;

    static const unsigned N = 5;
    vector<Vertex> v(N);
    vector<Edge> e;

    // add some vertices
    for(size_t i = 0; i < N; ++i) {
        // v[i] = add_vertex(g);
        v[i] = add_vertex(g);
    }

    // add some edges (with weights)
    e.push_back(add_edge(v[0], v[1], g).first);
    e.push_back(add_edge(v[1], v[2], g).first);
    e.push_back(add_edge(v[2], v[0], g).first);
    e.push_back(add_edge(v[3], v[4], g).first);
    e.push_back(add_edge(v[4], v[0], g).first);
};

template <typename Graph>
void test()
{
    Graph g;
    build_graph(g);


    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    typedef typename graph_traits<Graph>::vertex_iterator VertexIterator;

    typedef exterior_vertex_property<Graph, int> DistanceProperty;
    typedef typename DistanceProperty::container_type DistanceContainer;
    typedef typename DistanceProperty::map_type DistanceMap;

    // cout << typename_of<typename Graph::graph_tag>() << "\n";
    // cout << typename_of<DistanceContainer>() << "\n";
    cout << typename_of<DistanceMap>() << "\n";

    {
        DistanceContainer dc(num_vertices(g));
        DistanceMap dm(dc, g);

        breadth_first_search(g, *vertices(g).first,
            visitor(make_bfs_visitor(record_distances(dm, on_tree_edge())))
        );
    }

    // fun with matrices
    {
        typedef typename property_map<Graph, int EdgeProp::*>::type WeightMap;
        typedef typename DistanceProperty::matrix_type DistanceMatrix;
        typedef typename DistanceProperty::matrix_map_type DistanceMatrixMap;

        DistanceMatrix dist(num_vertices(g));
        DistanceMatrixMap dm(dist, g);
        WeightMap wm(get(&EdgeProp::weight, g));

        floyd_warshall_all_pairs_shortest_paths(g, dm, weight_map(wm));

        VertexIterator i, j, end;
        for(tie(i, end) = vertices(g); i != end; ++i) {
            for(j = vertices(g).first; j != end; ++j) {
                Vertex u = *i, v = *j;
                std::cout << dm[u][v] << " ";
            }
            std::cout << "\n";
        }

        // now comes the really fun part... slicing the matrix into
        // a new property map... Good stuff.
        std::cout << "slice:\n";
        for(tie(i, end) = vertices(g); i != end; ++i) {
            DistanceMap d(dm[*i]);
            for(tie(j, end) = vertices(g); j != end; ++j) {
                std::cout << d[*j] << " ";
            }
            std::cout << "\n";
        }
    }
}


int
main(int argc, char *argv[])
{
   typedef adjacency_list<vecS, vecS, undirectedS, VertexProp, EdgeProp> AdjVector;
   typedef adjacency_list<listS, listS, undirectedS, VertexProp, EdgeProp> AdjList;
   typedef undirected_graph<VertexProp, EdgeProp> Graph;

   cout << "\n*** adjacency_list<vecS, vecS> ***\n";
   test<AdjVector>();

   // cout << "\n*** adjacency_list<listS, listS> ***\n";
   // test<AdjList>();

   cout << "\n*** undirected_graph<> ***\n";
   test<Graph>();
}
