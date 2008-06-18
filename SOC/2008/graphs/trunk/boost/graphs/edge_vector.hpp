
#ifndef EDGE_VECTOR_HPP
#define EDGE_VECTOR_HPP

#include "property_vector.hpp"
#include "incidence_vector.hpp"
#include "out_vector.hpp"
#include "in_vector.hpp"

// What's in an edge vector? Basically, an edge vector has to specify
// the type of property storage and the type of incidence storage. What
// are the possible parameters to edge storage?
// 1. The edge properties
// 2. Allocator for the property store
// 3. Allocator for the incidence store

// How does this generalize for directed graphs? Well... It would essentially
// rely on variadic templates - or rather variadic template templates. The
// goal would be to write something like: edge_vector<A, B, C> where A, B, and
// C are actually interpreted by the callers of the metafunctions - it ain't
// pretty, but it may actually work.

/**
 * The basic_edge_vector is the outer part of the metafunctions that generate
 * types for adjacency lists.
 *
 * This is not the prettiest solution, but it does reuse the same outer
 * metafunction for both directed and undirected graphs. The meaning of the
 * first and second allocator differ depending on the type of graph. For
 * undirected graphs, FirstAlloc is the allocator for the per-vertex incidence
 * store and the SecondAlloc is the allocator for properties. For directed
 * graphs, FirstAlloc and SecondAlloc are the per-vertex allocators for
 * out- and in-edge stores respectively.
 */
template <
    template <typename> class FirstAlloc,
    template <typename> class SecondAlloc>
struct basic_edge_vector
{
    // The property store metafunction generates the type of vector used to
    // store global properties for undirected graphs.
    template <typename EdgeProps>
    struct property_store
    {
        typedef SecondAlloc<EdgeProps> property_allocator;
        typedef property_vector<EdgeProps, property_allocator > type;
    };

    // The incidence store metafunction generates the type of vector used to
    // store edges incident to the an undirected vertex.
    template <typename VertexDesc, typename PropDesc>
    struct incidence_store
    {
        typedef std::pair<VertexDesc, PropDesc> incidence_pair;
        typedef FirstAlloc<incidence_pair> incidence_allocator;
        typedef incidence_vector<incidence_pair, incidence_allocator> type;
    };

    // The out store metafunction generates the type of vector used to store
    // out edges of a vertex in a directed graph.
    template <typename VertexDesc, typename Props>
    struct out_store
    {
        typedef std::pair<VertexDesc, Props> out_pair;
        typedef FirstAlloc<out_pair> out_allocator;
        typedef out_vector<out_pair, out_allocator> type;
    };

    // The in store metafunction generates the type of vector used to store
    // incoming edges (actually just the referencing vertex) of directed graph.
    // In edges are partially represented by the referencing vertex and a
    // pointer to the properties.
    template <typename VertexDesc, typename Props>
    struct in_store
    {
        typedef std::pair<VertexDesc, Props*> in_pair;
        typedef SecondAlloc<in_pair> in_allocator;
        typedef in_vector<in_pair, in_allocator> type;
    };
};

/**
 * The default edge vector is a basic edge vector that uses the standard
 * allocators for both the first and second allocators.
 */
struct edge_vector : basic_edge_vector<std::allocator, std::allocator> { };

#endif

