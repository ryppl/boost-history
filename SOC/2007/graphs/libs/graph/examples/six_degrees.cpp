// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <vector>
#include <map>

#include <boost/graph/dijkstra_shortest_paths.hpp>

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

    // keep the number of vertices for later...
    size_t n = num_vertices(g);

    // The weight map records the weight of each edge. Since the movie
    // graph is naturally unweighted (and has no weight property for the
    // edges), we are "faking" it by creating it as an external property
    // with all weights initially set to 1.
    MovieWeightMap weights = get(&Performance::weight, g);

    // The distance map records the shortest distance from the source to
    // the the vertex represented at that index.
    typedef vector<int> DistanceMap;
    DistanceMap distances(n);

    // The predeceessor map records, for the vertex at each index, the
    // predecessor (or parent) in the shortest-path tree. By iterating
    // from predecessor to predecessor, we can find the shortest path
    typedef vector<Vertex> PredecessorMap;
    PredecessorMap predecessors(n);

    dijkstra_shortest_paths(g, u,
			    weight_map(weights).
			    distance_map(&distances[0]).
			    predecessor_map(&predecessors[0]));


    // we're going to need the actor and movie names for this...
    ActorNameMap names = get(&Actor::name, g);
    MovieNameMap movies = get(&Performance::movie, g);

    // print the movies in which the actors appear by iterating over
    // the elements in the predecessor map
    while(v != u) {
	Vertex p = predecessors[v];

	// what are our two names...
	string from = names[v];
	string to = names[p];

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
	string movie = movies[e];

	// print out the path
	cout << from << " with " << to << " in '" << movie << "'\n";

	// move to the next predecessor
	v = p;
    }

    return 0;
}
