
#ifndef BOOST_GRAPHS_ADJLIST_VS_LIST_HPP
#define BOOST_GRAPHS_ADJLIST_VS_LIST_HPP

#include <boost/counted_list.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

/**
 * @param Alloc A unary template class that will allocate stored vertices.
 */
template <template <typename> class Alloc = std::allocator>
struct vertex_list
{
    typedef void key_type;

    typedef typename descriptor_traits<
        std::list<int, Alloc<int>>
    >::descriptor_type vertex_descriptor;

    template <typename Vertex>
    struct vertex_store
    {
        typedef Alloc<Vertex> allocator;
        typedef counted_list<Vertex, allocator> type;
    };
};


} } } /* namespace boost::graphs::adjacency_list */

#endif

