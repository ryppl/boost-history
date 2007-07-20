// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_CONNECTIVITY_HPP
#define BOOST_GRAPH_CONNECTIVITY_HPP

#include <boost/graph/named_parameters.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/connected_components.hpp>

namespace boost
{
    namespace detail
    {
        template <typename Graph, typename CompMap, typename Components>
        inline void
        assign_vertices_to_components(const Graph& g,
                                      size_t n,
                                      CompMap comps_map,
                                      Components& comps)
        {
            comps.resize(n);
            typename graph_traits<Graph>::vertex_iterator i, end;
            for(tie(i, end) = vertices(g); i != end; ++i) {
                comps[comps_map[*i]].push_back(*i);
            }
        }

        // Notes on fetch_connected_components()
        //
        // If the component map is non-void, then we don't need to fetch
        // the connected components - we'll just assume that it's already
        // been done. If, however, the component map is void, then we
        // actually have to get them using the algorithm.

        // This is the catch-all version, where component map is non-void.
        // Here, we have to find the max element... Note that if the user
        // passes the return from connected_components, we can effectively
        // bypass the computation of the number of components - which would
        // probably be prefereable.
        template <
            typename Graph, typename Components, typename VertexIndexMap,
            typename SizeType, typename ComponentMap, typename ColorMap>
        inline SizeType
        fetch_connected_components(const Graph& g,
                                   Components& comps,
                                   VertexIndexMap,
                                   SizeType number,
                                   ComponentMap comps_map,
                                   ColorMap)
        {
            SizeType ret(0);
            if(number == 0) {
                typename graph_traits<Graph>::vertex_iterator i, end;
                for(tie(i, end) = vertices(g); i != end; ++i) {
                    ret = std::max(comps_map[*i] + 1, ret);
                }
            }
            else {
                ret = number;
            }
            assign_vertices_to_components(g, ret, comps_map, comps);
            return ret;
        }


        template <
            typename Graph, typename Components,
            typename VertexIndexMap, typename SizeType
        >
        inline SizeType
        fetch_connected_components(const Graph& g,
                                   Components& comps,
                                   VertexIndexMap indices,
                                   SizeType,
                                   parameter::void_,
                                   parameter::void_)
        {
            typedef exterior_vertex_property<Graph, std::size_t> ComponentProperty;
            typedef typename ComponentProperty::container_type ComponentContainer;
            typedef typename ComponentProperty::map_type ComponentMap;

            ComponentContainer comps_store(num_vertices(g));
            ComponentMap comps_map(make_property_map(comps_store));

            // get the compoents first
            SizeType ret = connected_components(g, comps_map,
                vertex_index_map(indices));

            // and assign them to the output
            assign_vertices_to_components(g, ret, comps_map, comps);
            return ret;
        }

        template <
            typename Graph, typename VertexIndexMap, typename Components,
            typename SizeType, typename ColorMap
        >
        inline SizeType
        fetch_connected_components(const Graph& g,
                                   Components& comps,
                                   VertexIndexMap indices,
                                   SizeType,
                                   parameter::void_,
                                   ColorMap colors)
        {
            typedef exterior_vertex_property<Graph, size_t> ComponentProperty;
            typedef typename ComponentProperty::container_type ComponentContainer;
            typedef typename ComponentProperty::map_type ComponentMap;

            ComponentContainer comps_store(num_vertices(g));
            ComponentMap comps_map(make_property_map(comps_store));

            // get the components
            SizeType ret = connected_components(g, comps_map,
                vertex_index_map(indices).
                color_map(colors));

            // and assign them to the output
            assign_vertices_to_components(g, ret, comps_map, comps);
            return ret;
        }
    }

    // TODO: There seems to be a bug in Boost.Parameter that prevents the
    // actual use of more than 5 parameters - even when the arity is
    // jacked up - unless I'm doing something wrong. For the time being,
    // the number parameter is not specified, and simply defaults to 0
    // since the only way to get around it would be to pass something
    // as a pair - and I'm not interested in that right now.

    // the connectivity function
    BOOST_PARAMETER_FUNCTION(
        (std::size_t), connectivity, tag,
        (required
            (graph, *)
            (out(components), *))
        (optional
            (number, (std::size_t), 0)
            (component_map, *, parameter::void_())
            (out(color_map), *, parameter::void_())
            (vertex_index_map, *, get(vertex_index, graph)))
        )
    {
        // step 1, get the components (maybe), returning the number
        return detail::fetch_connected_components(graph,
                components,
                vertex_index_map,
                number,
                component_map,
                color_map);
    }
}

#endif
