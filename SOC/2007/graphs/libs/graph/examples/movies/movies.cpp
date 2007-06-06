// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

// std includes
#include <iostream>

// boost includes
#include <boost/tokenizer.hpp>

// example includes
#include "movies.hpp"

using namespace std;
using namespace boost;

static Vertex
add_actor(Graph &g, ActorMap &actors, string &name)
{
    // try inserting the actors name into the actors map
    Vertex v;
    ActorMap::iterator it;
    bool inserted;
    tie(it, inserted) = actors.insert(make_pair(name, Vertex()));
    if(inserted) {
	// if the name was actually inserted, then add a new vertex
	// to the graph, configure the entry in the map, and set
	// the actors name
	v = add_vertex(g);
	it->second = v;

	// configure the vertex
	g[v].index = num_vertices(g) - 1;
	g[v].name = name;
    }
    else {
	// otherwise, the name is already in the map, so just
	// return the iterator associated with it
	v = it->second;
    }

    return v;
}

static Edge
add_performance(Graph &g, Vertex u, Vertex v, string const& movie)
{
    Edge e;
    bool inserted;
    tie(e, inserted) = add_edge(u, v, g);
    if(inserted) {
	g[e].weight = 1;
	g[e].movie = movie;
    }
    return e;
}

void
build_movie_graph(istream& is, Graph& g, ActorMap& actors)
{
    // pull all of the data from std in.
    for(string line; getline(is, line); ) {
	// skip any comment or blank lines
	if(line[0] == '#' || line.empty()) {
	    continue;
	}

	// tokenize the string
	char_delimiters_separator<char> sep(false, "", ";");
	tokenizer<> tok(line, sep);
	tokenizer<>::iterator i = tok.begin();
	
	// grab the first actor
	string first = *i++;
	string second = *i++;
	string movie = *i++;

	// get the vertices associated with the actors (adding them
	// to the graph if necessary).
	Vertex
	    u = add_actor(g, actors, first),
	    v = add_actor(g, actors, second);

	// create an edge (performance) linking the actors
	add_performance(g, u, v, movie);
    }
}

Vertex
find_actor_vertex(const Graph& g, const ActorMap& actors, const std::string& name)
{
    Vertex v = Graph::null_vertex();
    ActorMap::const_iterator i = actors.find(name);
    if(i != actors.end()) {
	v = i->second;
    }
    return v;
}
