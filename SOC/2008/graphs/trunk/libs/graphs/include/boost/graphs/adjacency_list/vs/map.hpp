
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
        std::map<Key, int, Compare<Key>, Alloc<std::pair<Key, int>>>,
        vertex_descriptor_kind
    >::descriptor_type vertex_descriptor;

    template <typename Edges, typename Label>
    struct store
    {
    private:
        typedef std::pair<Edges, Label> vertex;
        typedef Compare<Key> compare;
        typedef Alloc<std::pair<Key, vertex>> allocator;
    public:
        typedef std::map<Key, vertex, compare, allocator> type;
    };
};

} } } /* namespace boost::graphs::adjacency_list */

#endif
