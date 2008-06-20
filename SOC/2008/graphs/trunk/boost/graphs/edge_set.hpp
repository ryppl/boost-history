
#ifndef EDGE_SET_HPP
#define EDGE_SET_HPP

#include "property_list.hpp"
#include "incidence_set.hpp"
#include "out_set.hpp"
#include "in_set.hpp"

/**
 * The edge set metafunction defines the basic facets of set-based edge
 * storage. Edge sets are best used to implement simple graphs (that do not
 * allow multiple edges).
 *
 * @param Compare - A unary template class that compares vertex descriptors.
 * @param FirstAlloc - An allocator template for either incident edges or out edges.
 * @param SecondAlloc - An allocator template for either global properties or in edges.
 */
template <
    template <typename> class Compare,
    template <typename> class FirstAlloc,
    template <typename> class SecondAlloc>
struct basic_edge_set
{
    // The property store metafunnction generates the global store type
    // for undirected graphs.
    template <typename EdgeProps>
    struct property_store
    {
        typedef SecondAlloc<EdgeProps> allocator;
        typedef property_list<EdgeProps, allocator> type;
    };

    // The incidence store metafunction generates the per-vertex stores for
    // incident edges.
    template <typename VertexDesc, typename PropDesc>
    struct incidence_store
    {
        typedef std::pair<VertexDesc, PropDesc> value;
        typedef Compare<VertexDesc> compare;
        typedef FirstAlloc<value> allocator;
        typedef incidence_set<value, compare, allocator> type;
    };

    // The out store metafunction generates the type of set used to store out
    // edges of a vertex in a directed graph.
    template <typename VertexDesc, typename Props>
    struct out_store
    {
        typedef std::pair<VertexDesc, Props> out_pair;
        typedef Compare<VertexDesc> compare;
        typedef FirstAlloc<out_pair> allocator;
        typedef out_set<out_pair, compare, allocator> type;
    };

    // The in store metafunction generates the type of list used to store
    // incoming edges of directed graph. In edges are represented by the
    // referencing vertex and an out edge descriptor.
    template <typename VertexDesc, typename OutDesc>
    struct in_store
    {
        typedef std::pair<VertexDesc, OutDesc> in_pair;
        typedef Compare<VertexDesc> compare;
        typedef SecondAlloc<in_pair> allocator;
        typedef in_set<in_pair, compare, allocator> type;
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

