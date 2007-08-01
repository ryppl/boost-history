// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_EXTERIOR_PROPERTY_HPP
#define BOOST_GRAPH_EXTERIOR_PROPERTY_HPP

#include <vector>
#include <tr1/unordered_map>

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <boost/property_map.hpp>

namespace boost
{
    namespace detail
    {
        // These property map adapters are basically here to provide a common
        // method for initializing the property maps. They also provide a
        // cast operator for returning the underlying mapping type.

        // Do we really /need/ to do this. Not really. There are otherways
        // to achieve syntactic conformity, but they're less graceful.

        template <typename Graph, typename Container>
        struct vector_property_map_adapter
            : public boost::put_get_helper<
                    typename iterator_property_map<
                            typename Container::iterator,
                            typename property_map<Graph, vertex_index_t>::type
                        >::reference,
                    vector_property_map_adapter<Graph, Container>
                >
        {
            typedef iterator_property_map<
                    typename Container::iterator,
                    typename property_map<Graph, vertex_index_t>::type
                > map_type;
            typedef typename map_type::key_type key_type;
            typedef typename map_type::value_type value_type;
            typedef typename map_type::reference reference;
            typedef typename map_type::category category;

            inline vector_property_map_adapter()
                : m_map()
            { }

            inline vector_property_map_adapter(Container& c)
                : m_map(c.begin())
            { }

            inline vector_property_map_adapter(const vector_property_map_adapter& x)
                : m_map(x.m_map)
            { }

            inline reference operator [](const key_type& k) const
            { return m_map[k];  }

            map_type m_map;
        };

        template <typename Graph, typename Container>
        struct hash_property_map_adapter
            : public boost::put_get_helper<
                    typename associative_property_map<Container>::reference,
                    hash_property_map_adapter<Graph, Container>
                >
        {
            typedef associative_property_map<Container> map_type;
            typedef typename map_type::key_type key_type;
            typedef typename map_type::value_type value_type;
            typedef typename map_type::reference reference;
            typedef typename map_type::category category;

            inline hash_property_map_adapter()
                : m_map()
            { }

            inline hash_property_map_adapter(Container& c)
                : m_map(c)
            { }

            inline hash_property_map_adapter(const hash_property_map_adapter& x)
                : m_map(x.m_map)
            { }

            inline reference operator[](const key_type& k) const
            { return m_map[k]; }

            operator map_type() const
            { return m_map; }

            map_type m_map;
        };
    }

    namespace detail
    {
        // These structures implement very, very basic matrices
        // over vectors and hash tables. Like the adapters above, these
        // classes provide a degree of syntactic uniformity for their
        // declarations.

        template <typename Graph, typename Value>
        struct vector_matrix_adapter
        {
            typedef typename graph_traits<Graph>::vertices_size_type size_type;
            typedef typename graph_traits<Graph>::vertex_descriptor key_type;
            typedef Value value_type;

            typedef std::vector<value_type> container_type;
            typedef std::vector<container_type> matrix_type;

            inline vector_matrix_adapter(size_type n)
                : m_matrix(n, container_type(n))
            { }

            inline typename matrix_type::reference operator [](key_type k)
            { return m_matrix[k]; }

            inline typename matrix_type::reference operator [](key_type k) const
            { return m_matrix[k]; }

            mutable matrix_type m_matrix;
        };

        // There's a strange side-effect using the []'s of a hashtable in
        // that it's a modifying operation. If the key isn't found, this
        // will create a value for the key with the default value. However,
        // since we know that all the keys exist in the map, it shouldn't
        // be a big deal.
        //
        // Also, we can kind of skip the initialization of the underlying
        // data structures. there's going to be a bunch of resizing, but
        // in the long run, it may not hurt too much.
        template <typename Graph, typename Value>
        struct hash_matrix_adapter
        {
            typedef typename graph_traits<Graph>::vertices_size_type size_type;
            typedef typename graph_traits<Graph>::vertex_descriptor key_type;

            typedef Value value_type;
            typedef std::tr1::unordered_map<key_type, value_type> container_type;
            typedef std::tr1::unordered_map<key_type, container_type> matrix_type;

            inline hash_matrix_adapter(size_type n)
                : m_matrix(n)
            {
                typename matrix_type::iterator i, end = m_matrix.end();
                for(i = m_matrix.begin(); i != end; ++i) {
                    i->second.rehash(n);
                }
            }

            inline typename matrix_type::mapped_type& operator [](key_type k)
            { return m_matrix[k]; }

            inline typename matrix_type::mapped_type& operator [](key_type k) const
            { return m_matrix[k]; }

            mutable matrix_type m_matrix;
        };
}

    // This is very, very dirty. If the adjacency list implementation
    // has not been included at this point, we need to define
    // it's graph tag so we can specialize on it. I feel so unclean.
    // A better solution would be to migrate commonly used graph tags
    // like this to a separate header.
#ifndef BOOST_GRAPH_DETAIL_ADJACENCY_LIST_HPP
    struct vec_adj_list_tag { };
#endif

    // These structures associate a preferred mapping style
    // with a graph type.
    struct vector_mapped_tag {};
    struct hash_mapped_tag {};

    template <typename Graph, typename Value>
    struct vector_mapped_vertex_property_traits
    {
        typedef vector_mapped_tag mapping_type;
        typedef typename graph_traits<Graph>::vertex_descriptor key_type;
        typedef Value value_type;

        typedef typename std::vector<Value> container_type;
        typedef detail::vector_matrix_adapter<Graph, value_type> matrix_type;
        typedef detail::vector_property_map_adapter<Graph, container_type> map_type;
    };

    template <typename Graph, typename Value>
    struct hash_mapped_vertex_property_traits
    {
        typedef hash_mapped_tag mapping_type;
        typedef typename graph_traits<Graph>::vertex_descriptor key_type;
        typedef Value value_type;

        typedef typename std::tr1::unordered_map<key_type, value_type> container_type;
        typedef detail::hash_matrix_adapter<Graph, value_type> matrix_type;
        typedef detail::hash_property_map_adapter<Graph, container_type> map_type;
    };


    template <typename Graph, typename Value>
    struct exterior_vertex_property
    {
        typedef typename Graph::graph_tag graph_tag;
        typedef typename mpl::if_<
                is_same<graph_tag, vec_adj_list_tag>,
                vector_mapped_vertex_property_traits<Graph, Value>,
                hash_mapped_vertex_property_traits<Graph, Value>
            >::type traits_type;

        typedef typename traits_type::key_type key_type;
        typedef typename traits_type::value_type value_type;
        typedef typename traits_type::container_type container_type;
        typedef typename traits_type::matrix_type matrix_type;
        typedef typename traits_type::map_type map_type;
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
