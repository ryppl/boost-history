
#ifndef EDGE_LIST_HPP
#define EDGE_LIST_HPP

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
    template <typename> class FirstAlloc,
    template <typename> class SecondAlloc>
struct basic_edge_list
{
    // The property store metafunction generates the underlying global store
    // type for edge properties in undirected graphs.
    template <typename EdgeProps>
    struct property_store
    {
        typedef SecondAlloc<EdgeProps> allocator;
        typedef property_list<EdgeProps, allocator> type;
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
        typedef std::pair<VertexDesc, Props> out_pair;
        typedef FirstAlloc<out_pair> out_allocator;
        typedef out_list<out_pair, out_allocator> type;
    };

    // The in store metafunction generates the type of list used to store
    // incoming edges of directed graph. In edges are represented by the
    // referencing vertex and an out edge descriptor.
    template <typename VertexDesc, typename OutDesc>
    struct in_store
    {
        typedef std::pair<VertexDesc, OutDesc> in_pair;
        typedef SecondAlloc<in_pair> in_allocator;
        typedef in_list<in_pair, in_allocator> type;
    };
};

struct edge_list : basic_edge_list<std::allocator, std::allocator> { };

#endif

