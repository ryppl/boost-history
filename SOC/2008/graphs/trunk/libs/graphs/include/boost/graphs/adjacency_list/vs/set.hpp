
#ifndef BOOST_GRAPHS_ADJLIST_VS_SET_HPP
#define BOOST_GRAPHS_ADJLIST_VS_SET_HPP

#include <map>

#include <boost/graphs/label.hpp>

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
    typedef void key_type;

    typedef typename descriptor_traits<
        std::set<int, Compare<int>, Alloc<int>>,
        vertex_descriptor_kind
    >::descriptor_type vertex_descriptor;

    // Quietly define a set as a map from label (which is the key being
    // compared) to the vertex which is actually just the set of edges.
    template <typename Edges, typename Label>
    struct store
    {
    private:
        typedef Edges vertex;
        typedef Compare<Label> compare;
        typedef Alloc<std::pair<Label, vertex>> allocator;
    public:
        typedef std::map<Label, vertex, compare, allocator> type;
    };
};

} } } /* namespace boost::graphs::adjacency_list */

#endif
