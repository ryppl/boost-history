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

// This visitor is responsible for recording (setting) the bacon numbers
// of actors in the movie graph.
template <typename BaconMap>
struct BaconNumberRecorder : public bfs_visitor<>
{
    BaconNumberRecorder(BaconMap b)
	: bacons(b)
    {}

    // The tree_edge() method is invoked when a vertex v is discovered
    // while exploring an edge (u,v). With respect to a BFS, this means
    // that v is in the next higher level (or depth) than u. Therefore,
    // the Kevin Bacon number of v should be one more than that of u.
    template <typename Edge, typename Graph>
    void tree_edge(Edge e, const Graph& g) const
    {
	typedef typename Graph::vertex_descriptor Vertex;

	// get the source and target vertices of this tree edge.
	Vertex
	    u = source(e, g),
	    v = target(e, g);

	// set the bacon number to 1 + u's bacon number
	bacons[v] = bacons[u] + 1;
    }

    BaconMap bacons;
};

// This is just a convenience function so we can call a function rather than
// explicitly instantiate the visitor type. It makes it more "action-oriented".
template <typename BaconMap>
BaconNumberRecorder<BaconMap> record_bacon_numbers(BaconMap b)
{
    return BaconNumberRecorder<BaconMap>(b);
}

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
    ActorIndexMap indices = get(&Actor::index, g);
    ActorNameMap names = get(&Actor::name, g);
    ActorBaconMap bacons = get(&Actor::bacon_number, g);

    // pick a starting vertex (kevin bacon, obviously) and set his
    // number to 0.
    Vertex kevin = actors["Kevin Bacon"];
    bacons[kevin] = 0;

    // run a breadth-first search on the graph and record
    // the kevin bacon numbers for each actor
    breadth_first_search(g, kevin,
			 // named parameters
			 vertex_index_map(indices)
			 .visitor(record_bacon_numbers(bacons))
	);

    // just run over the vertices and print the back numbers
    Graph::vertex_iterator i, j;
    for(tie(i, j) = vertices(g); i != j; ++i) {
	cout << g[*i].bacon_number << " : " << g[*i].name << "\n";
    }

    return 0;
}

