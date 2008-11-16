
#ifndef BOOST_GRAPHS_ADJLIST_VERTEX_LIST_HPP
#define BOOST_GRAPHS_ADJLIST_VERTEX_LIST_HPP

#include <list>

#include <boost/none.hpp>
#include <boost/counted_list.hpp>
#include <boost/graphs/adjacency_list/vertex_store.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

// Forward declarations
template <typename, typename> class vertices_list;

/**
 * @param Alloc A unary template class that will allocate stored vertices.
 */
template <template <typename> class Alloc = std::allocator>
struct vertex_list
{
    typedef unused key_type;

    typedef typename descriptor_traits<
        std::list<int, Alloc<int>>
    >::descriptor_type vertex_descriptor;

    template <typename Vertex>
    struct vertex_store
    {
        typedef Alloc<Vertex> allocator_type;
        typedef counted_list<Vertex, allocator_type> type;
    };
};

template <typename T, typename A>
struct vertex_store_traits<counted_list<T,A>>
{
private:
    typedef counted_list<T,A> base_type;
public:
    typedef typename base_type::iterator store_iterator;

    typedef typename base_type::size_type                           vertices_size_type;
    typedef typename descriptor_traits<base_type>::descriptor_type  vertex_descriptor;
    typedef basic_vertex_iterator<base_type>                        vertex_iterator;
    typedef std::pair<vertex_iterator, vertex_iterator>             vertex_range;
};

} } } /* namespace boost::graphs::adjacency_list */

#endif

