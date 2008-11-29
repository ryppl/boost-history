
#ifndef BOOST_GRAPHS_ADJLIST_IES_VECTOR_HPP
#define BOOST_GRAPHS_ADJLIST_IES_VECTOR_HPP

#include <vector>

namespace boost { namespace graphs { namespace adjacency_list {

template <template <typename> class Alloc = std::allocator>
struct incidence_vector
{
    template <typename Vertex, typename Edge>
    struct store
    {
    private:
        typedef std::pair<Vertex, Edge> edge;
        typedef Alloc<edge> allocator;
    public:
        typedef std::vector<edge, allocator> type;
    };
};

} } }

#endif


