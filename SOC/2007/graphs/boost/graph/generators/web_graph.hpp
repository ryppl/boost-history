// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_GENERATORS_WEB_GRAPH_HXX
#define BOOST_GRAPH_GENERATORS_WEB_GRAPH_HXX

#include <vector>
#include <boost/utility.hpp>
#include <boost/graph/generators/options.hpp>
#include <boost/graph/generators/cycle_graph.hpp>

namespace boost
{
    // Web graphs G = W(m, n) is the same as the prism graph P(m, n + 1) with
    // the edges that comprise the outer cycle removed. This leaves m spokes
    // extending from the graph. Note that these graphs will have m * n + 1
    // vertices.

    template <
        class Graph,
        class RandomAccessIterator,
        class CycleDirection,
        class SpokeDirection
    >
    inline RandomAccessIterator
    induce_web_graph(Graph& g, size_t m, size_t n,
                      RandomAccessIterator iter,
                      CycleDirection cycle,
                      SpokeDirection spokes)
    {
        typedef RandomAccessIterator iterator;

        // induce the prism on all but the last m vertices
        induce_prism_graph(g, m, n, iter, cycle, spokes);

        // connect the peripheral vertices to the outermost cycle
        for(size_t i = 0; i < m; ++i) {
            iterator u = iter + i + m * (n - 1);
            iterator v = iter + i + m * (n);
            detail::generate_edge(g, *u, *v, spokes);
        }

        return iter;
    }


    template <
        class Graph,
        class RandomAccessContainer,
        class CycleDirection,
        class SpokeDirection
    >
    inline void
    make_web_graph(Graph& g, size_t m, size_t n,
                   RandomAccessContainer& verts,
                   CycleDirection cycle,
                   SpokeDirection spokes)
    {
        size_t N = m * (n + 1);
        for(size_t i = 0; i < N; ++i) {
            verts[i] = add_vertex(g);
        }
        induce_web_graph(g, m, n, &verts[0], cycle, spokes);
    }

    template <class Graph, class CycleDirection, class SpokeDirection>
    inline void
    make_web_graph(Graph& g, size_t m, size_t n,
                     CycleDirection cycle,
                     SpokeDirection spoke)
    {
        typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
        std::vector<Vertex> verts(m * (n + 1));
        make_web_graph(g, m, n, verts, cycle, spoke);
    }

    template <class Graph>
    inline void
    make_web_graph(Graph& g, size_t m, size_t n)
    {
        make_web_graph(g, m, n,
                       with_clockwise_cycle(),
                       with_outward_spokes());
    }
}

#endif
