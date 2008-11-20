
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
        std::vector<int, Alloc<int>>
    >::descriptor_type vertex_descriptor;

    // The store metafunction generates the type used to store vertices in
    // either a directed or undirected graph. This metafunction takes the
    // fully configured vertex type as a type parameter.
    template <typename Vertex>
    struct vertex_store
    {
        typedef Alloc<Vertex> allocator;
        typedef std::vector<Vertex, allocator> type;
    };
};

} } } /* namespace boost::graphs::adjacency_list */

#endif
