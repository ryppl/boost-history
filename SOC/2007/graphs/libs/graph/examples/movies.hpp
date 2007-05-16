// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOVIES_HPP
#define MOVIES_HPP

#include <iosfwd>
#include <string>
#include <map>

#include <boost/graph/undirected_graph.hpp>

// The Actor struct contains properties about the actors in the
// movie graph.
struct Actor
{
    std::string name;
    unsigned bacon_number;
};

// The Performance struct describes information about the performance
// of two actors - specifically, what movie and year they performed
// together in.
//
// Note that the edge property type contains a weight. While a performance
// wouldn't typically be weighted (it doesn't mean anything), we need a
// weight map to work with some algorithms.
struct Performance
{
    Performance()
	: weight(1)
	, movie()
    {}

    Performance(const Performance& copy)
	: weight(1)
	, movie(copy.movie)
    {}

    unsigned weight;
    std::string movie;
};

// The Graph data structure is an undirected graph with vertices
// being represented by actors and edges, their co-performances.
typedef boost::undirected_graph<Actor, Performance> Graph;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_descriptor Edge;

// These property maps index information in the Actor and Performance
// structures, respectively. They are used to access specific pieces
// of information inside the graph.
typedef boost::property_map<Graph::type, std::string Actor::*>::type ActorNameMap;
typedef boost::property_map<Graph::type, unsigned Actor::*>::type ActorBaconMap;
typedef boost::property_map<Graph::type, std::string Performance::*>::type MovieNameMap;
typedef boost::property_map<Graph::type, unsigned Performance::*>::type MovieWeightMap;

// we use an extra map to help dynamically populate the graph.
// this maps actor names to the vertices that they're inserted as
typedef std::map<std::string, Vertex> ActorMap;

void build_movie_graph(std::istream &, Graph&, ActorMap&);
Vertex find_actor_vertex(const Graph&, const ActorMap&, const std::string&);

#endif
