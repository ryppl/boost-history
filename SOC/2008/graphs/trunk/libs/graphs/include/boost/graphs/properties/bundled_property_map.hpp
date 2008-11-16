
#ifndef BUNDLED_PROPERTY_MAP_HPP
#define BUNDLED_PROPERTY_MAP_HPP

/**
 * The bundled property map provides a "slicing" property map for vertex or
 * edge properties that are defined by compound types (structs or classes).
 */
template <typename Graph, typename Key, typename Bundle, typename Property>
struct bundled_property_map
{
    typedef Key key_type;
    typedef Property value_type;

    inline bundled_property_map(Graph& g, Property Bundle::* b)
        : graph(g), bundle(b)
    { }

    inline value_type& operator()(key_type const& key)
    { return graph[key].*bundle; }

    inline void operator()(key_type const& key, value_type const& value) const
    { graph[key].*bundle = value; }

    Graph& graph;
    Property Bundle::* bundle;
};

#endif
