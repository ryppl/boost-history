
#ifndef BOOST_GRAPHS_ADJLIST_UNDIRECTED_GRAPH_HPP
#define BOOST_GRAPHS_ADJLIST_UNDIRECTED_GRAPH_HPP

#include <boost/assert.hpp>
#include <boost/none.hpp>

#include <boost/graphs/adjacency_list/vertex_store.hpp>
#include <boost/graphs/adjacency_list/edge_store.hpp>
#include <boost/graphs/adjacency_list/incidence_store.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

/**
 * The undirected graph class...
 */
template <
    typename VertexLabel,
    typename EdgeLabel,
    typename VertexStore,
    typename EdgeStore,
    typename IncidenceStore>
class undirected_graph
{
public:
    // Describe labels
    typedef VertexLabel vertex_label;
    typedef EdgeLabel edge_label;

    // Generate descriptors
    typedef typename VertexStore::vertex_descriptor vertex_descriptor;
    typedef typename EdgeStore::edge_descriptor edge_descriptor;

    // Generate incidence store
    typedef typename IncidenceStore::template store<edge_descriptor>::type incidence_store;
    typedef typename VertexStore::template store<incidence_store, VertexLabel>::type vertex_store;
    typedef typename EdgeStore::template store<vertex_descriptor, EdgeLabel>::type edge_store;

    // Misc types
    typedef typename VertexStore::key_type vertex_key;
    typedef typename vertex_store::size_type vertices_size_type;

public:
    vertex_store v;
    edge_store e;
};

template <typename VL, typename EL, typename VS, typename ES, typename IS>
typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor
add_vertex(undirected_graph<VL,EL,VS,ES,IS>& g)
{
    typedef typename undirected_graph<VL,EL,VS,ES,IS>::vertex_label VertexLabel;
    return vs::insert(g.v, VertexLabel());
}

template <typename VL, typename EL, typename VS, typename ES, typename IS>
typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor
add_vertex(undirected_graph<VL,EL,VS,ES,IS>& g,
           typename undirected_graph<VL,EL,VS,ES,IS>::vertex_label&& l)
{ return vs::insert(g.v, l); }

template <typename VL, typename EL, typename VS, typename ES, typename IS>
typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor
add_vertex(undirected_graph<VL,EL,VS,ES,IS>& g,
           typename undirected_graph<VL,EL,VS,ES,IS>::vertex_key const& k,
           typename undirected_graph<VL,EL,VS,ES,IS>::vertex_label&& l)
{ return vs::insert(g.v, k, l); }




template <typename VL, typename EL, typename VS, typename ES, typename IS>
typename undirected_graph<VL,EL,VS,ES,IS>::vertices_size_type
num_vertices(undirected_graph<VL,EL,VS,ES,IS> const& g)
{ return vs::size(g.v); }


} } } /* namespace boost::graphs::adjacency_list */

#endif

