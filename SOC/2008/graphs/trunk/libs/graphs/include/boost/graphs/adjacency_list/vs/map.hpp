
#ifndef BOOST_GRAPHS_ADJLIST_VS_MAP_HPP
#define BOOST_GRAPHS_ADJLIST_VS_MAP_HPP

#include <map>

#include <boost/none.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

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
    struct vertex_store
    {
        typedef Alloc<std::pair<Key, Vertex>> allocator;
        typedef Compare<Key> compare;
        typedef std::map<Key, Vertex, compare, allocator> type;
    };
};

// Specialize the vertex store traits to refer to the mapped type instead
// of the value type.
template <typename Key, typename Value, typename Compare, typename Alloc>
struct vertex_store_traits<std::map<Key, Value, Compare, Alloc>>
{
    typedef typename std::map<Key, Value, Compare, Alloc>::mapped_type vertex_type;
};

} } } /* namespace boost::graphs::adjacency_list */

#endif
