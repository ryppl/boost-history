
#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <boost/descriptors.hpp>

// Include associative adpaters for exterior properties.
#include "property_map/hashed_property_container.hpp"
#include "property_map/indexed_property_container.hpp"

// Include property map implementations.
#include "property_map/container_property_map.hpp"
#include "property_map/simple_property_map.hpp"
#include "property_map/bundled_property_map.hpp"

// TODO: We currently distinguish between exterior and interior using the names
// of the structures...

// Define the mapping strategy based on the type of descriptor. By default,
// we prefer to use hashing since the number of data structures that actually
// index their vertices is... tiny.
struct index_mapping { };
struct hash_mapping { };

template <typename Descriptor>
struct descriptor_mapping
{ typedef hash_mapping strategy; };

template <typename Index>
struct descriptor_mapping<index_descriptor<Index>>
{ typedef index_mapping strategy;};

// Select the type of container based on the underlying store selector.
template <typename Selector, typename Descriptor, typename Property>
struct choose_container
{
    typedef typename boost::mpl::if_<
            boost::is_same<
                typename descriptor_mapping<Descriptor>::strategy,
                hash_mapping>,
            hashed_property_container<Descriptor, Property>,
            indexed_property_container<Descriptor, Property>
        >::type type;
};

/**
 * Used to cotnain exterior vertex properties.
 */
template <typename Graph, typename Property>
struct exterior_vertex_property
    : choose_container<
            typename Graph::vertex_store_selector, typename Graph::vertex_descriptor, Property
        >::type
{
    typedef typename choose_container<
            typename Graph::vertex_store_selector, typename Graph::vertex_descriptor, Property
        >::type base_type;

    exterior_vertex_property(Graph const& g, Property const& p = Property())
        : base_type(g.begin_vertices(), g.end_vertices(), p)
    { }
};

/**
 * Used to contain exterior edge properties.
 *
 * @todo It turns out that undirected edge_vectors can use vectors to store
 * properties. This is because the edge descriptor is basically just returning
 * the index into the vector as a key.
 */
template <typename Graph, typename Property>
struct exterior_edge_property
    : hashed_property_container<typename Graph::edge_descriptor, Property>
{
    typedef hashed_property_container<typename Graph::edge_descriptor, Property> base_type;

    exterior_edge_property(Graph const& g, Property const& p = Property())
        : base_type(g.begin_edges(), g.end_edges(), p)
    { }
};

/**
 * The property map structure provides a funtional abstraction over the
 * associative mapping provided by some type (or set of types). The default
 * implementation is constructed over an associative container.
 *
 * @requires AssociativeContainer<Container>
 */
template <typename Container>
struct exterior_property_map
    : container_property_map<Container>
{
    exterior_property_map(Container& cont)
        : container_property_map<Container>(cont)
    { }
};

/**
 * The interior property map defines a property map over the interior properties
 * of a vertex. Although this takes 3 parameters, you should always use default
 * value of the 3rd parameter. The Key parameter must either be the vertex
 * or edge descriptor type of the graph.
 */
template <typename Graph, typename Key, typename Property = typename Graph::vertex_properties>
struct interior_property_map
    : simple_property_map<Graph, Key, Property>
{
    interior_property_map(Graph& g)
        : simple_property_map<Graph, Key, Property>(g)
    { }
};

/** A specialization of the above for members of the vertex/edge label. */
template <typename Graph, typename Key, typename Bundle, typename Property>
struct interior_property_map<Graph, Key, Property Bundle::*>
    : bundled_property_map<Graph, Key, Bundle, Property>
{
    interior_property_map(Graph& g, Property Bundle::* b)
        : bundled_property_map<Graph, Key, Bundle, Property>(g, b)
    { }
};

#endif
