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
    struct with_outward_spokes { };
    struct with_inward_spokes { };
    struct with_bidirected_spokes
        : public with_outward_spokes
        , public with_inward_spokes
    { };

    // Forward and reverse edge options apply to edges in path
    // graphs. Note that reverse and bidirected edges only apply
    // to directed or bidirectional graphs.
    struct with_forward_edges { };
    struct with_reverse_edges { };
    struct with_bidirected_edges
        : public with_forward_edges
        , public with_reverse_edges
    { };

    namespace detail
    {
        // These structs are used to map graph-specific options onto
        // the basic forward, reverse and bidirected edges.
        template <typename Option> struct edge_gen_policy { };

        template <> struct edge_gen_policy<with_clockwise_cycle> { typedef with_forward_edges creation_policy; };
        template <> struct edge_gen_policy<with_counterclockwise_cycle> { typedef with_reverse_edges creation_policy; };
        template <> struct edge_gen_policy<with_bidirected_cycle> { typedef with_bidirected_edges creation_policy; };

        template <> struct edge_gen_policy<with_outward_spokes> { typedef with_forward_edges creation_policy; };
        template <> struct edge_gen_policy<with_inward_spokes> { typedef with_reverse_edges creation_policy; };
        template <> struct edge_gen_policy<with_bidirected_spokes> { typedef with_bidirected_edges creation_policy; };


        // Helper functions for connecting vertices
        template <typename Graph>
        inline void
        generate_edge(Graph& g,
                      typename graph_traits<Graph>::vertex_descriptor u,
                      typename graph_traits<Graph>::vertex_descriptor v,
                      with_forward_edges)
        { add_edge(u, v, g); }

        template <typename Graph>
        inline void
        generate_edge(Graph& g,
                      typename graph_traits<Graph>::vertex_descriptor u,
                      typename graph_traits<Graph>::vertex_descriptor v,
                       with_reverse_edges)
        { add_edge(v, u, g); }

        template <typename Graph>
        inline void
        generate_edge(Graph& g,
                      typename graph_traits<Graph>::vertex_descriptor u,
                      typename graph_traits<Graph>::vertex_descriptor v,
                      with_bidirected_edges)
        {
            add_edge(u, v, g);
            add_edge(v, u, g);
        }


        template <typename Graph, typename EdgeOption>
        inline void
        generate_edge(Graph& g,
                       typename graph_traits<Graph>::vertex_descriptor u,
                       typename graph_traits<Graph>::vertex_descriptor v,
                       EdgeOption)
        {
            typename edge_gen_policy<EdgeOption>::creation_policy policy;
            generate_edge(g, u, v, policy);
        }
    }
}

#endif
