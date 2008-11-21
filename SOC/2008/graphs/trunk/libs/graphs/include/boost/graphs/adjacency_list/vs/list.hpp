
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

    template <typename Edges, typename Label>
    struct store
    {
    private:
        typedef std::pair<Edges, Label> vertex;
        typedef Alloc<vertex> allocator;
    public:
        typedef counted_list<vertex, allocator> type;
    };
};


} } } /* namespace boost::graphs::adjacency_list */

#endif

