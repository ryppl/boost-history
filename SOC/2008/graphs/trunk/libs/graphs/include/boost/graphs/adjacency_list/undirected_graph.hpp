
#ifndef BOOST_GRAPHS_ADJLIST_UNDIRECTED_GRAPH_HPP
#define BOOST_GRAPHS_ADJLIST_UNDIRECTED_GRAPH_HPP

#include <tuple>

#include <boost/assert.hpp>
#include <boost/none.hpp>

#include <boost/graphs/adjacency_list/vertex_store.hpp>
#include <boost/graphs/adjacency_list/edge_store.hpp>
#include <boost/graphs/adjacency_list/incidence_store.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

// NOTE: Why do we have parameters for edge and incidence sets if they're
// essentially required to be the same thing? To allow as much genericity as
// possible.

// TODO: This is a giant TODO related to the question above. The edge set should
// never actually be a set or multiset. Just a list or vector because the
// multigraph property is determined by the incidence set.

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
    typedef typename IncidenceStore::template store<vertex_descriptor, edge_descriptor>::type incidence_store;
    typedef typename VertexStore::template store<incidence_store, VertexLabel>::type vertex_store;
    typedef typename EdgeStore::template store<vertex_descriptor, EdgeLabel>::type edge_store;

    // Misc types
    typedef typename VertexStore::key_type vertex_key;
    typedef typename vertex_store_traits<vertex_store>::vertices_size_type vertices_size_type;
    typedef typename edge_store_traits<edge_store>::edges_size_type edges_size_type;

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
{ return add_vertex(g, typename undirected_graph<VL,EL,VS,ES,IS>::vertex_label()); }

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
 * @note These overloads are ambigous for mapped vertex graphs if the key and
 * label are the same type. We should probably provide special function to
 * differentiate these functions.
 */
//@{
template <typename VL, typename EL, typename VS, typename ES, typename IS>
inline typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor
vertex(undirected_graph<VL,EL,VS,ES,IS> const& g,
       typename undirected_graph<VL,EL,VS,ES,IS>::vertex_label const& l)
{ return vs::find(g.v, l); }

template <typename VL, typename EL, typename VS, typename ES, typename IS>
inline typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor
vertex(undirected_graph<VL,EL,VS,ES,IS> const& g,
       typename undirected_graph<VL,EL,VS,ES,IS>::vertex_key const& k)
{ return vs::find(g.v, k); }
//@}

/** @name Remove Vertex
 * Remove the given vertex from the graph. This will also remove all incident
 * edges from the vertex before removing it from the graph.
 * @todo Implement remove_edges.
 */
//@{
template <typename VL, typename EL, typename VS, typename ES, typename IS>
inline void
remove_vertex(undirected_graph<VL,EL,VS,ES,IS>& g,
              typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor v)
{
    // remove_edges();
    return vs::erase(g.v, v);
}
//@}

/** Return the number of vertices in the graph. */
template <typename VL, typename EL, typename VS, typename ES, typename IS>
inline typename undirected_graph<VL,EL,VS,ES,IS>::vertices_size_type
num_vertices(undirected_graph<VL,EL,VS,ES,IS> const& g)
{ return vs::size(g.v); }

/** @name Remove Vertex
 * Remove the given vertex from the graph. This will also remove all incident
 * edges from the vertex before removing it from the graph.
 * @todo Implement remove_edges.
 */
//@{
template <typename VL, typename EL, typename VS, typename ES, typename IS>
inline typename undirected_graph<VL,EL,VS,ES,IS>::edge_descriptor
add_edge(undirected_graph<VL,EL,VS,ES,IS>& g,
         typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor u,
         typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor v)
{ return add_edge(g, u, v, typename undirected_graph<VL,EL,VS,ES,IS>::edge_label()); }

namespace detail {
    // Specialize the binding of edge descriptors into the vertex by recognizing
    // that only unique associative containers actually need to test the result
    // of the insertion before binding.
    template <typename VS, typename V, typename Ins, typename Tag>
    inline void bind_edges(VS& vs, V u, V v, Ins ins, Tag)
    {
        incs::insert(vs::edges(vs, u), v, ins.first);
        incs::insert(vs::edges(vs, v), u, ins.first);
    }

    template <typename VS, typename V, typename Ins, typename Tag>
    inline void bind_edges(VS& vs, V u, V v, Ins ins, unique_associative_container_tag)
    {
        if(ins.second) {
            incs::insert(vs::edges(vs, u), ins.first);
            incs::insert(vs::edges(vs, v), ins.first);
        }
    }
} /* namespace detail */

template <typename VL, typename EL, typename VS, typename ES, typename IS>
inline typename undirected_graph<VL,EL,VS,ES,IS>::edge_descriptor
add_edge(undirected_graph<VL,EL,VS,ES,IS>& g,
         typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor u,
         typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor v,
         typename undirected_graph<VL,EL,VS,ES,IS>::edge_label&& l)
{
    typedef typename undirected_graph<VL,EL,VS,ES,IS>::edge_descriptor Edge;

    // This is not a great implementation... (or correct, really).
    std::pair<Edge, bool> x = es::insert(g.e, u, v, l);
    detail::bind_edges(g.v, u, v, x, container_category(g.e));
    return x.first;

    // What we should do follows: The primary reason for this is that 
#if NO_COMPILE
    if(has_unique_edges(*this)) {    // obviously done at compile time
        Edge e = edge(u, v);
        if(e) return e;
        else {
            // See code above
        }
    }
    else {
        // See code above.
    }
#endif
}
//@}

namespace detail {
    // Return a pair of vertices ordered by their degree s.t., degree(first)
    // is less than degree(second).
    template <typename VS, typename V>
    inline std::pair<V, V> verts_by_size(VS const& vs, V u, V v)
    {
        return incs::size(vs::edges(vs, u)) < incs::size(vs::edges(vs, v)) ?
            std::make_pair(u, v) : std::make_pair(v, u);
    }
}

/** Return a descriptor to the edge connecting the given vertices. */
template <typename VL, typename EL, typename VS, typename ES, typename IS>
inline typename undirected_graph<VL,EL,VS,ES,IS>::edge_descriptor
edge(undirected_graph<VL,EL,VS,ES,IS> const& g,
     typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor u,
     typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor v)
{
    // Here, we have to search the incidence store of the smaller degree vertex
    // for an edge that references the pair...
    typedef typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor Vertex;
    Vertex s, t;
    std::tie(s, t) = detail::verts_by_size(g.v, u, v);
    return incs::find(vs::edges(g.v, s), t);
}


/** Return the number of edges in the graph. */
template <typename VL, typename EL, typename VS, typename ES, typename IS>
inline typename undirected_graph<VL,EL,VS,ES,IS>::edges_size_type
num_edges(undirected_graph<VL,EL,VS,ES,IS> const& g)
{ return es::size(g.e); }



} } } /* namespace boost::graphs::adjacency_list */

#endif

