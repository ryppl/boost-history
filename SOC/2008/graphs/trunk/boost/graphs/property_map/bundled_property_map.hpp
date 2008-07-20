
#ifndef BOOST_GRAPHS_PROPERTY_MAP_BUNDLED_PROPERTIES_HPP
#define BOOST_GRAPHS_PROPERTY_MAP_BUNDLED_PROPERTIES_HPP

namespace boost {
namespace graphs {

/**
 * A simple property map provides get/put functions for non-bundled or simple
 * vertex and edge properties. This is not called "simple" because it is
 * trivially implemented but because it return the properties assigned to the
 * vertex or edge (depending on the descriptor) without any indirection. To
 * build property maps over members or a structured (or bundled) property,
 * use the bundled property map.
 */
template <typename Graph, typename Descriptor, typename Bundle, typename Property>
struct bundled_property_map
{
    typedef Descriptor key_type;
    typedef Property property_type;

    inline bundled_property_map(Graph& g, Property Bundle::* mem)
        : graph(g)
        , member(mem)
    { }

    inline bundled_property_map(bundled_property_map const& x)
        : graph(x.g)
        , member(x.mem)
    { }

    inline Bundle const& bundle(key_type const& k) const
    { return graph.properties(k); }

    inline Bundle& bundle(key_type const& k)
    { return graph.properties(k); }


    inline Property const& get(key_type const& k) const
    { return bundle(k).*member; }

    inline void put(key_type const& k, Property const& v)
    { bundle(k).*member = v; }

    Graph& graph;
    Property Bundle::* member;
};

template <typename G, typename D, typename B, typename P>
typename bundled_property_map<G,D,B,P>::property_type const&
get(bundled_property_map<G,D,B,P> const& pm,
    typename bundled_property_map<G,D,B,P>::key_type const& k)
{
    return pm.get(k);
}

template <typename G, typename D, typename B, typename P>
void
put(bundled_property_map<G,D,B,P>& pm,
    typename bundled_property_map<G,D,B,P>::key_type const& k,
    typename bundled_property_map<G,D,B,P>::property_type const& v)
{
    pm.put(k, v);
}

} /* namesapce graphs */
} /* namespace boost */

#endif
