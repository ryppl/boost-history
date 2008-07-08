
#ifndef UNDIRECTED_TYPES_HPP
#define UNDIRECTED_TYPES_HPP

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
#include <boost/graphs/property_vector.hpp>
#include <boost/graphs/property_list.hpp>
#include <boost/graphs/incidence_vector.hpp>
#include <boost/graphs/incidence_list.hpp>
#include <boost/graphs/incidence_set.hpp>
#include <boost/graphs/incidence_iterator.hpp>

// Vertex and Edge components
#include <boost/graphs/undirected_vertex.hpp>
#include <boost/graphs/undirected_edge.hpp>

// Adjacency components
#include <boost/graphs/adjacency_iterator.hpp>

using namespace std;

/**
 * This class is a giant metafunction that generates the types required to
 * implement an undirected gaph.
 */
template <typename VertexProps, typename EdgeProps, typename VertexStore, typename EdgeStore>
struct undirected_types
{
    // Generate descriptors.
    typedef typename VertexStore::vertex_descriptor vertex_descriptor;
    typedef typename EdgeStore::property_descriptor property_descriptor;
    typedef typename EdgeStore::incidence_descriptor incidence_descriptor;

    // Generate the property store and related types.
    typedef typename EdgeStore::template property_store<EdgeProps, vertex_descriptor>::type property_store;
    typedef typename property_store::size_type edges_size_type;

    // Generate stuff related to edges
    typedef undirected_edge<vertex_descriptor, property_descriptor> edge_descriptor;
    typedef undirected_edge_iterator<property_store, edge_descriptor> edge_iterator;
    typedef std::pair<edge_iterator, edge_iterator> edge_range;

    // Generate the incidence store type.
    typedef typename EdgeStore::template incidence_store<vertex_descriptor>::type incidence_store;
    typedef typename incidence_store::size_type incident_edges_size_type;
    typedef incidence_iterator<typename incidence_store::iterator, edge_descriptor> incident_edge_iterator;
    typedef std::pair<incident_edge_iterator, incident_edge_iterator> incident_edge_range;

    // Generate the vertex type and its store and key type.
    typedef undirected_vertex<EdgeProps, incidence_store> vertex_type;
    typedef typename VertexStore::template store<vertex_type>::type vertex_store;
    typedef typename vertex_store::size_type vertices_size_type;
    typedef typename vertex_store::vertex_iterator vertex_iterator;
    typedef typename vertex_store::vertex_range vertex_range;
    typedef typename VertexStore::key_type vertex_key;

    // Generate the adjacency iterator
    typedef adjacency_iterator<incident_edge_iterator> adjacent_vertex_iterator;
    typedef std::pair<adjacent_vertex_iterator, adjacent_vertex_iterator> adjacent_vertex_range;

};

#endif
