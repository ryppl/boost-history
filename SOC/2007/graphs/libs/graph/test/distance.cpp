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
#include <cxxabi.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/exterior_property.hpp>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/distance.hpp>
#include <boost/graph/closeness_centrality.hpp>

using namespace std;
using namespace boost;

struct VertexProp
{
    int dummy;
};

struct EdgeProp
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

template <typename Graph, typename PropertyMap>
void print_map(const Graph& g, PropertyMap pm)
{
    typename Graph::vertex_iterator i, end;
    cout << "{ ";
    for(tie(i, end) = vertices(g); i != end; ++i) {
        cout << pm[*i] << " ";
    }
    cout << "}\n";
}

template <typename Graph, typename Matrix>
void print_matrix(const Graph& g, Matrix m)
{
    cout << "[\n";
    typename Graph::vertex_iterator i, j, end;
    for(tie(i, end) = vertices(g); i != end; ++i) {
        print_map(g, m[*i]);
    }
    cout << "]\n";
}

template <typename Graph>
void test()
{
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    typedef typename graph_traits<Graph>::edge_descriptor Edge;


    typedef exterior_vertex_property<Graph, int> DistanceProperty;
    typedef typename DistanceProperty::container_type DistanceContainer;
    typedef typename DistanceProperty::map_type DistanceMap;
    typedef typename property_map<Graph, int EdgeProp::*>::type WeightMap;

    Graph g;
    build_graph(g);

    // single-vertex computations
    {
        DistanceContainer distances(num_vertices(g));
        DistanceMap dists(distances);
        WeightMap weights = get(&EdgeProp::weight, g);

        dijkstra_shortest_paths(g, *vertices(g).first,
            weight_map(weights).
            distance_map(dists));

        double total_geo = total_geodesic_distance(g, dists);
        double mean_geo = mean_geodesic_distance(g, dists);
        double inverse_geo = inverse_geodesic_distance(g, dists);
        double close = closeness(g, dists);

        cout << "* geodesics: "; print_map(g, dists);
        cout << "* total geo: " << total_geo << "\n";
        cout << "* mean geo: " << mean_geo << "\n";
        cout << "* inv geo: " << inverse_geo << "\n";
        cout << "* closeness: " << close << "\n";
    }


    // all-vertices computation
    {
        typedef typename DistanceProperty::matrix_type DistanceMatrix;

        WeightMap weights = get(&EdgeProp::weight, g);
        DistanceMatrix dists(num_vertices(g));

        // compute all shortest paths so we can run some other stuff
        floyd_warshall_all_pairs_shortest_paths(g, dists,
                weight_map(weights));

        print_matrix(g, dists);

        typedef exterior_vertex_property<Graph, float> ClosenessProperty;
        typedef typename ClosenessProperty::container_type ClosenessContainer;
        typedef typename ClosenessProperty::map_type ClosenessMap;

        {
            ClosenessContainer c(num_vertices(g));
            ClosenessMap m(c);
            all_total_geodesic_distances(g, dists, m);
            std::cout << "all total geodesics: "; print_map(g, m);
        }

        {
            ClosenessContainer c(num_vertices(g));
            ClosenessMap m(c);
            all_mean_geodesic_distances(g, dists, m);
            std::cout << "all mean geodesics: "; print_map(g, m);
        }

        {
            ClosenessContainer c(num_vertices(g));
            ClosenessMap m(c);
            all_inverse_geodesic_distances(g, dists, m);
            std::cout << "all inverse geodesics: "; print_map(g, m);
        }

        {
            ClosenessContainer c(num_vertices(g));
            ClosenessMap m(c);
            all_closenesses(g, dists, m);
            std::cout << "all closenesses: "; print_map(g, m);
        }
    }
}

int
main(int argc, char *argv[])
{
    typedef undirected_graph<VertexProp, EdgeProp> Graph;
    typedef adjacency_list<vecS, vecS, undirectedS, VertexProp, EdgeProp> AdjList;

    cout << "\n*** undirected_graph<> *** \n";
    test<Graph>();

    cout << "\n*** adjacency_list<> *** \n";
    test<AdjList>();
}
