
#ifndef BOOST_GRAPHS_ADJLIST_VERTEX_MAP_HPP
#define BOOST_GRAPHS_ADJLIST_VERTEX_MAP_HPP

#include <map>

#include <boost/descriptors.hpp>
#include <boost/graphs/adjacency_list/vertex_iterator.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

// Forward declarations
template <typename, typename, typename, typename> class vertices_map;

/**
 * @param Key The key type of the vertex map can be any LessThanComparable type.
 * @param Compare A unary template class that implements a comparison of keys.
 * @param Alloc An allocator for key/value pairs of the underliyng map.
 */
template <
        typename Key,
        template <typename> class Compare = std::less,
        template <typename> class Alloc = std::allocator>
struct vertex_map
{
    typedef Key key_type;

    typedef typename descriptor_traits<
        std::map<Key, int, Compare<Key>, Alloc<std::pair<Key, int>>>
    >::descriptor_type vertex_descriptor;

    template <typename Vertex>
    struct store
    {
        typedef Alloc<std::pair<Key, Vertex>> allocator;
        typedef Compare<Key> compare;
        typedef vertices_map<Vertex, Key, compare, allocator> type;
    };
};

} } } /* namespace boost::graphs::adjacency_list */

#endif
