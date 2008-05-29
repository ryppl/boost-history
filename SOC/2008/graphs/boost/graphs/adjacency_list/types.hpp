
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_TYPES_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_TYPES_HPP

// These are "types" of graphs that pretty much define the basic structural
// management features for adjacency lists. Because the actual interface of
// the data structure depends primarily on the directedness of its edges,
// all of the basic components are defined within these types. In fact, we
// might think of all of these as different versions of the same graph.

// Creating new types of graphs really just means copying and modifying one
// of the existing graph types. It might be a lot of work, but it should
// actually pay off if you really need to do something new.


// Notes..
// One of the "hard" things to think about is the generalization of outgoing
// or incident edges between undirected and directed graphs. This distinction
// is important since we can usually treat the two types of graphs in a similar
// manner. Getting the edges of an undirected vertex means getting all incident
// edges. Getting the edges of a directed vertex means getting the outgoing
// edges. For generic algorithms, there needs to be a concept "default edge
// type" concept that provides either the incident or out edges of the vertex.
//
// It might just be easy enough to build an "incident" edge type and accessor
// for directed vertices that simply returns or something like that.

namespace boost {
namespace graphs {
namespace adj_list {

} /* adj_list */
} /* graphs */
} /* boost */


#include <boost/graphs/adjacency_list/undirected.hpp>

#endif
