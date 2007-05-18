// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

// std includes
#include <iostream>
#include <string>
#include <map>

// boost includes
#include <boost/tokenizer.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/topological_sort.hpp>

using namespace std;
using namespace boost;

struct Target
{
    int index;
    string name;
};

typedef directed_graph<Target> Graph;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_descriptor Edge;
typedef property_map<Graph::type, int Target::*>::type TargetIndexMap;
typedef property_map<Graph::type, string Target::*>::type TargetNameMap;

typedef map<string, Vertex> TargetMap;

Vertex
add_target(Graph& g, TargetMap& targets, const string& name)
{
    // try inserting the actors name into the actors map
    Vertex v;
    TargetMap::iterator it;
    bool inserted;
    tie(it, inserted) = targets.insert(make_pair(name, Vertex()));
    if(inserted) {
	// create a vertex for the name
	v = add_vertex(g);

	// associate the vertex with the name
	it->second = v;

	// configure the vertex properties
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

// Look very carefully at the add_edge() call in this function and
// you'll notice that it switches the ordering of the vertices in
// the edge that's being created. Unfortunately, the problem requires
// that we model the direction of edges as "used-by", although our
// input format is more of a "dependency" listing. This isn't really
// a problem as long as we recognize the difference.
Edge
add_dependency(Graph &g, Vertex u, Vertex v)
{
    Edge e;
    bool inserted;
    tie(e, inserted) = add_edge(v, u, g);
    return e;
}

int
main(int argc, char* argv[])
{
    typedef list<Vertex> BuildOrder;
    typedef boost::char_separator<char> separator;
    typedef boost::tokenizer<separator> tokenizer;

    Graph g;
    TargetMap targets;

    for(string line; getline(cin, line); ) {
	// skip comment and blank lines
	if(line[0] == '#' || line.empty()) {
	    continue;
	}

	// split the string on the dependency
	size_t index = line.find_first_of(':');
	if(index == string::npos) {
	    continue;
	}
	string target(line, 0, index);
	string deps(line, index + 1);

	// add the target to the build graph
	Vertex u = add_target(g, targets, target);

	// tokenize the dependencies
	separator sep(" \t");
	tokenizer tok(deps, sep);
	tokenizer::iterator i = tok.begin(), j = tok.end();
	for( ; i != j; ++i) {
	    string dep = *i;

	    // add this dependency as a target
	    Vertex v = add_target(g, targets, dep);

	    // add the edge
	    add_dependency(g, u, v);
	}
    }

    // we need to create a vertex index map for the graph before
    // we can generate the build order
    TargetIndexMap indices = get(&Target::index, g);

    // the build order is just a 
    BuildOrder order;
    topological_sort(g, front_inserter(order),
		     // named parameters
		     vertex_index_map(indices)
	);

    // write out the build order...
    BuildOrder::iterator i = order.begin(), j = order.end();
    for( ; i != j; ++i) {
	cout << g[*i].name << "\n";
    }
}
