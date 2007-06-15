// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

// std includes
#include <iostream>
#include <vector>
#include <map>

// boost includes
#include <boost/graph/dijkstra_shortest_paths.hpp>

// example includes
#include "movies.hpp"

using namespace std;
using namespace boost;

int
main(int argc, char *argv[])
{
    string src = "Kevin Bacon";
    string tgt;

    // get the source and target nodes from the graph
    if(argc < 2) {
	cerr << "usage: actor_paths actor [actor] < movies";
	return -1;
    }
    else if(argc == 2) {
	tgt = argv[1];
    }
    else {
	src = argv[1];
	tgt = argv[2];
    }

    // instantiate the graph and the actor map
    Graph g;
    ActorMap actors;

    // read the graph from stdin
    build_movie_graph(cin, g, actors);

    // get the source and target vertices
    Vertex u = find_actor_vertex(g, actors, src);
    Vertex v = find_actor_vertex(g, actors, tgt);
    if(u == Graph::null_vertex()) {
	cerr << "could not find actor " << src << "\n";
	return -1;
    }
    if(v == Graph::null_vertex()) {
	cerr << "could not find actor " << tgt << "\n";
	return -1;
    }

    // get the parents map so we can record them
    // during a breadth first search
    ActorParentMap parents = get(&Actor::parent, g);

    breadth_first_search(
	g, u,
	visitor(
	    make_bfs_visitor(
		record_predecessors(parents, on_tree_edge())
		)
	    )
	);

    // print the movies in which the actors appear by iterating over
    // the elements in the predecessor map
    while(v != u) {
	Vertex p = parents[v];

	// what are our two names...
	string from = g[v].name;
	string to = g[p].name;

	// what edge does (v,p) exist on. unforunately, because this isn't
	// an adjacency matrix, we actually have to search the outgoing (or
	// incoming?) edges of v to find p, and get the edge associated with
	// that performance.
	Edge e;
	Graph::out_edge_iterator i, j;
	for(tie(i, j) = out_edges(v, g); i != j; ++i) {
	    if(target(*i, g) == p) {
		e = *i;
		break;
	    }
	}

	// what's the movie name?
	string movie = g[e].name;

	// print out the path
	cout << from << " starred with " << to << " in '" << movie << "'\n";

	// move to the next predecessor
	v = p;
    }

    return 0;
}
