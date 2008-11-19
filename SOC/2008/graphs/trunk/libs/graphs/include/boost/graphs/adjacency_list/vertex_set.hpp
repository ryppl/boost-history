
#ifndef BOOST_GRAPHS_ADJLIST_VERTEX_SET_HPP
#define BOOST_GRAPHS_ADJLIST_VERTEX_SET_HPP

#include <set>

#include <boost/none.hpp>
#include <boost/graphs/label.hpp>
#include <boost/graphs/adjacency_list/vertex_store.hpp>

namespace boost { namespace graphs { namespace adjacency_list {


/**
 * @param Compare A unary template class that compares vertex label.
 * @param Alloc A unary template class that allocates vertices.
 */
template <
    template <typename> class Compare = std::less,
    template <typename> class Alloc = std::allocator>
struct vertex_set
{
    typedef unused key_type;

    typedef typename descriptor_traits<
        std::set<int, Compare<int>, Alloc<int>>
    >::descriptor_type vertex_descriptor;

    template <typename Vertex>
    struct vertex_store
    {
    private:
        typedef Alloc<Vertex> allocator;
        typedef typename label_traits<Vertex>::label_type label_type;
        typedef labelled_compare<Vertex, Compare<label_type>> compare;
    public:
        typedef std::set<Vertex, compare, allocator> type;
    };
};

} } } /* namespace boost::graphs::adjacency_list */

#endif
