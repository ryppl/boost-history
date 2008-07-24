
#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <boost/shared_ptr.hpp>

#include <boost/descriptors.hpp>

// Include associative adpaters for exterior properties.
#include "properties/hashed_property_container.hpp"
#include "properties/indexed_property_container.hpp"

// Include property map implementations.
#include "properties/container_property_map.hpp"
#include "properties/simple_property_map.hpp"
#include "properties/bundled_property_map.hpp"

namespace detail
{
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
    template <typename Descriptor, typename Property>
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
}

/**
 * Used to cotnain exterior vertex properties.
 */
template <typename Graph, typename Property>
struct exterior_vertex_property
    : detail::choose_container<typename Graph::vertex_descriptor, Property>::type
{
    typedef typename detail::choose_container<
        typename Graph::vertex_descriptor, Property
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


// The following is a solution to require/supply problem present in a number
// of generic graph algorithms with respect to the external data. Essentially,
// we can choose to require the user to give us a property map, or we can
// create our own data, releasing it when we're done. The property wrapper
// and generic_*_property classes do this for us. Depending on the method of
// construction, generic properties will either use an existing property map
// or create the their own property container (using a shared pointer so it
// gets deleted later.

namespace detail
{
    template <typename Graph>
    inline typename Graph::vertex_range
    get_range(Graph const& g, typename Graph::vertex_descriptor)
    { return g.vertices(); }

    template <typename Graph>
    inline typename Graph::edge_range
    get_range(Graph const& g, typename Graph::edge_descriptor)
    { return g.edges(); }
}

/**
 * The property wrapper type allows the definition of exterior properties that
 * can maintain either their own internal store (via shared pointers) or be
 * constructed over an exterioir store. This is useful in algorithms that
 * can provide default exterior properties or allow the user to provide their
 * own.
 *
 * The use of this type incurs a slight overhead due to an additional level of
 * indirection.
 */
template <typename Graph, typename Descriptor, typename Property>
struct property_wrapper
{
    // Select the container and map type for the self-wrapping property.
    typedef typename detail::choose_container<Descriptor, Property>::type container_type;
    typedef container_property_map<container_type> map_type;

    typedef typename map_type::value_type value_type;
    typedef typename map_type::key_type key_type;
    typedef typename map_type::reference reference;

    // Delay initialization...
    inline property_wrapper()
        : container(), map()
    { }

    // Construct an underlying store over the map.
    inline property_wrapper(Graph const& g, value_type const& x = value_type())
        : container(new container_type(detail::get_range(g, Descriptor()), x))
        , map(*container)
    { }

    // Construct the map over a user-provided property map. No container needed.
    inline property_wrapper(map_type map)
        : container(), map(map)
    { }

    inline value_type& operator()(key_type const& key)
    { return map(key); }

    inline value_type const& operator()(key_type const& key) const
    { return map(key); }

    inline void swap(property_wrapper& x)
    {
        using std::swap;
        container.swap(x.container);
        swap(map, x.map);
    }

    boost::shared_ptr<container_type> container;
    map_type map;
};

/**
 * The optional vertex map allows a user-provided property map or a self-
 * contained exterior property to be passed to a generic function. The user
 * provided property map is not required to be constructed over an exterior
 * property.
 */
template <typename Graph, typename Property>
struct optional_vertex_map
    : property_wrapper<Graph, typename Graph::vertex_descriptor, Property>
{
    typedef property_wrapper<Graph, typename Graph::vertex_descriptor, Property> base_type;
    typedef typename base_type::map_type map_type;

    inline optional_vertex_map()
        : base_type()
    { }

    inline optional_vertex_map(Graph const& g, Property const& x = Property())
        : base_type(g, x)
    { }

    inline optional_vertex_map(map_type map)
        : base_type(map)
    { }
};

/**
 * The optional edge map allows a user-provided property map or a self-
 * contained exterior property to be passed to a generic function. The user
 * provided property map is not required to be constructed over an exterior
 * property.
 */
template <typename Graph, typename Property>
struct optional_edge_map
    : property_wrapper<Graph, typename Graph::edge_descriptor, Property>
{
    typedef property_wrapper<Graph, typename Graph::vertex_descriptor, Property> base_type;
    typedef typename base_type::map_type map_type;

    inline optional_edge_map()
        : base_type()
    { }

    inline optional_edge_map(Graph const& g, Property const& x = Property())
        : base_type(g, x)
    { }

    inline optional_edge_map(map_type map)
        : base_type(map)
    { }
};

#endif
