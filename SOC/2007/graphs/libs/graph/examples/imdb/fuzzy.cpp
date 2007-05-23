// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

// std includes
#include <utility>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <tr1/unordered_map>

// boost includes
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_as_matrix.hpp>

using namespace std;
using namespace boost;

struct Actor;
typedef shared_ptr<Actor> ActorPtr;

struct Role;
typedef shared_ptr<Role> RolePtr;
typedef vector<RolePtr> RoleVector;

struct Role
{
    string production;
    string year;
    string role;
    string key;
    unsigned media;
};

struct Actor
{
    string name;
    RoleVector roles;
};

// these flags are used to filter selections when constructing
// the graph - they also indicate a type of media.
static const unsigned MOVIE = 0x01;
static const unsigned VIDEO = 0x02;
static const unsigned SERIES = 0x04;
static const unsigned SHOW = 0x08;

// graph structures
struct VertexProperty;
struct EdgeProperty;

// typedef undirected_graph<VertexProperty, EdgeProperty> Graph;
typedef adjacency_list<vecS, vecS, undirectedS, VertexProperty, EdgeProperty> Graph;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_descriptor Edge;

// various maps to help help us associate names with movies
// and actors.
typedef map<string, Vertex> NameMap;

// The VertexProperty structure encodes additonal information about
// a vertex in the graph. Rather than trying to build a more common
// co-citation graph (in which people are connected by a production)
// we are building a multi-graph that contains both people and
// productions and vertices. This is definitely a bipartite graph.
//
// Note that rather than have this property point to the parsed
// actors and movie objects, we duplicate much of that data here.
// It's a size optimization and could potentially halve the amount
// of memory required.
//
// Interestingly, this graph has some interesting semantics. The
// adjacent vertices of an actor vertex represent his or her
// filmography while adjacent vertices of of a movie vertex
// represent its cast (at least the "important" members thereof).
struct VertexProperty
{
    // properties needed to support different algorithms
    int index;
    float distance;
    Vertex parent;

    // domain-specific properties
    enum { ACTOR, MOVIE } type;
    string name;
};

// The EdgeProperty structure represents roles played by actors
// in various productions. I don't really need to keep track of
// this stuff, but it makes the program a little more interesting
// (and consumes more memory).
struct EdgeProperty
{
    // properties required for different algorithms
    float weight;

    // domain-specific properties
    string role;
};

// property maps
typedef property_map<Graph, int VertexProperty::*>::type VertexIndexMap;
typedef property_map<Graph, float VertexProperty::*>::type VertexDistanceMap;
typedef property_map<Graph, Vertex VertexProperty::*>::type VertexParentMap;
typedef property_map<Graph, float EdgeProperty::*>::type EdgeWeightMap;

RolePtr
parse_movie(const string& str)
{
    unsigned media = 0;

    // look for the first left paren because it definitely marks the
    // end of the movie name
    size_t left = str.find_first_of('(');
    if(left == string::npos) {
	return RolePtr();
    }
    string name(str, 0, left - 1);

    // good to know... if there's no leading quotes then this is a movie.
    // if there are quotes, we automatically know that its a tv series or
    // mini series.
    if(name[0] != '\"') {
	// this might be a movie, but it could be a tv show or a straight
	// to video movie (like a porn)
	media = MOVIE;
    }
    else {
	// this is definitely  series - it could be a mini series
	media = SERIES;
    }

    // we should now be in good shape to get the year
    size_t right = str.find_first_of(')', left + 1);
    if(right == string::npos) {
	return RolePtr();
    }
    string year(str, left + 1, right - left - 1);

    // if we believe that this is a movie, then we should look for
    // the next paren after the date since it should contain either
    // (TV), (V), or be absent
    bool archive = false;
    if(media == MOVIE) {
	left = str.find_first_of('(', right + 1);
	if(left != string::npos) {
	    char c = str[left + 1];
	    if(c == 'T') {
		media = SHOW;
	    }
	    else if(c == 'V') {
		// this might also be a video game so beware...
		media = VIDEO;
	    }
	    else if(c == 'a') {
		// this is probably archive footage, which means
		// that we should safely ignore it.
		archive = true;
	    }
	    else if(str[left + 1] == 'u') {
		// the role is uncredited - leaving this in just makes
		// for more fun...
	    }
	}
    }

    // if we found archive footage, just bail now..
    if(archive) {
	return RolePtr();
    }

    // can we find the role?
    string who;
    left = str.find_first_of('[', right + 1);
    if(left != string::npos) {
	right = str.find_first_of(']', left + 1);
	if(right != string::npos) {
	    who = str.substr(left + 1, right - left - 1);
	}
    }

    // generate the return role
    RolePtr role(new Role());
    role->production = name;
    role->year = year;
    role->role = who;
    role->key = name + " (" + year + ")";
    role->media = media;
    return role;
}

