
#include <iostream>
#include <string>
#include <map>

#include <boost/tokenizer.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include "../../boost/undirected_graph.hpp"

using namespace std;
using namespace boost;

// The Actor struct contains properties about the actors in the
// movie graph.
struct Actor
{
    string name;
    unsigned bacon_number;
};

// The Performance struct describes information about the performance
// of two actors - specifically, what movie and year they performed
// together in.
struct Performance
{
    string movie;
};

// The Graph data structure is an undirected graph with vertices
// being represented by actors and edges, their co-performances.
typedef undirected_graph<Actor, Performance> Graph;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_descriptor Edge;

// These property maps index information in the Actor and Performance
// structures, respectively. They are used to access specific pieces
// of information inside the graph.
typedef property_map<Graph::type, string Actor::*>::type ActorNameMap;
typedef property_map<Graph::type, unsigned Actor::*>::type ActorBaconMap;
typedef property_map<Graph::type, string Performance::*>::type MovieNameMap;

// we use an extra map to help dynamically populate the graph.
// this maps actor names to the vertices that they're inserted as
typedef map<string, Vertex> ActorMap;

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
BaconNumberRecorder<BaconMap> record_bacons(BaconMap b)
{
    return BaconNumberRecorder<BaconMap>(b);
}

static Vertex
add_actor(Graph &g, ActorMap &actors, string &name)
{
    // get the actor name map associated with the graph
    ActorNameMap actor_names = get(&Actor::name, g);

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
	actor_names[v] = name;
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
    // get the movie name map associated with the graph
    MovieNameMap movie_names = get(&Performance::movie, g);

    Edge e;
    bool inserted;
    tie(e, inserted) = add_edge(u, v, g);
    if(inserted) {
	movie_names[e] = movie;
    }
}

int
main(int argc, char *argv[])
{
    // instantiate the graph
    Graph g;

    // instanteate the graph-construction helping map
    ActorMap actors;

    // pull all of the data from std in.
    for(string line; getline(cin, line); ) {
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

    // get the bacon number map associated with the graph
    ActorNameMap names = get(&Actor::name, g);
    ActorBaconMap bacons = get(&Actor::bacon_number, g);

    // pick a starting vertex (kevin bacon, obviously) and set his
    // number to 0.
    Vertex kevin = actors["Kevin Bacon"];
    bacons[kevin] = 0;

    // run a breadth-first search on the graph and record
    // the kevin bacon numbers for each actor
    breadth_first_search(g, kevin, visitor(record_bacons(bacons)));

    // just run over the vertices and print the related info
    Graph::vertex_iterator i, j;
    for(tie(i, j) = vertices(g); i != j; ++i) {
	cout << bacons[*i] << " : " << names[*i] << "\n";
    }
}

