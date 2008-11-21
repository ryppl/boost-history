
#ifndef BOOST_GRAPHS_ADJLIST_ES_SET_HPP
#define BOOST_GRAPHS_ADJLIST_ES_SET_HPP

#include <map>

#include <boost/graphs/edge.hpp>

namespace boost { namespace graphs { namespace adjacency_list {


/**
 * @param Compare A unary template class that compares the ends of edges. The
 * argument type for this function is Pair<VertexDesc, VertexDesc>.
 * @param Alloc A unary template class that allocates edges.
 */
template <
    template <typename> class Compare = std::less,
    template <typename> class Alloc = std::allocator>
struct edge_set
{
    typedef typename descriptor_traits<
        std::set<int, Compare<int>, Alloc<int>>
    >::descriptor_type edge_descriptor;

    // This quietly implements a map, not a set because we'll be using the
    // endpoints as keys to the label.
    template <typename Vertex, typename Label>
    struct store
    {
        typedef std::pair<Vertex, Vertex> ends;
        typedef Compare<ends> compare;
        typedef Alloc<std::pair<ends, Label>> allocator;
    public:
        typedef std::map<ends, Label, compare, allocator> type;
    };
};

} } } /* namespace boost::graphs::adjacency_list */

#endif
