// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iterator>
#include <algorithm>

#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/degree_distribution.hpp>
#include <boost/graph/connectivity.hpp>

#include "movies.hpp"

using namespace std;
using namespace boost;

typedef vector<graph_traits<Graph>::degree_size_type> Distribution;
typedef list<Vertex> VectorList;
typedef vector<VectorList> Histogram;

int
main(int argc, char *argv[])
{
    Graph g;
    ActorMap actors;

    // build the movie graph from std input
    build_movie_graph(cin, g, actors);

    Distribution dist;
    Histogram hist;
    degree_distribution(g, dist);
    degree_histogram(g, hist);

    cout << "vertices: " << num_vertices(g) << "\n";
    cout << "edges: " << num_edges(g) << "\n";
    cout << "degree distribution: ";
    copy(dist.begin(), dist.end(), ostream_iterator<size_t>(cout, " "));
    cout << "\n";
    cout << "max degree: " << (dist.size() - 1) << "\n";
    cout << "most-connected actor: " << g[hist.back().back()].name << "\n";

    return 0;
}
