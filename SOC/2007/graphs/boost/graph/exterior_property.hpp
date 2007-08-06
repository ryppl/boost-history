// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_EXTERIOR_PROPERTY_HPP
#define BOOST_GRAPH_EXTERIOR_PROPERTY_HPP

#include <vector>
#include <boost/utility.hpp>
#include <boost/graph/container_property_map.hpp>

namespace boost
{
    namespace detail
    {
        // The vector matrix provides a little abstraction over vector
        // types that makes matrices easier to work with. Note that it's
        // non-copyable, meaning you should be passing it by value.
        template <typename Graph, typename Key, typename Value>
        struct vector_matrix
        {
            typedef std::size_t size_type;
            typedef Key key_type;
            typedef Value value_type;

            typedef std::vector<value_type> container_type;
            typedef std::vector<container_type> matrix_type;
            typedef container_property_map<Graph, container_type, Key> map_type;
            typedef typename map_type::indexer_type indexer_type;

            // Instantiate the matrix over n elements (creates an nxn matrix).
            // The graph has to be passed in order to ensure the index maps
            // are constructed correctly when returning indexible elements.
            inline vector_matrix(size_type n, const Graph& g)
                : m_matrix(n, container_type(n))
                , m_graph(g)
            { }

            inline map_type operator [](key_type k)
            { return map_type(m_matrix[indexer_type::index(k, m_graph)], m_graph); }

            inline map_type operator [](key_type k) const
            { return map_type(m_matrix[indexer_type::index(k, m_graph)], m_graph); }

            mutable matrix_type m_matrix;
            const Graph& m_graph;
        };
    }

    template <typename Graph, typename Value>
    struct exterior_vertex_property
    {
        typedef typename graph_traits<Graph>::vertex_descriptor key_type;
        typedef Value value_type;
        typedef std::vector<Value> container_type;
        typedef detail::vector_matrix<Graph, key_type, Value> matrix_type;
        typedef container_property_map<Graph, container_type, key_type> map_type;
    };

    template <typename Graph, typename Value>
    struct exterior_edge_property
    {
        typedef typename graph_traits<Graph>::edge_descriptor key_type;
        typedef Value value_type;
        typedef std::vector<Value> container_type;
        typedef detail::vector_matrix<Graph, key_type, Value> matrix_type;
        typedef container_property_map<Graph, container_type, key_type> map_type;
    };

    template <typename Matrix>
    struct property_matrix_traits
    {
        typedef typename Matrix::matrix_type matrix_type;
        typedef typename Matrix::container_type container_type;
        typedef typename Matrix::value_type value_type;
        typedef typename Matrix::key_type key_type;
    };
}

#endif
