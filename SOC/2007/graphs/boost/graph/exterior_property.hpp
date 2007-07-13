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

namespace boost
{
    namespace detail
    {
        // These metafunctions are used to decipher the associative strategy
        // of graphs (i.e., how to associate a vertex or edge with a property).
        // Unfortunatly, this isn't made explicit through any means I know of.
        // However, we do know that vector-based storage (at least for vertices)
        // tends to favor std::size as descriptors and uses those to index
        // property vectors. Otherwise, the descriptor is generally a void-cast
        // pointer to the stored element.

        // TODO: Edge descriptors are a little different. They may be required to
        // be in associative maps regardless of actual type (maybe). There's not
        // a single example of using exterior edge properties in Boost.Graph.

        template <typename Vertex, typename Value>
        struct choose_ext_vprop_container
        {
            typedef typename mpl::if_<
                    is_same<Vertex, std::size_t>,
                    std::vector<Value>,
                    std::tr1::unordered_map<Vertex, Value>
                >::type type;
        };

        template <typename Graph, typename Container>
        struct choose_ext_vprop_map
        {
            typedef typename graph_traits<Graph>::vertex_descriptor vertex_type;
            typedef typename mpl::if_<
                    is_same<vertex_type, std::size_t>,
                    iterator_property_map<
                        typename Container::iterator,
                        typename property_map<Graph, vertex_index_t>::type>,
                    associative_property_map<Container>
                >::type type;
        };
    };

    template <typename Graph, typename Value>
    struct exterior_vertex_property
    {
        typedef Value value_type;
        typedef typename graph_traits<Graph>::vertex_descriptor key_type;

        typedef typename
            detail::choose_ext_vprop_container<key_type, value_type>::type
            container_type;

        typedef typename
            detail::choose_ext_vprop_map<Graph, container_type>::type
            map_type;
    };

    // These functions are needed to abstract the initialization sequence.
    // If you know the actual container type of your exerior property, then
    // you skip these functions. If you don't and you're declaring these
    // generically, then you _must_ use these to ensure correct initialiation
    // of the property map.

    template <typename Key, typename Value>
    static inline std::tr1::unordered_map<Key, Value>&
    make_property_map(std::tr1::unordered_map<Key, Value>& c)
    { return c; }

    template <typename Value>
    static inline typename std::vector<Value>::iterator
    make_property_map(std::vector<Value>& c)
    { return c.begin(); }
}

#endif
