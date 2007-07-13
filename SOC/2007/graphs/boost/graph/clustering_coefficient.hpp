// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_CLUSTERING_COEFFICIENT_HXX
#define BOOST_GRAPH_CLUSTERING_COEFFICIENT_HXX

#include <boost/utility.hpp>
#include <boost/graph/graph_traits.hpp>

namespace boost
{
    namespace detail
    {
        template <class Graph>
        inline size_t
        num_possible_edges(const Graph& g, size_t k, directed_tag)
        {
            return k * (k - 1);
        }

        template <class Graph>
        inline size_t
        num_possible_edges(const Graph& g, size_t k, undirected_tag)
        {
            return k * (k - 1) / 2;
        }

        // This template matches directedS and bidirectionalS.
        template <class Graph>
        inline size_t
        count_edges(const Graph& g,
                    typename Graph::vertex_descriptor u,
                    typename Graph::vertex_descriptor v,
                    directed_tag)

        {
            return (edge(u, v, g).second ? 1 : 0) +
                   (edge(v, u, g).second ? 1 : 0);
        }

        // This template matches undirectedS
        template <class Graph>
        inline size_t
        count_edges(const Graph& g,
                    typename Graph::vertex_descriptor u,
                    typename Graph::vertex_descriptor v,
                    undirected_tag)
        {
            return edge(u, v, g).second ? 1 : 0;
        }
    }

    template <typename Graph>
    inline size_t
    num_centered_triples(const Graph& g,
                        typename Graph::vertex_descriptor v)
    {
        // find all of the adjacent vertices
        typename graph_traits<Graph>::directed_category cat;
        typename graph_traits<Graph>::adjacency_iterator i, end;
        tie(i, end) = adjacent_vertices(v, g);
        size_t k = std::distance(i, end);

        size_t ret = detail::num_possible_edges(g, k, cat);
        return ret;
    }

    // This is seriously flawed for directed graphs...
    // Adjacenct vertices correspond to out degrees.

    template <typename Graph>
    inline size_t
    num_centered_triangles(const Graph& g,
                           typename Graph::vertex_descriptor v)
    {
        size_t count = 0;
        typename graph_traits<Graph>::directed_category cat;
        typename graph_traits<Graph>::adjacency_iterator i, j, end;
        for(tie(i, end) = adjacent_vertices(v, g); i != end; ++i) {
            for(j = next(i); j != end; ++j) {
                count += detail::count_edges(g, *i, *j, cat);
            }
        }
        return count;
    }

    template <typename Graph>
    double
    clustering_coefficient(const Graph& g,
                           typename Graph::vertex_descriptor v)
    {
        double ret = 0.0;
        double triples = (double)num_centered_triples(g, v);
        if(triples > 0.0) {
            ret = (double)num_centered_triangles(g, v) / triples;
        }
        return ret;
    }

    template <typename Graph>
    double
    clustering_coefficient(const Graph& g)
    {
        double cc = 0.0;
        typename Graph::vertex_iterator i, end;
        for(tie(i, end) = vertices(g); i != end; ++i) {
            cc += clustering_coefficient(g, *i);
        }
        return cc / (double)num_vertices(g);
    }
}

#endif
