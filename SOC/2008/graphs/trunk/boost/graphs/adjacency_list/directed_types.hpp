
#ifndef BOOST_GRAPHS_ADJLIST_DIRECTED_TYPES_HPP
#define BOOST_GRAPHS_ADJLIST_DIRECTED_TYPES_HPP

// Vertex stores
#include <boost/graphs/adjacency_list/vertex_vector.hpp>
#include <boost/graphs/adjacency_list/vertex_list.hpp>
#include <boost/graphs/adjacency_list/vertex_set.hpp>
#include <boost/graphs/adjacency_list/vertex_map.hpp>

// Edge stores
#include <boost/graphs/adjacency_list/edge_vector.hpp>
#include <boost/graphs/adjacency_list/edge_list.hpp>
#include <boost/graphs/adjacency_list/edge_set.hpp>

// Edges store components
#include <boost/graphs/adjacency_list/out_vector.hpp>
#include <boost/graphs/adjacency_list/out_list.hpp>
#include <boost/graphs/adjacency_list/out_set.hpp>
#include <boost/graphs/adjacency_list/out_iterator.hpp>
#include <boost/graphs/adjacency_list/in_vector.hpp>
#include <boost/graphs/adjacency_list/in_list.hpp>
#include <boost/graphs/adjacency_list/in_set.hpp>
#include <boost/graphs/adjacency_list/in_iterator.hpp>

// Vertex and Edge components
#include <boost/graphs/adjacency_list/directed_vertex.hpp>
#include <boost/graphs/adjacency_list/directed_edge.hpp>

// Adjacency components
#include <boost/graphs/adjacency_list/adjacency_iterator.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

/**
 * This class is a metafunction that generates all of the types needed to
 * implement a directed graph.
 */
template <typename VertexLabel, typename EdgeLabel, typename VertexStore, typename EdgeStore>
struct directed_types
{
    // Start by generating all of the descriptors.
    typedef typename VertexStore::vertex_descriptor vertex_descriptor;
    typedef typename EdgeStore::out_descriptor out_descriptor;
    typedef typename EdgeStore::in_descriptor in_descriptor;

    // Generate edges. Iterators are last.
    typedef directed_edge<out_descriptor, in_descriptor> edge_descriptor;

    // Generate the out store and related types
    typedef typename EdgeStore::template out_store<vertex_descriptor, EdgeLabel>::type out_store;
    typedef typename out_store::size_type out_edges_size_type;
    typedef typename out_store::size_type edges_size_type;

    // Generate the in store and related types
    typedef typename EdgeStore::template in_store<vertex_descriptor>::type in_store;
    typedef typename in_store::size_type in_edges_size_type;

    // Generate the incident edge size type. It should be the larger of the two,
    // but for now it's just the out edges.
    typedef out_edges_size_type incident_edges_size_type;

    // Generate the vertex, its store, and related types.
    typedef directed_vertex<VertexLabel, out_store, in_store> vertex_type;
    typedef typename VertexStore::template store<vertex_type>::type vertex_store;
    typedef typename vertex_store::size_type vertices_size_type;
    typedef typename vertex_store::vertex_iterator vertex_iterator;
    typedef typename vertex_store::vertex_range vertex_range;
    typedef typename VertexStore::key_type vertex_key;

    // Generate a bunch of iterators
    typedef basic_out_iterator<vertex_type, edge_descriptor> out_edge_iterator;
    typedef std::pair<out_edge_iterator, out_edge_iterator> out_edge_range;
    typedef basic_in_iterator<typename in_store::iterator, edge_descriptor> in_edge_iterator;
    typedef std::pair<in_edge_iterator, in_edge_iterator> in_edge_range;
    typedef directed_edge_iterator<vertex_store, edge_descriptor> edge_iterator;
    typedef std::pair<edge_iterator, edge_iterator> edge_range;

    // Generate incident edge iterators as out edge iterators - not a
    // combination of in and out edges.
    typedef out_edge_iterator incident_edge_iterator;
    typedef out_edge_range incident_edge_range;

    // Generate adjacent vertx iterators over the incidence iterator
    typedef adjacency_iterator<incident_edge_iterator> adjacent_vertex_iterator;
    typedef std::pair<adjacent_vertex_iterator, adjacent_vertex_iterator> adjacent_vertex_range;
};

} } } /* namespace boost::graphs::adjacency_list */

#endif
