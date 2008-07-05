
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_STORAGE_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_STORAGE_HPP

#include <boost/graphs/adjacency_list/storage_traits.hpp>


// Vertex storage types
// We can store vertices in almost anything conceivable. These are the default
// options for storing. Note that we could expand this set to also include
// multi stores and hash stores as well. The differences aren't so great. In
// fact, we could put just about anything here as long as we can bind it to
// a couple of common interfaces.
#include <boost/graphs/adjacency_list/vs/vertex_vector.hpp>
#include <boost/graphs/adjacency_list/vs/vertex_list.hpp>
#include <boost/graphs/adjacency_list/vs/vertex_set.hpp>
#include <boost/graphs/adjacency_list/vs/vertex_map.hpp>

// Edge storage types
// Fortunately, there are really only two types of edge storage, sets and
// multisets. Note, however, that these can be implemented in a couple of
// different ways (e.g., vector or list as a multiset).
#include <boost/graphs/adjacency_list/es/edge_vector.hpp>
#include <boost/graphs/adjacency_list/es/edge_list.hpp>
#include <boost/graphs/adjacency_list/es/edge_set.hpp>
#include <boost/graphs/adjacency_list/es/edge_multiset.hpp>

// Vertex edge storage types
// The vertex edge storage types define the actual adjacency list component
// of each vertex. Like above, there aren't actually too many types of vertex
// edge storage since, for example, mapped containers don't make a lot of sense.
// However, vectors, lists, and sets (both single and multi) work just fine.
#include <boost/graphs/adjacency_list/ves/vertex_edge_vector.hpp>
#include <boost/graphs/adjacency_list/ves/vertex_edge_list.hpp>
#include <boost/graphs/adjacency_list/ves/vertex_edge_set.hpp>

#endif
