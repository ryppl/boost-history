
#ifndef BOOST_GRAPHS_ADJLIST_IES_LIST_HPP
#define BOOST_GRAPHS_ADJLIST_IES_LIST_HPP

#include <boost/counted_list.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

template <template <typename> class Alloc = std::allocator>
struct incidence_list
{
    template <typename EdgeDesc>
    struct store
    {
    private:
        typedef Alloc<EdgeDesc> allocator;
    public:
        typedef counted_list<EdgeDesc, allocator> type;
    };
};

} } }

#endif


