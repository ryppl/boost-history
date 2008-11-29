
#ifndef BOOST_GRAPHS_ADJLIST_IES_LIST_HPP
#define BOOST_GRAPHS_ADJLIST_IES_LIST_HPP

#include <boost/counted_list.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

template <template <typename> class Alloc = std::allocator>
struct incidence_list
{
    template <typename Vertex, typename Edge>
    struct store
    {
    private:
        typedef std::pair<Vertex, Edge> edge;
        typedef Alloc<edge> allocator;
    public:
        typedef counted_list<edge, allocator> type;
    };
};

} } }

#endif


