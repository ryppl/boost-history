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
        inline typename graph_traits<Graph>::degree_size_type
        possible_edges(const Graph& g, std::size_t k, directed_tag)
        {
            typedef typename graph_traits<Graph>::degree_size_type T;
            return T(k) * (T(k) - 1);
        }

        template <class Graph>
        inline typename graph_traits<Graph>::degree_size_type
        possible_edges(const Graph& g, size_t k, undirected_tag)
        {
            // dirty little trick...
            return possible_edges(g, k, directed_tag()) / 2;
        }

        // This template matches directedS and bidirectionalS.
        template <class Graph>
        inline typename graph_traits<Graph>::degree_size_type
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
        inline typename graph_traits<Graph>::degree_size_type
        count_edges(const Graph& g,
                    typename Graph::vertex_descriptor u,
                    typename Graph::vertex_descriptor v,
                    undirected_tag)
        {
            return edge(u, v, g).second ? 1 : 0;
        }
    }

    template <typename Graph, typename Vertex>
    inline typename graph_traits<Graph>::degree_size_type
    vertex_num_routes(const Graph& g, Vertex v)
    {
        typename graph_traits<Graph>::directed_category cat;
        typename graph_traits<Graph>::adjacency_iterator i, end;
        tie(i, end) = adjacent_vertices(v, g);
        std::size_t k = std::distance(i, end);
        return detail::possible_edges(g, k, cat);
    }

    template <typename Graph, typename Vertex>
    inline typename graph_traits<Graph>::degree_size_type
    vertex_num_triangles(const Graph& g, Vertex v)
    {
        typedef typename graph_traits<Graph>::degree_size_type T;
        T count = 0;
        typename graph_traits<Graph>::directed_category cat;
        typename graph_traits<Graph>::adjacency_iterator i, j, end;
        for(tie(i, end) = adjacent_vertices(v, g); i != end; ++i) {
            for(j = next(i); j != end; ++j) {
                count += detail::count_edges(g, *i, *j, cat);
            }
        }
        return count;
    }

    template <typename T, typename Graph, typename Vertex>
    inline T
    vertex_clustering_coefficient(const Graph& g, Vertex v)
    {
        T zero(0);
        T routes = T(vertex_num_routes(g, v));
        return (routes > zero) ?
            T(vertex_num_triangles(g, v)) / routes : zero;
    }

    template <typename Graph, typename Vertex>
    inline float
    vertex_clustering_coefficient(const Graph& g, Vertex v)
    {
        return vertex_clustering_coefficient<float>(g, v);
    }

    template <typename Graph, typename ClusteringMap>
    inline void
    clustering_coefficient(const Graph& g, ClusteringMap cluster)
    {
        typedef typename property_traits<ClusteringMap>::value_type T;
        typename graph_traits<Graph>::vertex_iterator i, end;
        for(tie(i, end) = vertices(g); i != end; ++i) {
            cluster[*i] = vertex_clustering_coefficient<T>(g, *i);
        }
    }

    template <typename T, typename Graph>
    inline T
    graph_clustering_coefficient(const Graph& g)
    {
        T cc(0);
        typename graph_traits<Graph>::vertex_iterator i, end;
        for(tie(i, end) = vertices(g); i != end; ++i) {
            cc += vertex_clustering_coefficient<T>(g, *i);
        }
        return cc / T(num_vertices(g));
    }

    template <typename Graph>
    inline float
    graph_clustering_coefficient(const Graph& g)
    {
        return graph_clustering_coefficient<float>(g);
    }
}

#endif
