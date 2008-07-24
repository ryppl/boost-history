
#ifndef ALGORITHM_UTILITY_HPP
#define ALGORITHM_UTILITY_HPP

namespace detail
{
    // Optionally initialize the container, but not if the map is already
    // initialized.
    template <typename Graph, typename Map>
    void do_opt_init(Graph const& g, Map& map, typename Map::value_type x)
    {
        if(!map.container) {
            Map t(g, x);
            map.swap(t);
        }
    }

    // Delayed initialization of optional property maps. The default solution
    // is to do nothing (i.e,. the map is already initialized). Specialized
    // variants simply swap the given map with one that's actually initialized.
    template <typename Graph, typename Map>
    void optional_prop_init(Graph const&, Map&, typename Map::value_type)
    { }

    template <typename Graph, typename Prop>
    void optional_prop_init(Graph const& g, optional_vertex_map<Graph, Prop>& map, Prop x)
    { do_opt_init(g, map, x); }

    template <typename Graph, typename Prop>
    void optional_prop_init(Graph const g, optional_edge_map<Graph, Prop>& map, Prop x)
    { do_opt_init(g, map, x); }

}

#endif
