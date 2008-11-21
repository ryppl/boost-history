
#ifndef BOOST_GRAPHS_ADJLIST_IES_SET_HPP
#define BOOST_GRAPHS_ADJLIST_IES_SET_HPP

#include <set>

namespace boost { namespace graphs { namespace adjacency_list {

template <
    template <typename> class Compare = std::less,
    template <typename> class Alloc = std::allocator>
struct incidence_set
{
    template <typename EdgeDesc>
    struct store
    {
    private:
        typedef Compare<EdgeDesc> compare;
        typedef Alloc<EdgeDesc> allocator;
    public:
        typedef std::set<EdgeDesc, compare, allocator> type;
    };
};

} } }

#endif


