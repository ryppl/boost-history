
#ifndef EDGE_SET_HPP
#define EDGE_SET_HPP

#include "property_list.hpp"

// An edge set denotes a) that incident edges are stored in a set for
// each vertex and b) that properties are stored in a list.

template <
    template <typename> class Compare,
    template <typename> class IncAlloc,
    template <typename> class PropAlloc>
struct basic_edge_set
{
    template <typename EdgeProps>
    struct property_store
    {
        typedef property_list<EdgeProps, PropAlloc<EdgeProps> > type;
    };
};

template <template <typename> class Compare = std::less>
struct edge_set : basic_edge_set<Compare, std::allocator, std::allocator> { };

#endif

