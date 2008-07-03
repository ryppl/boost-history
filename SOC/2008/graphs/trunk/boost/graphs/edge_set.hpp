
#ifndef EDGE_SET_HPP
#define EDGE_SET_HPP

#include "triple.hpp"
#include "placeholder.hpp"

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
    template <typename> class Compare = std::less,
    template <typename> class FirstAlloc = std::allocator,
    template <typename> class SecondAlloc = std::allocator>
struct edge_set
{
    // The property store metafunnction generates the global store type
    // for undirected graphs.
    template <typename EdgeProps>
    struct property_store
    {
        typedef typename hold<std::list<int>::iterator>::type dummy_type;
        typedef triple<EdgeProps, dummy_type, dummy_type> property;
        typedef SecondAlloc<property> allocator;
        typedef property_list<property, allocator> type;
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
        // Define a dummy type that eventually be used to store iterators to
        // in edge iterators. The actual out edge type is a tuple of target
        // vertex, edge properties, and in edge iterator (placeheld). The in
        // edge type is the source vertex and the out edge iterator (placeheld).
        typedef typename hold<std::set<int>::iterator>::type dummy_type;
        typedef triple<VertexDesc, Props, dummy_type> edge;
        typedef Compare<VertexDesc> compare;
        typedef FirstAlloc<edge> allocator;
        typedef out_set<edge, compare, allocator> type;
    };

    // The in store metafunction generates the type of list used to store
    // incoming edges of directed graph. In edges are represented by the
    // referencing vertex and an out edge descriptor.
    template <typename VertexDesc>
    struct in_store
    {
        // Define a dummy type that will ultimately act as a placeholder for
        // an iterator into the out edge vector. Use that to define the in edge
        // pair.
        typedef typename hold<typename std::set<int>::iterator>::type dummy_type;
        typedef std::pair<VertexDesc, dummy_type> edge;
        typedef Compare<VertexDesc> compare;
        typedef SecondAlloc<edge> allocator;
        typedef in_set<edge, compare, allocator> type;
    };
};

#endif