Vertex
add_actor(Graph& g, NameMap& actors, const ActorPtr& actor)
{
    // try inserting the actors name into the actors map
    Vertex v;
    NameMap::iterator it;
    bool inserted;
    const string& name = actor->name;
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
	g[v].type = VertexProperty::ACTOR;
    }
    else {
	// otherwise, the name is already in the map, so just
	// return the iterator associated with it
	v = it->second;
    }

    return v;
}

Vertex
add_movie(Graph& g, NameMap& movies, const RolePtr& role)
{
    // try inserting the actors name into the actors map
    Vertex v;
    NameMap::iterator it;
    bool inserted;
    const string& key = role->key;
    tie(it, inserted) = movies.insert(make_pair(key, Vertex()));
    if(inserted) {
	// if the name was actually inserted, then add a new vertex
	// to the graph, configure the entry in the map, and set
	// the actors name
	v = add_vertex(g);
	it->second = v;

	// configure the vertex
	g[v].index = num_vertices(g) - 1;
	g[v].name = key;
	g[v].type = VertexProperty::MOVIE;
    }
    else {
	// otherwise, the name is already in the map, so just
	// return the iterator associated with it
	v = it->second;
    }

    return v;
}

// The computation of a "fuzzy" six degrees only really varies in this function.
// Essentially, we're going to compute weights for the edges based on the popularity
// or more appropriately, the "I've been around..." property. Essentially, the edge
// is weighted by the number of movies the actor has been in - or the reciprical
// of that value. Essentially, it should be easier to follow links through more
// popular actors.
//
Edge
add_role(Graph& g, Vertex actor, Vertex movie, const RolePtr& role, size_t weight)
{
    Edge e;
    bool inserted;
    tie(e, inserted) = add_edge(actor, movie, g);
    if(inserted) {
	g[e].weight = 1.0 / (float)(weight * weight);
	g[e].role = role->role;
    }
    return e;
}

