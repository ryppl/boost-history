
#ifndef BOOST_GRAPHS_PROPERTY_MAP_SIMPLE_PROPERTIES_HPP
#define BOOST_GRAPHS_PROPERTY_MAP_SIMPLE_PROPERTIES_HPP

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
template <typename Graph, typename Descriptor, typename Property>
struct simple_property_map
{
    typedef Descriptor key_type;
    typedef Property property_type;

    simple_property_map(Graph& g)
        : graph(g)
    { }

    simple_property_map(simple_property_map const& x)
        : graph(x.g)
    { }

    Graph& graph;
};

template <typename G, typename D, typename P>
typename simple_property_map<G,D,P>::property_type const&
get(simple_property_map<G,D,P> const& pm,
    typename simple_property_map<G,D,P>::key_type const& x)
{
    return pm.graph.properties(x);
}

template <typename G, typename D, typename P>
void
put(simple_property_map<G,D,P>& pm,
    typename simple_property_map<G,D,P>::key_type const& x,
    typename simple_property_map<G,D,P>::property_type const& v)
{
    pm.graph.properties(x) = v;
}

} /* namesapce graphs */
} /* namespace boost */

#endif
