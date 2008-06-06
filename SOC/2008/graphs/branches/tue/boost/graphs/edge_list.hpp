
#ifndef EDGE_LIST_HPP
#define EDGE_LIST_HPP

#include "property_list.hpp"
#include "incidence_list.hpp"

// The edge list does two things. First, it indicates that edges will
// be stored in an incidence list (as opposed to a vector or set).
// Second, this will indicate the use of a global property list.
// Again, as opposed to a vector.

template <
    template <typename> class IncAlloc,
    template <typename> class PropAlloc>
struct basic_edge_list
{
    template <typename EdgeProps>
    struct property_store
    {
        typedef property_list<EdgeProps, PropAlloc<EdgeProps> > type;
    };

    template <typename VertexDesc, typename PropDesc>
    struct incidence_store
    {
        typedef std::pair<VertexDesc, PropDesc> incidence_pair;
        typedef incidence_list<incidence_pair, IncAlloc<incidence_pair> > type;
    };
};

struct edge_list : basic_edge_list<std::allocator, std::allocator> { };

#endif

