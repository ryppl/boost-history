// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_GENERATORS_STAR_GRAPH_HXX
#define BOOST_GRAPH_GENERATORS_STAR_GRAPH_HXX

#include <vector>
#include <boost/utility.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/generators/options.hpp>

namespace boost
{
    // As a variant, we could introduce S(m, n) where m is the number of radial
    // vertices and n is the length of paths from the center to the periphery.

    template <class Graph, class RandomAccessIterator, class SpokeDirection>
    inline void
    induce_star_graph(Graph& g, size_t n,
                      RandomAccessIterator iter,
                      SpokeDirection spokes)
    {
        for(size_t i = 1; i < n; ++i) {
            detail::add_spoke_edge(g, *iter, *(iter + i), spokes);
        }
    }

    template <class Graph, class RandomAccessContainer, class SpokeDirection>
    inline void
    make_star_graph(Graph& g, size_t n,
                    RandomAccessContainer& verts,
                    SpokeDirection spokes)
    {
        for(size_t i = 0; i < n; ++i) {
            verts[i] = add_vertex(g);
        }
        induce_star_graph(g, n, &verts[0], spokes);
    }

    template <class Graph, class SpokeDirection>
    inline void
    make_star_graph(Graph& g, size_t n, SpokeDirection spokes)
    {
        typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
        std::vector<Vertex> verts(n);
        make_star_graph(g, n, verts, spokes);
    }

    template <class Graph>
    inline void
    make_star_graph(Graph& g, size_t n)
    {
        make_star_graph(g, n, with_outward_spokes());
    }
}

#endif
