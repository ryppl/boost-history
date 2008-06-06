
#ifndef EDGE_SET_HPP
#define EDGE_SET_HPP

#include "property_list.hpp"
#include "incidence_set.hpp"

/**
 * The edge set metafunction defines the basic facets of set-based edge
 * storage. Edge sets are best used to implement simple graphs (that do not
 * allow multiple edges).
 *
 * There are three parmaters to this function:
 * Compare - A function that compares vertex descriptors
 * IncAlloc - An allocator for the edge set
 * PropAlloc - An allocator for the global property set.
 *
 * Note that the edge set uses a map for the incidence store.
 */
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

    template <typename VertexDesc, typename PropDesc>
    struct incidence_store
    {
        typedef std::pair<VertexDesc, PropDesc> value;
        typedef Compare<VertexDesc> compare;
        typedef IncAlloc<value> alloc;
        typedef incidence_set<value, compare, alloc> type;
    };
};

/**
 * The most common edge set uses standard allocators and allows parameterization
 * of the comparison function. The comparison function must operate on vertex
 * descriptors.
 */
template <template <typename> class Compare = std::less>
struct edge_set : basic_edge_set<Compare, std::allocator, std::allocator> { };

#endif

