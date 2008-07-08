
#ifndef EDGE_VECTOR_HPP
#define EDGE_VECTOR_HPP

#include "triple.hpp"
#include "placeholder.hpp"

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
    template <typename> class FirstAlloc = std::allocator,
    template <typename> class SecondAlloc = std::allocator>
struct edge_vector
{
    // The property store metafunction generates the type of vector used to
    // store global properties for undirected graphs.
    template <typename EdgeProps>
    struct property_store
    {
        // Define a dummy type that will eventually container iterators into
        // an incidence container. Use this as part of the triple for each
        // edge store - the properties and "out-facing" iterators.
        typedef typename hold<typename std::vector<int>::iterator>::type dummy_type;
        typedef triple<EdgeProps, dummy_type, dummy_type> property;
        typedef SecondAlloc<property> allocator;
        typedef property_vector<property, allocator> type;
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
        // Define a dummy type that eventually be used to store iterators to
        // in edge iterators. The actual out edge type is a tuple of target
        // vertex, edge properties, and in edge iterator (placeheld). The in
        // edge type is the source vertex and the out edge iterator (placeheld).
        typedef typename hold<typename std::vector<int>::iterator>::type dummy_type;
        typedef triple<VertexDesc, Props, dummy_type> edge;
        typedef FirstAlloc<edge> allocator;
        typedef out_vector<edge, allocator> type;
    };

    // The in store metafunction generates the type of vector used to store
    // incoming edges of directed graph. In edges are represented by the
    // referencing vertex and an out edge descriptor.
    template <typename VertexDesc>
    struct in_store
    {
        // Define a dummy type that will ultimately act as a placeholder for
        // an iterator into the out edge vector. Use that to define the in edge
        // pair.
        typedef typename hold<typename std::vector<int>::iterator>::type dummy_type;
        typedef std::pair<VertexDesc, dummy_type> edge;
        typedef SecondAlloc<edge> allocator;
        typedef in_vector<edge, allocator> type;
    };
};

#endif
