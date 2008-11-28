
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

    vertex_label& operator[](vertex_descriptor d)
    { return vs::label(v, d); }
    vertex_label const& operator[](vertex_descriptor d) const
    { return vs::label(v, d); }

    // TODO: Implement me
    edge_label& operator[](edge_descriptor d)
    { return edge_label(); }
    edge_label const& operator[](edge_descriptor d) const
    { return edge_label(); }

public:
    vertex_store v;
    edge_store e;
};

/** @name Add Vertex */
//@{
template <typename VL, typename EL, typename VS, typename ES, typename IS>
typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor
add_vertex(undirected_graph<VL,EL,VS,ES,IS>& g)
{ return vs::insert(g.v, typename undirected_graph<VL,EL,VS,ES,IS>::vertex_label()); }

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
//@}

/** @name Find Vertex
 * Return a descriptor to the first vertex found with the given label.
 *
 * There are two overloads of this function: one for finding vertices based on
 * their label, the other based on their key. The latter is only applicable
 * for graphs with mapped vertices.
 *
 * @note, These overloads are ambigous for mapped vertex graphs if the key
 * and label are the same type. We should probably provide special function to
 * differentiate these functions.
 */
//@{
template <typename VL, typename EL, typename VS, typename ES, typename IS>
typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor
find_vertex(undirected_graph<VL,EL,VS,ES,IS> const& g,
            typename undirected_graph<VL,EL,VS,ES,IS>::vertex_label const& l)
{ return vs::find(g.v, l); }

template <typename VL, typename EL, typename VS, typename ES, typename IS>
typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor
find_vertex(undirected_graph<VL,EL,VS,ES,IS> const& g,
            typename undirected_graph<VL,EL,VS,ES,IS>::vertex_key const& k)
{ return vs::find(g.v, k); }
//@}

/** @name Remove Vertex
 * Remove the given vertex from the graph.
 */
//@{
//@}

template <typename VL, typename EL, typename VS, typename ES, typename IS>
typename undirected_graph<VL,EL,VS,ES,IS>::vertices_size_type
num_vertices(undirected_graph<VL,EL,VS,ES,IS> const& g)
{ return vs::size(g.v); }


} } } /* namespace boost::graphs::adjacency_list */

#endif

