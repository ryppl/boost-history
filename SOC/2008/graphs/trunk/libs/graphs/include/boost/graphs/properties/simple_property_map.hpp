
#ifndef SIMPLE_PROPERTY_MAP_HPP
#define SIMPLE_PROPERTY_MAP_HPP

/**
 * A simple property map provides get/put functions for non-bundled or simple
 * vertex and edge properties. This is not called "simple" because it is
 * trivially implemented but because it return the properties assigned to the
 * vertex or edge (depending on the descriptor) without any indirection. To
 * build property maps over members or a structured (or bundled) property,
 * use the bundled property map.
 *
 * Note that this works because all graph types overload the [] operators to
 * return either vertex or edge properties depending on key type.
 */
template <typename Graph, typename Key, typename Property>
struct simple_property_map
{
    typedef Key key_type;
    typedef Property value_type;
    typedef value_type& reference;

    inline simple_property_map(Graph& g)
        : graph(g)
    { }

    inline value_type& operator()(key_type const& key)
    { return graph[key]; }

    inline void operator()(key_type const& key, value_type const& value) const
    { graph[key] = value; }

    Graph& graph;
};

#endif
