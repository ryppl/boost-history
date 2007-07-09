// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_GENERATORS_PATH_GRAPH_HXX
#define BOOST_GRAPH_GENERATORS_PATH_GRAPH_HXX

#include <vector>
#include <boost/utility.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/generators/options.hpp>

namespace boost
{
    // The path graph P(n) is a tree (or chain). 

    template <typename Graph,class ForwardIterator, class EdgeDirection>
    inline ForwardIterator
    induce_path_graph(Graph& g, size_t n,
                      ForwardIterator iter,
                      EdgeDirection dir)
    {
        // iterate n - 1 times (edges in a chain)
        for(size_t i = 0; i < n - 1; ++i) {
            ForwardIterator p = iter++;
            detail::generate_edge(g, *p, *iter, dir);
        }
        return iter;
    }

    template <class Graph, class RandomAccessContainer, class EdgeDirection>
    inline void
    make_path_graph(Graph& g, size_t n,
                     RandomAccessContainer& verts,
                     EdgeDirection dir)
    {
        for(size_t i = 0; i < n; ++i) {
            verts[i] = add_vertex(g);
        }
        induce_path_graph(g, n, &verts[0], dir);
    }

    template <class Graph, class EdgeDirection>
    inline void
    make_path_graph(Graph& g, size_t n, EdgeDirection dir)
    {
        typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
        std::vector<Vertex> verts(n);
        make_path_graph(g, n, verts, dir);
    }

    template <class Graph>
    inline void
    make_path_graph(Graph& g, size_t n)
    {
        make_path_graph(g, n, with_forward_edges());
    }
}

#endif
