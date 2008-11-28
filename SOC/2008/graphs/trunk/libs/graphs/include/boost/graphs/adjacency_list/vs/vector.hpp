
#ifndef BOOST_GRAPHS_ADJLIST_VS_VECTOR_HPP
#define BOOST_GRAPHS_ADJLIST_VS_VECTOR_HPP

#include <vector>

namespace boost { namespace graphs { namespace adjacency_list {

/**
 * @param Alloc A unary template class that will allocate stored vertices.
 */
template <template <typename> class Alloc = std::allocator>
struct vertex_vector
{
    typedef void key_type;

    typedef typename descriptor_traits<
        std::vector<int, Alloc<int>>,
        vertex_descriptor_kind
    >::descriptor_type vertex_descriptor;

    template <typename Edges, typename Label>
    struct store
    {
    private:
        typedef std::pair<Edges, Label> vertex;
        typedef Alloc<vertex> allocator;
    public:
        typedef std::vector<vertex, allocator> type;
    };
};

} } } /* namespace boost::graphs::adjacency_list */

#endif
