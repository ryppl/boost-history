// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_GENERATORS_CYCLE_GRAPH_HXX
#define BOOST_GRAPH_GENERATORS_CYCLE_GRAPH_HXX

#include <vector>
#include <boost/utility.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/generators/options.hpp>
#include <boost/graph/generators/path_graph.hpp>

namespace boost
{
    template <typename Graph,class ForwardIterator, class CycleDirection>
    inline ForwardIterator
    induce_cycle_graph(Graph& g, size_t n,
                       ForwardIterator iter,
                       CycleDirection cycle)
    {
        typename detail::edge_gen_policy<CycleDirection>::creation_policy policy;

        // start by inducing a path, saving the last iterator in the chain
        ForwardIterator last = induce_path_graph(g, n, iter, policy);

        // connect the last edge back to the beginning
        detail::generate_edge(g, *last, *iter, cycle);

        // last element added to the cycle
        return last;
    }

    template <class Graph, class RandomAccessContainer, class CycleDirection>
    inline void
    make_cycle_graph(Graph& g, size_t n,
                     RandomAccessContainer& verts,
                     CycleDirection cycle)
    {
        for(size_t i = 0; i < n; ++i) {
            verts[i] = add_vertex(g);
        }
        induce_cycle_graph(g, n, &verts[0], cycle);
    }

    template <class Graph, class CycleDirection>
    inline void
    make_cycle_graph(Graph& g, size_t k, CycleDirection cycle)
    {
        typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
        std::vector<Vertex> verts(k);
        make_cycle_graph(g, k, verts, cycle);
    }

    template <class Graph>
    inline void
    make_cycle_graph(Graph& g, size_t k)
    {
        typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
        std::vector<Vertex> verts(k);
        make_cycle_graph(g, k, verts, with_clockwise_cycle());
    }
}

#endif
