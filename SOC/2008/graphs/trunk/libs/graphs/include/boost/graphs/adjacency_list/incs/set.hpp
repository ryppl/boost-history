
#ifndef BOOST_GRAPHS_ADJLIST_IES_SET_HPP
#define BOOST_GRAPHS_ADJLIST_IES_SET_HPP

#include <map>

namespace boost { namespace graphs { namespace adjacency_list {

template <
    template <typename> class Compare = std::less,
    template <typename> class Alloc = std::allocator>
struct incidence_set
{
    // The incidence set quietly implements a map from an adjacent vertex to
    // the edge descriptor that references the underlying edge.
    template <typename Vertex, typename Edge>
    struct store
    {
    private:
        typedef Compare<Vertex> compare;
        typedef Alloc<std::pair<Vertex, Edge>> allocator;
    public:
        typedef std::map<Vertex, Edge, compare, allocator> type;
    };
};

} } }

#endif


