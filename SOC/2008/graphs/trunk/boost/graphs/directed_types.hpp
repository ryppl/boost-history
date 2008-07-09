
#ifndef DIRECTED_TYPES_HPP
#define DIRECTED_TYPES_HPP

// Vertex stores
#include <boost/graphs/vertex_vector.hpp>
#include <boost/graphs/vertex_list.hpp>
#include <boost/graphs/vertex_set.hpp>
#include <boost/graphs/vertex_map.hpp>

// Edge stores
#include <boost/graphs/edge_vector.hpp>
#include <boost/graphs/edge_list.hpp>
#include <boost/graphs/edge_set.hpp>

// Edges store components
#include <boost/graphs/out_vector.hpp>
#include <boost/graphs/out_list.hpp>
#include <boost/graphs/out_set.hpp>
#include <boost/graphs/out_iterator.hpp>
#include <boost/graphs/in_vector.hpp>
#include <boost/graphs/in_list.hpp>
#include <boost/graphs/in_set.hpp>
#include <boost/graphs/in_iterator.hpp>

// Vertex and Edge components
#include <boost/graphs/directed_vertex.hpp>
#include <boost/graphs/directed_edge.hpp>

// Adjacency components
#include <boost/graphs/adjacency_iterator.hpp>

/**
 * This class is a metafunction that generates all of the types needed to
 * implement a directed graph.
 */
template <typename VertexProps, typename EdgeProps, typename VertexStore, typename EdgeStore>
class directed_types
{
    // Start by generating all of the descriptors.
    typedef typename VertexStore::vertex_descriptor vertex_descriptor;
    typedef typename EdgeStore::out_descriptor out_descriptor;
    typedef typename EdgeStore::in_descriptor in_descriptor;

    // Generate edges. Iterators are last.
    typedef directed_edge<out_descriptor, in_descriptor> edge_descriptor;

    // Generate the out store and related types
    typedef typename EdgeStore::template out_store<vertex_descriptor, EdgeProps> out_store;
    typedef typename out_store::size_type out_edges_size_type;
    typedef typename out_store::size_type edges_size_type;
    typedef basic_out_iterator<out_store, edge_descriptor> out_edge_iterator;
    typedef std::pair<out_edge_iterator, out_edge_iterator> out_edge_range;

    // Generate the in store and related types
    typedef typename EdgeStore::template in_store<vertex_descriptor> in_store;
    typedef typename in_store::size_type in_edge_size_type;
    typedef basic_in_iterator<typename in_store::iterator, edge_descriptor> in_edge_iterator;
    typedef std::pair<in_edge_iterator, in_edge_iterator> in_edge_range;

    // Generate the vertex, its store, and related types.
    typedef directed_vertex<VertexProps, out_store, in_store> vertex_type;
    typedef typename VertexStore::template store<vertex_type>::type vertex_store;
    typedef typename vertex_store::size_type vertices_size_type;
    typedef typename vertex_store::vertex_iterator vertex_iterator;
    typedef typename vertex_store::vertex_range vertex_range;
    typedef typename vertex_store::vertex_key vertex_key;

    // Generate the edge iterators as far back as possible...
    typedef directed_edge_iterator<vertex_store, edge_descriptor> edge_iterator;
    typedef std::pair<edge_iterator, edge_iterator> edge_range;
};

#endif
