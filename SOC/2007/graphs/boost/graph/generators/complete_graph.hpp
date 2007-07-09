// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_GENERATORS_COMPLETE_GRAPH_HXX
#define BOOST_GRAPH_GENERATORS_COMPLETE_GRAPH_HXX

#include <vector>
#include <boost/utility.hpp>
#include <boost/graph/graph_traits.hpp>

namespace boost
{
    namespace detail
    {
        template <typename Graph>
        inline void
        complete_edge(Graph& g,
                      typename graph_traits<Graph>::vertex_descriptor u,
                      typename graph_traits<Graph>::vertex_descriptor v,
                      directed_tag)
        {
            add_edge(u, v, g);
            add_edge(v, u, g);
        }

        template <typename Graph>
        inline void
        complete_edge(Graph& g,
                      typename graph_traits<Graph>::vertex_descriptor u,
                      typename graph_traits<Graph>::vertex_descriptor v,
                      undirected_tag)
        {
            add_edge(u, v, g);
        }
    }

    template <class Graph, class RandomAccessContainer>
    inline void
    make_complete_graph(Graph& g, size_t k, RandomAccessContainer& verts)
    {
        typename graph_traits<Graph>::directed_category cat;
        for(size_t i = 0; i < k; ++i) {
            verts[i] = add_vertex(g);
        }
        for(size_t i = 0; i < k; ++i) {
            for(size_t j = i + 1; j < k; ++j) {
                detail::complete_edge(g, verts[i], verts[j], cat);
            }
        }
    }

    template <class Graph>
    inline void
    make_complete_graph(Graph& g, size_t k)
    {
        typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
        std::vector<Vertex> verts(k);
        make_complete_graph(g, k, verts);
    }

}

#endif
