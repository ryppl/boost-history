
#ifndef BOOST_GRAPHS_ADJLIST_VERTEX_VECTOR_HPP
#define BOOST_GRAPHS_ADJLIST_VERTEX_VECTOR_HPP

#include <vector>
#include <algorithm>

#include <boost/none.hpp>
#include <boost/graphs/adjacency_list/vertex_store.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

// Forward declarations
template <typename, typename> struct vertices_vector;

/**
 * @param Alloc A unary template class that will allocate stored vertices.
 */
template <template <typename> class Alloc = std::allocator>
struct vertex_vector
{
    typedef unused key_type;

    typedef typename descriptor_traits<
        std::vector<int, Alloc<int>>
    >::descriptor_type vertex_descriptor;

    // The store metafunction generates the type used to store vertices in
    // either a directed or undirected graph. This metafunction takes the
    // fully configured vertex type as a type parameter.
    template <typename Vertex>
    struct vertex_store
    {
        typedef Alloc<Vertex> allocator_type;
        typedef std::vector<Vertex, allocator_type> type;
    };
};

template <typename T, typename A>
struct vertex_store_traits<std::vector<T,A>>
{
private:
    typedef std::vector<T,A> base_type;
public:
    typedef typename base_type::iterator store_iterator;

    typedef typename base_type::size_type                           vertices_size_type;
    typedef typename descriptor_traits<base_type>::descriptor_type  vertex_descriptor;
    typedef basic_vertex_iterator<base_type>                        vertex_iterator;
    typedef std::pair<vertex_iterator, vertex_iterator>             vertex_range;
};

} } } /* namespace boost::graphs::adjacency_list */

#endif
