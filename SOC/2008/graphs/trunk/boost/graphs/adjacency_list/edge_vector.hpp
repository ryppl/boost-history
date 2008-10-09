
#ifndef BOOST_GRAPHS_ADJLIST_EDGE_VECTOR_HPP
#define BOOST_GRAPHS_ADJLIST_EDGE_VECTOR_HPP

#include <boost/triple.hpp>
#include <boost/descriptors.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

// Forward declarations
template <typename, typename> class property_vector;
template <typename, typename> class incidence_vector;
template <typename, typename> class out_vector;
template <typename, typename> class in_vector;

// What's in an edge vector? Basically, an edge vector has to specify
// the type of property storage and the type of incidence storage. What
// are the possible parameters to edge storage?
// 1. The edge label
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
 * store and the SecondAlloc is the allocator for label. For directed
 * graphs, FirstAlloc and SecondAlloc are the per-vertex allocators for
 * out- and in-edge stores respectively.
 */
template <
    template <typename> class FirstAlloc = std::allocator,
    template <typename> class SecondAlloc = std::allocator>
struct edge_vector
{
    // A couple of dummy vectors used to build descriptors.
    typedef std::vector<int, FirstAlloc<int>> first_dummy;
    typedef std::vector<int, SecondAlloc<int>> second_dummy;

    // Descriptor types for undirected graphs.
    typedef typename descriptor_traits<first_dummy>::descriptor_type incidence_descriptor;
    typedef typename descriptor_traits<second_dummy>::descriptor_type label_descriptor;

    // The property store metafunction generates the type of vector used to
    // store global label for undirected graphs.
    template <typename VertexDesc, typename EdgeProps>
    struct property_store
    {
        typedef std::pair<VertexDesc, incidence_descriptor> end;
        typedef std::pair<EdgeProps, std::pair<end, end>> edge;
        typedef SecondAlloc<edge> allocator;
        typedef property_vector<edge, allocator> type;
    };

    // The incidence store metafunction generates the type of vector used to
    // store edges incident to the an undirected vertex.
    template <typename VertexDesc>
    struct incidence_store
    {
        typedef std::pair<VertexDesc, label_descriptor> incidence_pair;
        typedef FirstAlloc<incidence_pair> incidence_allocator;
        typedef incidence_vector<incidence_pair, incidence_allocator> type;
    };

    // Descriptor types for directed graphs.
    typedef typename descriptor_traits<first_dummy>::descriptor_type out_descriptor;
    typedef typename descriptor_traits<second_dummy>::descriptor_type in_descriptor;

    // The out store metafunction generates the type of vector used to store
    // out edges of a vertex in a directed graph.
    template <typename VertexDesc, typename EdgeProps>
    struct out_store
    {
        typedef std::pair<VertexDesc, std::pair<EdgeProps, in_descriptor>> edge;
        typedef FirstAlloc<edge> allocator;
        typedef out_vector<edge, allocator> type;
    };

    // The in store metafunction generates the type of vector used to store
    // incoming edges of directed graph. In edges are represented by the
    // referencing vertex and an out edge descriptor.
    template <typename VertexDesc>
    struct in_store
    {
        typedef std::pair<VertexDesc, out_descriptor> edge;
        typedef SecondAlloc<edge> allocator;
        typedef in_vector<edge, allocator> type;
    };
};

} } } /* namespace boost::graphs::adjacency_list */

#endif
