
#ifndef BOOST_GRAPHS_ADJLIST_EDGE_LIST_HPP
#define BOOST_GRAPHS_ADJLIST_EDGE_LIST_HPP

#include <list>

#include <boost/descriptors.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

// Forward declarations
template <typename, typename> class property_list;
template <typename, typename> class incidence_list;
template <typename, typename> class out_list;
template <typename, typename> class in_list;

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
    // A couple of dummy vectors used to build descriptors.
    typedef std::list<int, FirstAlloc<int>> first_dummy;
    typedef std::list<int, SecondAlloc<int>> second_dummy;

    // Descriptor types for undirected graphs.
    typedef typename descriptor_traits<first_dummy>::descriptor_type incidence_descriptor;
    typedef typename descriptor_traits<second_dummy>::descriptor_type label_descriptor;

    // The property store metafunction generates the underlying global store
    // type for edge label in undirected graphs.
    template <typename VertexDesc, typename EdgeProps>
    struct property_store
    {
        typedef std::pair<VertexDesc, incidence_descriptor> end;
        typedef std::pair<EdgeProps, std::pair<end, end>> edge;
        typedef SecondAlloc<edge> allocator;
        typedef property_list<edge, allocator> type;
    };

    // The incidence store metafunction generates the per-vertex storage type
    // for undirected vertices.
    template <typename VertexDesc>
    struct incidence_store
    {
        typedef std::pair<VertexDesc, label_descriptor> incidence_pair;
        typedef FirstAlloc<incidence_pair> allocator;
        typedef incidence_list<incidence_pair, allocator > type;
    };



    // Descriptor types for directed graphs
    typedef typename descriptor_traits<first_dummy>::descriptor_type out_descriptor;
    typedef typename descriptor_traits<second_dummy>::descriptor_type in_descriptor;

    // The out store metafunction generates the type of list used to store
    // out edges of a vertex in a directed graph.
    template <typename VertexDesc, typename EdgeProps>
    struct out_store
    {
        typedef std::pair<VertexDesc, std::pair<EdgeProps, in_descriptor>> edge;
        typedef FirstAlloc<edge> allocator;
        typedef out_list<edge, allocator> type;
    };

    // The in store metafunction generates the type of list used to store
    // incoming edges of directed graph. In edges are represented by the
    // referencing vertex and an out edge descriptor.
    template <typename VertexDesc>
    struct in_store
    {
        typedef std::pair<VertexDesc, out_descriptor> edge;
        typedef SecondAlloc<edge> allocator;
        typedef in_list<edge, allocator> type;
    };
};

} } } /* namespace boost::graphs::adjacency_list */

#endif
