// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

// std includes
#include <iostream>

// boost includes
#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>

// example includes
#include "movies.hpp"

using namespace std;
using namespace boost;

int
main(int argc, char *argv[])
{
    // instantiate the graph
    Graph g;

    // instantiate the graph-construction helping map
    ActorMap actors;

    // read the graph from stdin
    build_movie_graph(cin, g, actors);

    // get the bacon number map associated with the graph
    ActorDistanceMap dists = get(&Actor::distance, g);

    // pick a starting vertex (kevin bacon, obviously) and set his
    // number to 0.
    Vertex kevin = actors["Kevin Bacon"];
    dists[kevin] = 0;

    // run a breadth-first search on the graph and record
    // the kevin bacon numbers for each actor
    breadth_first_search(
        g, kevin,
        visitor(
            make_bfs_visitor(record_distances(dists, on_tree_edge()))
            )
    );

    // just run over the vertices and print the back numbers
    Graph::vertex_iterator i, j;
    for(tie(i, j) = vertices(g); i != j; ++i) {
    cout << g[*i].distance << " : " << g[*i].name << "\n";
    }

    return 0;
}