int
main(int argc, char* argv[])
{
    // set the current filter to movies only
    unsigned filter = MOVIE;
    size_t minimum = 1;

    // collect some stats on numbers of people/roles
    typedef pair<size_t, size_t> Stats;
    Stats role_stats(0, 0);
    Stats actor_stats(0, 0);

    // Graph data;
    Graph g;
    NameMap actors;
    NameMap movies;

    cerr << "building graph from input files\n";

    for(int a = 1; a < argc; ++a) {
	ifstream is(argv[a]);
	if(!is) {
	    cerr << "can't open file '" << argv[a] << "'\n";
	    continue;
	}

	// parse out the data - note, reset the line count each time
	// but not the actor or movies maps
	ActorPtr actor;
	unsigned count = 0;
	for(string line; getline(is, line); ++count) {
	    if(count % 100000 == 0) {
		cerr << "... line: " << count
		     << ", actors: " << actors.size()
		     << ", movies: " << movies.size()
		     << "\n";
	    }

	    if(line.empty()) {
		// we're going to create a simple graph file such that each
		// line contains the name of an actor and a role the actor
		// participates in. This is actually an interesting type of
		// bigraph - actors are connected to movies (only) - i hope.

		if(actor->roles.size() >= minimum) {
		    // add the actor the graph
		    Vertex u = add_actor(g, actors, actor);

		    RoleVector::iterator
			i = actor->roles.begin(),
			j = actor->roles.end();
		    for( ; i != j; ++i) {
			RolePtr role = *i;

			// add the movie as a vertex
			Vertex v = add_movie(g, movies, role);

			// add the actor to the cast (or the movie to the
			// the actor's filmography).
			add_role(g, u, v, role, actor->roles.size());

			++role_stats.first;
		    }
		    ++actor_stats.first;
		}
		++actor_stats.second;
	    }
	    else if(line[0] == '\t') {
		// this line contains a role in a movie, so we're going strip
		// the whitespace and parse it
		RolePtr role = parse_movie(trim_copy(line));

		// does the role match the filter?
		if(role && (role->media & filter)) {
		    // store the role for the actor
		    if(actor) {
			actor->roles.push_back(role);
		    }
		}
		++role_stats.second;
	    }
	    else {
		// grab the actor's name - it's the first tab separated
		// column in the data
		size_t index = line.find_first_of('\t');
		string name(line, 0, index);
		string movie(line, index + 1);

		// this is an actor line so we should instantiate and configure
		// it now...
		actor = ActorPtr(new Actor());
		actor->name = name;

		// the actors first (latest?) role should be the remainder of
		// this line...
		RolePtr role = parse_movie(trim_copy(movie));

		// does this movie match the filter? if so, then we're going
		// to keep it... otherwise, delete it
		if(role && (role->media & filter)) {
		    actor->roles.push_back(role);
		}
		++role_stats.second;
	    }
	}
    }

    cerr << "finished building graph\n";
    cerr << "actors: " << actor_stats.first << " / " << actor_stats.second << "\n";
    cerr << "roles: " << role_stats.first << " / " << role_stats.second << "\n";

    // start accumulating property maps for dijkstra's algorithm.
    // these references never change even though the contents of them might.
    VertexIndexMap indices = get(&VertexProperty::index, g);
    VertexDistanceMap distances = get(&VertexProperty::distance, g);
    VertexParentMap parents = get(&VertexProperty::parent, g);
    EdgeWeightMap weights = get(&EdgeProperty::weight, g);

    while(cin) {
	string src, dst;

	cout << "enter the first actor: ";
	getline(cin, src);
	if(!cin) break;		// catch ^D and exit

	NameMap::iterator vsrc = actors.find(src);
	if(vsrc == actors.end()) {
	    cout << "could not find actor '" << src << "'\n";
	    continue;
	}
	Vertex u = vsrc->second;

	cout << "enter the second actor: ";
	getline(cin, dst);
	if(!cin) break;		// catch ^D and exit

	NameMap::iterator vdst = actors.find(dst);
	if(vdst == actors.end()) {
	    cout << "could not find actor '" << dst << "'\n";
	    continue;
	}
	Vertex v = vdst->second;

	// run the algorithm... (this might take a while)
	// note that the pseudo code claims that this will
	// reinitialize distances and predecessors
	//
	// TODO - since all edge weights are 1, we could conceivably
	// use BFS to solve this problem - it would pobably be much more
	// efficient.
	cout << "computing shortest path (please wait)... " << "\n";
	dijkstra_shortest_paths(g, u,
				// named parameters
				vertex_index_map(indices)
				.distance_map(distances)
				.predecessor_map(parents)
				.weight_map(weights)
	    );


	// print the movies in which the actors appear by iterating over
	// the elements in the predecessor map
	int step = 1;
	while(v != u) {
	    // get the parent movie and its parent actor
	    Vertex m = parents[v];	// this _must_ be a movie
	    Vertex p = parents[m];	// and this _must_ be the next closest actor

	    // get the edges associated with those links
	    Edge from_edge, to_edge;
	    bool from_found, to_found;
	    tie(from_edge, from_found) = edge(v, m, g);
	    tie(to_edge, to_found) = edge(m, p, g);

	    // get the actor names and the movie name
	    string from = g[v].name;
	    string to = g[p].name;
	    string in = g[m].name;

	    // find the role names (for completeness)
	    string from_role = g[from_edge].role;
	    string to_role = g[to_edge].role;

	    if(from_role.empty()) from_role = "unlisted";
	    if(to_role.empty()) to_role = "unlisted";

	    cout << step++ << ". "
		 << from << " [as " << from_role << "]"
		 << " stars in '" << in << "' with "
		 << to << " [as " << to_role << "]"
		 << "\n";

	    // move to the next predecessor (actually up by 2)
	    v = p;
	}
    }

    return 0;
}
