// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_CONNECTIVITY_HPP
#define BOOST_GRAPH_CONNECTIVITY_HPP

// boost includes
#include <boost/graph/named_parameters.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

namespace boost
{
    namespace detail
    {
        template <
            typename Graph,
            typename CompMap,
            typename ColorMap,
            typename IndexMap>
        inline size_t
        forward_connected_components(const Graph& g,
                                     CompMap comps,
                                     ColorMap colors,
                                     IndexMap indices)
        {
            return connected_components(g, comps,
                color_map(colors).
                vertex_index_map(indices));
        }

        template <
            typename Graph,
            typename CompMap,
            typename IndexMap>
        inline size_t
        forward_connected_components(const Graph& g,
                                     CompMap comps,
                                     not_given,
                                     IndexMap indices)
        {
            return connected_components(g, comps,
                vertex_index_map(indices));
        }

        template <
            typename Graph,
            typename CompMap,
            typename Components>
        inline void allocate_components(const Graph& g,
                                        size_t n,
                                        CompMap comp_map,
                                        Components& comps)
        {
            comps.resize(n);
            typename Graph::vertex_iterator i, end;
            for(tie(i, end) = vertices(g); i != end; ++i) {
                comps[comp_map[*i]].push_back(*i);
            }
        }

        template <
            typename Graph,
            typename CompMap>
        inline void allocate_components(const Graph& g,
                                        size_t n,
                                        CompMap comp_map,
                                        not_given)
        { }
    }


    // connected_components_2 is forwarded to the appropriate
    // "legacy" function. we have to have a different name due
    // to ambiguities.
    BOOST_PARAMETER_FUNCTION(
        (std::size_t),
        connectivity, tag,
        (required
            (graph, *)
            (out(component_map), *))
        (optional
            (color_map, *, not_given())
            (vertex_index_map, *, get(vertex_index, graph))
            (out(components), *, not_given()))
        )
    {
        size_t n = detail::forward_connected_components(graph,
            component_map,
            color_map,
            vertex_index_map);

        // possibly allocate components, could be a non-call
        detail::allocate_components(graph, n, component_map, components);

        return n;
    }
}

#endif
