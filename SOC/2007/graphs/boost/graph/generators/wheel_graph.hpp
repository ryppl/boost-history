// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_GENERATORS_WHEEL_GRAPH_HXX
#define BOOST_GRAPH_GENERATORS_WHEEL_GRAPH_HXX

#include <vector>
#include <boost/utility.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/generators/options.hpp>
#include <boost/graph/generators/cycle_graph.hpp>
#include <boost/graph/generators/star_graph.hpp>

namespace boost
{
    template <
        class Graph,
        class ForwardIterator,
        class CycleDirection,
        class SpokeDirection
    >
    inline ForwardIterator
    induce_wheel_graph(Graph& g, size_t n,
                       ForwardIterator iter,
                       CycleDirection cycle,
                       SpokeDirection spokes)
    {
        // this should be easy... induce S(n) with iter as the root
        // of the star and then induce C(n - 1) with next(iter) as
        // the start of the cycle
        induce_star_graph(g, n, iter, spokes);
        induce_cycle_graph(g, n - 1, next(iter), cycle);

        // return the center of the wheel
        return iter;
    }

    template <
        class Graph,
        class RandomAccessContainer,
        class CycleDirection,
        class SpokeDirection
    >
    inline void
    make_wheel_graph(Graph& g, size_t n,
                     RandomAccessContainer& verts,
                     CycleDirection cycle,
                     SpokeDirection spokes)
    {
        for(size_t i = 0; i < n; ++i) {
            verts[i] = add_vertex(g);
        }
        induce_wheel_graph(g, n, &verts[0], cycle, spokes);
    }

    template <
        class Graph,
        class CycleDirection,
        class SpokeDirection
    >
    inline void
    make_wheel_graph(Graph& g, size_t n,
                     CycleDirection cycle,
                     SpokeDirection spokes)
    {
        typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
        std::vector<Vertex> verts(n);
        make_wheel_graph(g, n, verts, cycle, spokes);
    }

    template <class Graph>
    inline void
    make_wheel_graph(Graph& g, size_t n)
    {
        make_wheel_graph(g, n,
                         with_clockwise_cycle(),
                         with_outward_spokes());
    }
}

#endif
