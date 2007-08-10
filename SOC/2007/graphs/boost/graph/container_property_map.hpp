// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_CONTAINER_PROPERTY_MAP_HXX
#define BOOST_GRAPH_CONTAINER_PROPERTY_MAP_HXX

#include <boost/property_map.hpp>
#include <boost/graph/graph_traits.hpp>

namespace boost
{

    namespace detail
    {
        template <typename Graph>
        struct vertex_indexer
        {
            typedef vertex_index_t index_type;
            typedef typename property_map<Graph, vertex_index_t>::type map_type;
            typedef typename property_map<Graph, vertex_index_t>::const_type const_map_type;
            typedef typename property_traits<map_type>::value_type value_type;
            typedef typename graph_traits<Graph>::vertex_descriptor key_type;

            static const_map_type index_map(const Graph& g)
            { return get(vertex_index, g); }

            static map_type index_map(Graph& g)
            { return get(vertex_index, g); }

            static value_type index(key_type k, const Graph& g)
            { return get(vertex_index, g, k); }
        };

        template <typename Graph>
        struct edge_indexer
        {
            typedef edge_index_t index_type;
            typedef typename property_map<Graph, edge_index_t>::type map_type;
            typedef typename property_map<Graph, edge_index_t>::const_type const_map_type;
            typedef typename property_traits<map_type>::value_type value_type;
            typedef typename graph_traits<Graph>::edge_descriptor key_type;

            static const_map_type index_map(const Graph& g)
            { return get(edge_index, g); }

            static map_type index_map(Graph& g)
            { return get(edge_index, g); }

            static value_type index(key_type k, const Graph& g)
            { return get(edge_index, g, k); }
        };

        template <typename Graph, typename Key>
        struct choose_indexer
        {
            typedef typename mpl::if_<
                    is_same<Key, typename graph_traits<Graph>::vertex_descriptor>,
                    vertex_indexer<Graph>,
                    edge_indexer<Graph>
                >::type indexer_type;
            typedef typename indexer_type::index_type index_type;
        };
    }

    // This is an adapter built over the iterator property map with
    // more useful uniform construction semantics. Specifically, this
    // requires the container rather than the iterator and the graph
    // rather than the optional index map.
    template <typename Graph, typename Container, typename Key>
    struct container_property_map
        : public boost::put_get_helper<
                typename iterator_property_map<
                        typename Container::iterator,
                        typename property_map<
                                Graph,
                                typename detail::choose_indexer<Graph, Key>::index_type
                            >::type
                    >::reference,
                    container_property_map<Graph, Container, Key>
            >
    {
        typedef typename detail::choose_indexer<Graph, Key>::indexer_type indexer_type;
        typedef typename indexer_type::index_type index_type;
        typedef iterator_property_map<
                typename Container::iterator,
                typename property_map<Graph, index_type>::type
            > map_type;
        typedef typename map_type::key_type key_type;
        typedef typename map_type::value_type value_type;
        typedef typename map_type::reference reference;
        typedef typename map_type::category category;

        // The default constructor will *probably* crash if its actually
        // used for referencing vertices since the underlying iterator
        // map points past the end of an unknown container.
        inline container_property_map()
            : m_map()
        { }

        // This is the preferred constructor. It is invoked over the container
        // and the graph explicitly. This requires that the underlying iterator
        // map use the indices of the vertices in g rather than the default
        // identity map.
        //
        // Note the const-cast this ensures the reference type of the
        // vertex index map is non-const, which happens to be an
        // artifact of passing const graph references.
        inline container_property_map(Container& c, const Graph& g)
            : m_map(c.begin(), indexer_type::index_map(const_cast<Graph&>(g)))
        { }

        // Typical copy constructor.
        inline container_property_map(const container_property_map& x)
            : m_map(x.m_map)
        { }

        // The [] operator delegates to the underlying map/
        inline reference operator [](const key_type& k) const
        { return m_map[k];  }

        map_type m_map;
    };
}

#endif
