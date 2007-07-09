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
    template <
        class Graph,
        class RandomAccessContainer,
        class SpokeDirection
    >
    inline void
    make_star_graph(Graph& g, size_t k,
                    RandomAccessContainer& verts,
                    SpokeDirection spokes)
    {
        for(size_t i = 0; i < k; ++i) {
            verts[i] = add_vertex(g);
        }
        for(size_t i = 1; i < k - 1; ++i) {
            detail::connect_spoke_vertices(g, verts[0], verts[i], spokes);
        }
        detail::connect_spoke_vertices(g, verts[0], verts[k - 1], spokes);
    }

    template <class Graph, class SpokeDirection>
    inline void
    make_star_graph(Graph& g, size_t k, SpokeDirection spokes)
    {
        typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
        std::vector<Vertex> verts(k);
        make_star_graph(g, k, verts, spokes);
    }

    template <class Graph>
    inline void
    make_star_graph(Graph& g, size_t k)
    {
        typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
        std::vector<Vertex> verts(k);
        make_star_graph(g, k, verts, with_outward_spokes());
    }
}

#endif
