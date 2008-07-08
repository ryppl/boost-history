
#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <vector>
#include <tr1/unordered_map>

#include <boost/type_traits.hpp>
#include <boost/mpl/if.hpp>

#include "vertex_vector.hpp"
#include "edge_vector.hpp"

#include "property_map/hashed_properties.hpp"
#include "property_map/indexed_properties.hpp"

// All the fun of exterior properties... We need a mechanism that determines
// the underlying mapping type of exterior properties. For vector-based stores
// we can simply map each vertex to its corresponding element in another vector.
// For non-vector-based stores, it's easier to use a hash of the descriptor.

// We need a metafunction to determine whether or not a vertex set allows
// indexed or hashed exterior properties. By default, we should assume that
// we're using hash tables (they're more flexible).

template <typename VertexStore>
struct use_hashing
{ BOOST_STATIC_CONSTANT(bool, value = true); };

// Specialize over the basic vertex vector and edge vector.
template <template <typename> class Alloc>
struct use_hashing< vertex_vector<Alloc> >
{ BOOST_STATIC_CONSTANT(bool, value = false); };

template <template <typename> class Alloc>
struct use_hashing< edge_vector<Alloc> >
{ BOOST_STATIC_CONSTANT(bool, value = false); };

// Select the type of container based on the underlying store selector
template <typename Selector, typename Descriptor, typename Property>
struct choose_container
{
    typedef typename boost::mpl::if_<
            use_hashing<Selector>,
            hashed_property_container<Descriptor, Property>,
            indexed_property_container<Descriptor, Property>
        >::type type;
};

template <typename Graph, typename Property>
struct exterior_vertex_property
    : choose_container<
            typename Graph::vertex_store_selector, typename Graph::vertex_descriptor, Property
        >::type
{
    typedef typename choose_container<
            typename Graph::vertex_store_selector, typename Graph::vertex_descriptor, Property
        >::type base_type;

    exterior_vertex_property(Graph const& g)
        : base_type(g.num_vertices())
    { }

    exterior_vertex_property(Graph const& g, Property const& p)
        : base_type(g.begin_vertices(), g.end_vertices(), p)
    { }
};

template <typename Graph, typename Property>
struct exterior_edge_property
    : hashed_property_container<typename Graph::edge_descriptor, Property>
{
    typedef hashed_property_container<typename Graph::edge_descriptor, Property> base_type;

    exterior_edge_property(const Graph& g)
        : base_type(g.num_edges())
    { }

    exterior_edge_property(Graph const& g, Property const& p)
        : base_type(g.begin_edges(), g.end_edges(), p)
    { }
};

#if 0

// For now, these tags are used to actually decide the underlying implementation
// of the property map.
struct indexed_property_map_tag { };
struct hashed_property_map_tag { };

template <typename Tag, typename Iterator, typename Property>
struct exterior_property
{
    typedef typename mpl::if_<
            is_same<Tag, hashed_property_map_tag>,
            hashed_property_container<Iterator, Property>,
            indexed_property_container<Iterator, Property>
        >::type container_type;

    typedef typename mpl::if_<
            is_same<Tag, hashed_property_map_tag>,      // predicate
            hashed_property_map<Iterator, Property>,    // on true
            indexed_property_map<Iterator, Property>    // on false
        >::type map_type;
};

template <typename Graph, typename Property>
struct exterior_vertex_property
{
    typedef exterior_property<
            typename Graph::vertex_property_map_category,
            typename Graph::vertex_iterator,
            Property
                    > base_type;
            typedef typename base_type::container_type container_type;
            typedef typename base_type::map_type map_type;};

template <typename Graph, typename Property>
struct exterior_edge_property
{
    typedef exterior_property<
            typename Graph::edge_property_map_category,
            typename Graph::edge_iterator,
            Property
        > base_type;
    typedef typename base_type::container_type container_type;
    typedef typename base_type::map_type map_type;

};


// Interior properties aer also a great deal of fun.

template <typename Graph, typename Property = typename Graph::vertex_properties>
struct interior_vertex_property
{
    typedef simple_property_map<
            Graph,
            typename Graph::vertex_descriptor,
            typename Graph::vertex_properties
        > map_type;
};

template <typename Graph, typename Bundle, typename Property>
struct interior_vertex_property<Graph, Property Bundle::*>
{
    typedef bundled_property_map<
            Graph,
            typename Graph::vertex_descriptor,
            Bundle,
            Property
        > map_type;
};

template <typename Graph, typename Property = typename Graph::vertex_properties>
struct interior_edge_property
{
    typedef simple_property_map<
            Graph,
            typename Graph::edge_descriptor,
            typename Graph::edge_properties
        > map_type;
};

template <typename Graph, typename Bundle, typename Property>
struct interior_edge_property<Graph, Property Bundle::*>
{
    typedef bundled_property_map<
            Graph,
            typename Graph::edge_descriptor,
            Bundle,
            Property
        > map_type;
};

#endif

#endif
