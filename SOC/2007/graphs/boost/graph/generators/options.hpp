// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_GENERATORS_OPTIONS_GRAPH_HXX
#define BOOST_GRAPH_GENERATORS_OPTIONS_GRAPH_HXX

#include <boost/graph/graph_traits.hpp>

namespace boost
{
    // These options control the direction of edges added to
    // graphs that have a peripheral cycle as a subgraph. These
    // option only apply to directed graphs
    struct with_clockwise_cycle { };
    struct with_counterclockwise_cycle { };
    struct with_bidirected_cycle
        : public with_clockwise_cycle
        , public with_counterclockwise_cycle
    { };

    // These options control the direction of edges added to
    // graphs that have form star with a central vertex. These
    // options only apply to directed graphs.
    struct with_inward_spokes { };
    struct with_outward_spokes { };
    struct with_bidirected_spokes
        : public with_inward_spokes
        , public with_outward_spokes
    { };

    namespace detail
    {
        // Helper functions for connecting vertices

        template <typename Graph>
        inline void
        add_cycle_edge(Graph& g,
                       typename graph_traits<Graph>::vertex_descriptor u,
                       typename graph_traits<Graph>::vertex_descriptor v,
                       with_clockwise_cycle)
        { add_edge(u, v, g); }

        template <typename Graph>
        inline void
        add_cycle_edge(Graph& g,
                       typename graph_traits<Graph>::vertex_descriptor u,
                       typename graph_traits<Graph>::vertex_descriptor v,
                       with_counterclockwise_cycle)
        { add_edge(v, u, g); }

        template <typename Graph>
        inline void
        add_cycle_edge(Graph& g,
                       typename graph_traits<Graph>::vertex_descriptor u,
                       typename graph_traits<Graph>::vertex_descriptor v,
                       with_bidirected_cycle)
        {
            add_edge(u, v, g);
            add_edge(v, u, g);
        }


        template <typename Graph>
        inline void
        add_spoke_edge(Graph& g,
                       typename graph_traits<Graph>::vertex_descriptor u,
                       typename graph_traits<Graph>::vertex_descriptor v,
                       with_outward_spokes)
        { add_edge(u, v, g); }

        template <typename Graph>
        inline void
        add_spoke_edge(Graph& g,
                       typename graph_traits<Graph>::vertex_descriptor u,
                       typename graph_traits<Graph>::vertex_descriptor v,
                       with_inward_spokes)
        { add_edge(v, u, g); }

        template <typename Graph>
        inline void
        add_spoke_edge(Graph& g,
                       typename graph_traits<Graph>::vertex_descriptor u,
                       typename graph_traits<Graph>::vertex_descriptor v,
                       with_bidirected_spokes)
        {
            add_edge(u, v, g);
            add_edge(v, u, g);
        }
    }
}

#endif
