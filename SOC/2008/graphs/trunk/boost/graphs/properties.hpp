
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
 * Used to contain exterior vertex properties.
 */
template <typename Graph, typename Label>
struct exterior_vertex_label
    : detail::choose_container<typename Graph::vertex_descriptor, Label>::type
{
    typedef typename detail::choose_container<
        typename Graph::vertex_descriptor, Label
    >::type base_type;

    exterior_vertex_label(Graph const& g, Label const& l = Label())
        : base_type(g.begin_vertices(), g.end_vertices(), l)
    { }
};

/**
 * Used to contain exterior edge properties. Unlike exterior vertex labels, the
 * edge label must use a hashtable since edge descriptors are not integers.
 *
 * @todo It turns out that undirected edge_vectors can use vectors to store
 * properties. This is because the edge descriptor is basically just returning
 * the index into the vector as a key. It might be worthwhile specializing this
 * template along those lines. For now, this is not done.
 */
template <typename Graph, typename Label>
struct exterior_edge_label
    : hashed_property_container<typename Graph::edge_descriptor, Label>
{
    typedef hashed_property_container<typename Graph::edge_descriptor, Label> base_type;

    exterior_edge_label(Graph const& g, Label const& l = Label())
        : base_type(g.begin_edges(), g.end_edges(), l)
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
template <typename Graph, typename Key, typename Property = typename Graph::vertex_label>
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
template <typename Graph, typename Descriptor, typename Label>
struct optional_label
{
    // Select the container and map type for the self-wrapping property.
    typedef typename detail::choose_container<Descriptor, Label>::type container_type;
    typedef container_property_map<container_type> map_type;

    typedef typename map_type::value_type value_type;
    typedef typename map_type::key_type key_type;
    typedef typename map_type::reference reference;

    // By default, the optional property contains no property. This should
    // probably never be used.
    inline optional_label()
        : container(), map()
    { }

    // Allocate an exterior label and build a property map over it.
    inline optional_label(Graph const& g, value_type const& x = value_type())
        : container(new container_type(detail::get_range(g, Descriptor()), x))
        , map(*container)
    { }

    // Construct the optional property over the given map, without allocating
    // an exterior label.
    inline optional_label(map_type map)
        : container(), map(map)
    { }

    inline value_type& operator()(key_type const& key)
    { return map(key); }

    inline void operator()(key_type const& key, value_type const& value) const
    { return map(key, value); }

    inline optional_label& swap(optional_label&& x)
    {
        using std::swap;
        swap(container, x.container);   // Should overload to constant time op.
        swap(map, x.map);
        return *this;
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
template <typename Graph, typename Label>
struct optional_vertex_label
    : optional_label<Graph, typename Graph::vertex_descriptor, Label>
{
    typedef optional_label<Graph, typename Graph::vertex_descriptor, Label> base_type;
    typedef typename base_type::map_type map_type;

    inline optional_vertex_label()
        : base_type()
    { }

    inline optional_vertex_label(Graph const& g, Label const& x = Label())
        : base_type(g, x)
    { }

    inline optional_vertex_label(map_type map)
        : base_type(map)
    { }
};

/**
 * The optional edge map allows a user-provided property map or a self-
 * contained exterior property to be passed to a generic function. The user
 * provided property map is not required to be constructed over an exterior
 * property.
 */
template <typename Graph, typename Label>
struct optional_edge_label
    : optional_label<Graph, typename Graph::edge_descriptor, Label>
{
    typedef optional_label<Graph, typename Graph::vertex_descriptor, Label> base_type;
    typedef typename base_type::map_type map_type;

    inline optional_edge_label()
        : base_type()
    { }

    inline optional_edge_label(Graph const& g, Label const& x = Label())
        : base_type(g, x)
    { }

    inline optional_edge_label(map_type map)
        : base_type(map)
    { }
};


namespace detail
{
    // Optionally initialize the container, but not if the map is already
    // initialized.
    template <typename Graph, typename Map>
    void optional_init(Graph const& g, Map& map, typename Map::value_type x)
    {
        if(!map.container) {
            Map tmp(g, x);
            map.swap(tmp);
        }
    }
}

/**
 * Delayed initialization of optional property maps. The default solution
 * is to do nothing (i.e,. the map is already initialized). Specialized
 * variants simply swap the given map with one that's actually initialized.
 */
/*
template <typename Graph, typename Map>
void initialize(Graph const&, Map&, typename Map::value_type)
{ throw 0; }'
*/

template <typename Graph, typename Label>
void initialize(Graph const& g, optional_vertex_label<Graph, Label>& map, Label const& x)
{ detail::optional_init(g, map, x); }

template <typename Graph, typename Label>
void initialize(Graph const g, optional_edge_label<Graph, Label>& map, Label const& x)
{ detail::optional_init(g, map, x); }

#endif
