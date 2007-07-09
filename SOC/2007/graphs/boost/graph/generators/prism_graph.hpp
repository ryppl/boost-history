// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_GENERATORS_PRISM_GRAPH_HXX
#define BOOST_GRAPH_GENERATORS_PRISM_GRAPH_HXX

#include <vector>
#include <boost/utility.hpp>
#include <boost/graph/generators/options.hpp>
#include <boost/graph/generators/cycle_graph.hpp>

namespace boost
{
    // Prism graphs are kind of interesting... They're basically concentric
    // cycle graphs with aligned vertices connected by an edge (spoke).
    // The specification of a prism graphs is usually G = Y(m,n) such that
    // G is n concentric cycles with m vertices each.

    template <
        class Graph,
        class RandomAccessIterator,
        class CycleDirection,
        class SpokeDirection
    >
    inline void
    induce_prism_graph(Graph& g, size_t m, size_t n,
                       RandomAccessIterator iter,
                       CycleDirection cycle,
                       SpokeDirection spokes)
    {
        typedef RandomAccessIterator iterator;

        // start by inducing the n cycles on the graph
        for(size_t i = 0; i < n; ++i) {
            induce_cycle_graph(g, m, iter + (m * i), cycle);
        }

        // the, build the psuedo-spokes to connect the
        // concentric cycles
        for(size_t j = 0; j < n - 1; ++j) {
            for(size_t i = 0; i < m; ++i) {
                iterator u = iter + i + m * j;
                iterator v = iter + i + m * (j + 1);
                detail::add_spoke_edge(g, *u, *v, spokes);
            }
        }
    }

    template <
        class Graph,
        class RandomAccessContainer,
        class CycleDirection,
        class SpokeDirection
    >
    inline void
    make_prism_graph(Graph& g, size_t m, size_t n,
                     RandomAccessContainer& verts,
                     CycleDirection cycle,
                     SpokeDirection spokes)
    {
        size_t N = m * n;
        for(size_t i = 0; i < N; ++i) {
            verts[i] = add_vertex(g);
        }
        induce_prism_graph(g, m, n, &verts[0], cycle, spokes);

    }

    template <class Graph, class CycleDirection, class SpokeDirection>
    inline void
    make_prism_graph(Graph& g, size_t m, size_t n,
                     CycleDirection cycle,
                     SpokeDirection spoke)
    {
        typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
        std::vector<Vertex> verts(m * n);
        make_prism_graph(g, m, n, verts, cycle, spoke);
    }

    template <class Graph>
    inline void
    make_prism_graph(Graph& g, size_t m, size_t n)
    {
        make_prism_graph(g, m, n,
                         with_clockwise_cycle(),
                         with_outward_spokes());
    }
}

#endif
