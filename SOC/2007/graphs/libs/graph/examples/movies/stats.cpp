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

#include "movies.hpp"

using namespace std;
using namespace boost;

int
main(int argc, char *argv[])
{
    Graph g;
    ActorMap actors;

    // build the movie graph from std input
    build_movie_graph(cin, g, actors);

    // compute the degree distribution
    vector<size_t> dist;
    degree_distribution(g, dist);
    copy(dist.begin(), dist.end(),
	 ostream_iterator<size_t>(cout, " "));
    cout << "\n";

    return 0;
}
