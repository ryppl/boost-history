
#ifndef BOOST_GRAPHS_ADJLIST_IES_VECTOR_HPP
#define BOOST_GRAPHS_ADJLIST_IES_VECTOR_HPP

#include <vector>

namespace boost { namespace graphs { namespace adjacency_list {

template <template <typename> class Alloc = std::allocator>
struct incidence_vector
{
    template <typename EdgeDesc>
    struct store
    {
    private:
        typedef Alloc<EdgeDesc> allocator;
    public:
        typedef std::vector<EdgeDesc, allocator> type;
    };
};

} } }

#endif


