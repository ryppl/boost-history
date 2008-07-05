
#ifndef BOOST_GRAPHS_PROPERTIES_HPP
#define BOOST_GRAPHS_PROPERTIES_HPP

#include <vector>
#include <tr1/unordered_map>

#include <boost/type_traits.hpp>
#include <boost/mpl/if.hpp>

#include <boost/graphs/property_map/simple_properties.hpp>
#include <boost/graphs/property_map/bundled_properties.hpp>
#include <boost/graphs/property_map/hashed_properties.hpp>
#include <boost/graphs/property_map/indexed_properties.hpp>

namespace boost {
namespace graphs {

/**
 * Default color types for this library.
 */
enum color {
    white_color,
    gray_color,
    black_color,
    red_color,
    green_color,
    blue_color
};

/**
 * A traits class for colors. Specialize this if, for some reason, you ever
 * plan to specialize the notion of colors - which may be possible.
 *
 * @todo Obviously, this will be conceptized. See below.
 */
template <typename Color>
struct color_traits
{
    static color white()   { return white_color; }
    static color gray()    { return gray_color; }
    static color black()   { return black_color; }
    static color red()     { return red_color; }
    static color green()   { return green_color; }
    static color blue()    { return blue_color; }
};

// All the fun of exterior properties... We need a mechanism that determines
// the underlying mapping type of exterior properties. For vector-based stores
// we can simply map each vertex to its corresponding element in another vector.
// For non-vector-based stores, it's easier to use a hash of the descriptor.

// For now, these tags are used to actually decide the underlying implementation
// of the property map.
struct indexed_property_map_tag { };
struct hashed_property_map_tag { };

template <typename Tag, typename Iterator, typename Property>
struct exterior_property
{
    typedef typename mpl::if_<
            is_same<Tag, hashed_property_map_tag>,          // predicate
            hashed_property_container<Iterator, Property>,  // on true
            indexed_property_container<Iterator, Property>  // on false
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

} /* namespace graphs */
} /* namespace boost */

#endif
