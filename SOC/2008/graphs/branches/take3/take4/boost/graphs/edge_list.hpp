
#ifndef EDGE_LIST_HPP
#define EDGE_LIST_HPP

#include "triple.hpp"
#include "placeholder.hpp"

#include "property_list.hpp"
#include "incidence_list.hpp"
#include "out_list.hpp"
#include "in_list.hpp"

// The edge list does two things. First, it indicates that edges will
// be stored in an incidence list (as opposed to a vector or set).
// Second, this will indicate the use of a global property list.
// Again, as opposed to a vector.

/**
 * The basic_edge_list is the outer part of the metafunctions that generate
 * types for adjacency lists.
 */
template <
    template <typename> class FirstAlloc = std::allocator,
    template <typename> class SecondAlloc = std::allocator>
struct edge_list
{
    // The property store metafunction generates the underlying global store
    // type for edge properties in undirected graphs.
    template <typename EdgeProps>
    struct property_store
    {
        typedef placeholder<sizeof(typename std::list<int>::iterator)> dummy_type;
        typedef triple<EdgeProps, dummy_type, dummy_type> property;
        typedef SecondAlloc<property> allocator;
        typedef property_list<property, allocator> type;
    };

    // The incidence store metafunction generates the per-vertex storage type
    // for undirected vertices.
    template <typename VertexDesc, typename PropDesc>
    struct incidence_store
    {
        typedef std::pair<VertexDesc, PropDesc> incidence_pair;
        typedef FirstAlloc<incidence_pair> allocator;
        typedef incidence_list<incidence_pair, allocator > type;
    };

    // The out store metafunction generates the type of list used to store
    // out edges of a vertex in a directed graph.
    template <typename VertexDesc, typename Props>
    struct out_store
    {
        // Define a dummy type that eventually be used to store iterators to
        // in edge iterators. The actual out edge type is a tuple of target
        // vertex, edge properties, and in edge iterator (placeheld). The in
        // edge type is the source vertex and the out edge iterator (placeheld).
        typedef placeholder<sizeof(typename std::list<int>::iterator)> dummy_type;
        typedef triple<VertexDesc, Props, dummy_type> edge;
        typedef FirstAlloc<edge> allocator;
        typedef out_list<edge, allocator> type;
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
        typedef placeholder<sizeof(typename std::list<int>::iterator)> dummy_type;
        typedef std::pair<VertexDesc, dummy_type> edge;
        typedef SecondAlloc<edge> allocator;
        typedef in_list<edge, allocator> type;
    };
};

#endif
