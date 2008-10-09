
#ifndef BOOST_GRAPHS_ADJLIST_EDGE_SET_HPP
#define BOOST_GRAPHS_ADJLIST_EDGE_SET_HPP

#include <list>
#include <map>

#include <boost/triple.hpp>
#include <boost/descriptors.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

// Forward declarations
template <typename, typename> class property_list;
template <typename, typename, typename> class incidence_set;
template <typename, typename, typename> class out_set;
template <typename, typename, typename> class in_set;

/**
 * The edge set metafunction defines the basic facets of set-based edge
 * storage. Edge sets are best used to implement simple graphs (that do not
 * allow multiple edges).
 *
 * @param Compare - A unary template class that compares vertex descriptors.
 * @param FirstAlloc - An allocator template for either incident edges or out edges.
 * @param SecondAlloc - An allocator template for either global label or in edges.
 */
template <
    template <typename> class Compare = std::less,
    template <typename> class FirstAlloc = std::allocator,
    template <typename> class SecondAlloc = std::allocator>
struct edge_set
{
    // Several dummy containers used to build descriptors. This looks really
    // weird since we're expecting a set type somewhere around here. However,
    // there isn't actually a real "set" used in these stores. The property
    // store only needs to be a list, and the incidence/in/out stores are
    // actually all maps (vertex to something).
    typedef std::map<int, int, Compare<int>, FirstAlloc<int>> first_dummy;
    typedef std::map<int, int, Compare<int>, SecondAlloc<int>> second_dummy;
    typedef std::list<int, SecondAlloc<int>> prop_dummy;

    // Descriptor types for undirected graphs.
    typedef typename descriptor_traits<first_dummy>::descriptor_type incidence_descriptor;
    typedef typename descriptor_traits<prop_dummy>::descriptor_type label_descriptor;

    // The property store metafunnction generates the global store type for
    // undirected graphs. The property list only needs to be a list, not a set.
    template <typename VertexDesc, typename EdgeProps>
    struct property_store
    {
        typedef std::pair<VertexDesc, incidence_descriptor> end;
        typedef std::pair<EdgeProps, std::pair<end, end>> edge;
        typedef SecondAlloc<edge> allocator;
        typedef property_list<edge, allocator> type;
    };

    // The incidence store metafunction generates the per-vertex stores for
    // incident edges.
    template <typename VertexDesc>
    struct incidence_store
    {
        typedef std::pair<VertexDesc, label_descriptor> value;
        typedef Compare<VertexDesc> compare;
        typedef FirstAlloc<value> allocator;
        typedef incidence_set<value, compare, allocator> type;
    };



    // Descriptor types for directed graphs
    typedef typename descriptor_traits<first_dummy>::descriptor_type out_descriptor;
    typedef typename descriptor_traits<second_dummy>::descriptor_type in_descriptor;

    // The out store metafunction generates the type of set used to store out
    // edges of a vertex in a directed graph.
    template <typename VertexDesc, typename EdgeProps>
    struct out_store
    {
        typedef std::pair<VertexDesc, std::pair<EdgeProps, in_descriptor>> edge;
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
        typedef std::pair<VertexDesc, out_descriptor> edge;
        typedef Compare<VertexDesc> compare;
        typedef SecondAlloc<edge> allocator;
        typedef in_set<edge, compare, allocator> type;
    };
};

} } }

#endif

