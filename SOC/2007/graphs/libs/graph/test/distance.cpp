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
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/distance.hpp>

using namespace std;
using namespace boost;

struct VertexProperty
{
    int dummy;
};

struct EdgeProperty
{
    int weight;
};

template <typename Graph>
void build_graph(Graph& g)
{
    typedef typename Graph::vertex_descriptor Vertex;
    typedef typename Graph::edge_descriptor Edge;

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

    g[e[0]].weight = 1;
    g[e[1]].weight = 1;
    g[e[2]].weight = 1;
    g[e[3]].weight = 1;
    g[e[4]].weight = 1;
};

template <typename Graph, typename DistanceMap>
void dump_distance_map(const Graph& g, DistanceMap dists)
{
    typename Graph::vertex_iterator i, end;
    for(tie(i, end) = vertices(g); i != end; ++i) {
        cout << dists[*i] << " ";
    }
    cout << "\n";
}

template <typename Graph, typename DistanceMatrix>
void dump_distance_matrix(const Graph& g, DistanceMatrix dists)
{
    typename Graph::vertex_iterator i, j, end;
    for(tie(i, end) = vertices(g); i != end; ++i) {
        for(j = vertices(g).first; j != end; ++j) {
            cout << dists[*i][*j] << " ";
        }
        cout << "\n";
    }
}

void test_1()
{
    // because this is defined w/vecS's, we don't have to work very
    // hard on property maps

    typedef adjacency_list<vecS, vecS, undirectedS, VertexProperty, EdgeProperty> Graph;
    typedef Graph::vertex_descriptor Vertex;
    typedef property_map<Graph, int EdgeProperty::*>::type WeightPropertyMap;
    typedef vector<int> DistanceMap;
    typedef iterator_property_map<DistanceMap::iterator,
        property_map<Graph, vertex_index_t>::type> DistancePropertyMap;

    Graph g;
    build_graph(g);

    Vertex v = *vertices(g).first;
    WeightPropertyMap weights = get(&EdgeProperty::weight, g);

    cout << "\nadjacency_list<vecS, vecS...>\n";
    {
        DistanceMap distances(num_vertices(g));
        DistancePropertyMap dists(distances.begin());

        dijkstra_shortest_paths(g, v,
            weight_map(weights).
            distance_map(dists));

        cout << "* dists: "; dump_distance_map(g, dists);
        cout << "* mean geo: " << mean_geodesic_distance(g, dists) << "\n";
        cout << "* closeness: " << closeness(g, dists) << "\n";
        cout << "* eccentricity: " << eccentricity(g, dists) << "\n";
    }

    {
        typedef vector<DistanceMap> DistanceMatrix;

        DistanceMatrix dists(num_vertices(g), DistanceMap(num_vertices(g)));

        // compute all shortest paths
        floyd_warshall_all_pairs_shortest_paths(g, dists,
                weight_map(weights));

        // use the distances in all-pairs to compute eccentricities
        // for each vertex
        DistanceMap eccentrics(num_vertices(g));
        DistancePropertyMap eccs(eccentrics.begin());
        eccentricities(g, dists, eccs);

        cout << "* dists:\n"; dump_distance_matrix(g, dists);
        cout << "* eccs: "; dump_distance_map(g, eccs);
        cout << "* radius: " << radius(g, eccs) << "\n";
        cout << "* diameter: " << diameter(g, eccs) << "\n";

        vector<Vertex> cent;
        center(g, eccs, back_inserter(cent));
        cout << "center: ";
        for(size_t x = 0; x < cent.size(); ++x) {
            Vertex v = cent[x];
            cout << get(vertex_index, g, v) << " ";
        }
        cout << "\n";

        vector<Vertex> peri;
        periphery(g, eccs, back_inserter(peri));
        cout << "periphery: ";
        for(size_t x = 0; x < peri.size(); ++x) {
            Vertex v = peri[x];
            cout << get(vertex_index, g, v) << " ";
        }
        cout << "\n";
    }
}

void test_2()
{
}

void test_3()
{
    typedef undirected_graph<VertexProperty, EdgeProperty> Graph;
    typedef Graph::vertex_descriptor Vertex;

    typedef property_map<Graph, int EdgeProperty::*>::type WeightPropertyMap;

    typedef tr1::unordered_map<Vertex, int> DistanceMap;
    typedef associative_property_map<DistanceMap> DistancePropertyMap;

    Graph g;
    build_graph(g);

    Vertex v = *vertices(g).first;
    WeightPropertyMap weights = get(&EdgeProperty::weight, g);

    cout << "\nundirected_graph<...>\n";
    {
        DistanceMap distances(num_vertices(g));
        DistancePropertyMap dists(distances);

        // compute shortest paths
        dijkstra_shortest_paths(g, v,
            weight_map(weights).
            distance_map(dists)
            );

        cout << "* dists: "; dump_distance_map(g, dists);
        cout << "* mean geo: " << mean_geodesic_distance(g, dists) << "\n";
        cout << "* closeness: " << closeness(g, dists) << "\n";
        cout << "* eccentricity: " << eccentricity(g, dists) << "\n";
    }

    {
        typedef tr1::unordered_map<Vertex, DistanceMap> DistanceMatrix;

        DistanceMatrix dists(num_vertices(g));
        Graph::vertex_iterator i, end;
        for(tie(i, end) = vertices(g); i != end; ++i) {
            dists[*i].rehash(num_vertices(g));
        }

        // compute all shortest paths
        floyd_warshall_all_pairs_shortest_paths(g, dists,
            weight_map(weights));

        // use the distances in all-pairs to compute eccentricities
        // for each vertex
        DistanceMap eccentrics(num_vertices(g));
        DistancePropertyMap eccs(eccentrics);
        eccentricities(g, dists, eccs);

        int r, d;
        cout << "* dists:\n"; dump_distance_matrix(g, dists);
        cout << "* eccs: "; dump_distance_map(g, eccs);
        cout << "* radius: " << (r = radius(g, eccs)) << "\n";
        cout << "* diameter: " << (d = diameter(g, eccs)) << "\n";

        vector<Vertex> cent;
        center(g, eccs, back_inserter(cent));
        cout << "center: ";
        for(size_t x = 0; x < cent.size(); ++x) {
            Vertex v = cent[x];
            cout << get(vertex_index, g, v) << " ";
        }
        cout << "\n";

        vector<Vertex> peri;
        periphery(g, eccs, back_inserter(peri));
        cout << "periphery: ";
        for(size_t x = 0; x < peri.size(); ++x) {
            Vertex v = peri[x];
            cout << get(vertex_index, g, v) << " ";
        }
        cout << "\n";
    }
}


int
main(int argc, char *argv[])
{
    test_1();
    // test_2();
    test_3();
}
