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

#include <typeinfo>
#include <cxxabi.h>

#include "movies.hpp"

using namespace std;
using namespace boost;
using namespace __cxxabiv1;


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

    vector<size_t> dist;
    vector<size_t> out_dist;
    vector<size_t> in_dist;

    degree_distribution(g,
        _distribution = dist,
        _out_distribution = out_dist,
        _in_distribution = in_dist);

    cout << "degree distribution: ";
    copy(dist.begin(), dist.end(), ostream_iterator<size_t>(cout, " "));
    cout << "\n";

    cout << "out distribution: ";
    copy(out_dist.begin(), out_dist.end(), ostream_iterator<size_t>(cout, " "));
    cout << "\n";

    cout << "in distribution: ";
    copy(in_dist.begin(), in_dist.end(), ostream_iterator<size_t>(cout, " "));
    cout << "\n";

    return 0;
}
