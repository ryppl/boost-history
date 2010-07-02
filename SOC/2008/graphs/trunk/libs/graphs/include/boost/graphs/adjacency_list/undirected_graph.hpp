
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
    { return es::label(e, d); }
    edge_label const& operator[](edge_descriptor d) const
    { return es::label(e, d); }

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
    // Build the edge and bind the ends into the incidence stores of each
    // vertex.
    template <typename Graph, typename Vertex, typename Label>
    inline typename Graph::edge_descriptor
    do_add_edge(Graph& g, Vertex u, Vertex v, Label&& l)
    {
        std::pair<typename Graph::edge_descriptor, bool> x = es::insert(g.e, u, v, l);
        incs::insert(vs::edges(g.v, u), v, x.first);
        incs::insert(vs::edges(g.v, v), u, x.first);
        return x.first;
    }

    // For multigraphs, always add the edge.
    template <typename Graph, typename Vertex, typename Label, typename Tag>
    inline typename Graph::edge_descriptor
    dispatch_add_edge(Graph& g, Vertex u, Vertex v, Label&& l, Tag)
    { return do_add_edge(g, u, v, l); }

    // For unique-edged graphs, we should be checking if the edge exists before
    // actually adding it.
    template <typename Graph, typename Vertex, typename Label>
    inline typename Graph::edge_descriptor
    dispatch_add_edge(Graph& g, Vertex u, Vertex v, Label&& l, unique_associative_container_tag)
    {
        typename Graph::edge_descriptor e = edge(u, v);
        if(!e) {
            e = do_add_edge(u, v);
        }
        return e;
    }
} /* namespace detail */

template <typename VL, typename EL, typename VS, typename ES, typename IS>
inline typename undirected_graph<VL,EL,VS,ES,IS>::edge_descriptor
add_edge(undirected_graph<VL,EL,VS,ES,IS>& g,
         typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor u,
         typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor v,
         typename undirected_graph<VL,EL,VS,ES,IS>::edge_label&& l)
{
    typedef typename container_traits<
        typename undirected_graph<VL,EL,VS,ES,IS>::incidence_store
    >::category Category;
    return detail::dispatch_add_edge(g, u, v, l, Category());
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

/**
 * Remove the given edge from the graph, disconnecting the vertices at its
 * endpoints.
 *
 * @note The removal of edges is almost never a constant operation because we
 * have to search the incidence lists of each vertex to remove the corresponding
 * incidence record. The removal of the edge from the edge list is constant.
 */
template <typename VL, typename EL, typename VS, typename ES, typename IS>
inline void
remove_edge(undirected_graph<VL,EL,VS,ES,IS>& g,
            typename undirected_graph<VL,EL,VS,ES,IS>::edge_descriptor e)
{
    typedef typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor Vertex;
    Vertex u, v;
    std::tie(u, v) = es::ends(g.e, e);
    incs::erase(vs::edges(g.v, u), v, e);
    incs::erase(vs::edges(g.v, v), u, e);
    es::erase(g.e, e);
}

namespace detail {
    // This functor automatically erases edges from the edge set when visited
    // during incident edge erasure.
    template <typename EdgeList>
    struct edge_eraser
    {
        edge_eraser(EdgeList& el) : edges(el) { }

        template <typename Pair>
        void operator()(Pair const& x)
        { es::erase(edges, x.second); }

        EdgeList& edges;
    };

    template <typename EdgeList>
    edge_eraser<EdgeList> erase_edges(EdgeList& el)
    { return edge_eraser<EdgeList>(el); }
}

/** Remove all edges connecting the given vertices. */
template <typename VL, typename EL, typename VS, typename ES, typename IS>
inline void
remove_edges(undirected_graph<VL,EL,VS,ES,IS>& g,
             typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor u,
             typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor v)
{
    // Remove all edges u connected to v, while erasing the corresponding edge
    // from the edge set. Then the the same for v.
    incs::erase_all(vs::edges(g.v, u), v, detail::erase_edges(g.e));
    incs::erase_all(vs::edges(g.v, v), u);
}

namespace detail {
    // Reciprocating and cascading erasure for remove_edges(g,v).
    // TODO: This is not an efficient solution for multigraphs with sequential
    // incidence stores. For associative incidence stores, this is probably
    // fine.
    template <typename Graph, typename Vertex>
    struct edge_clearer
    {
        edge_clearer(Graph& g, Vertex v)
            : g(g), v(v)
        { }

        template <typename Pair>
        void operator()(Pair const& x)
        {
            // Don't erase self-loops. That would modify the sequence that
            // this functor is visiting - which could have some pretty nasty
            // side-effects.
            if(v != x.first) {
                incs::erase(vs::edges(g.v, x.first), v, x.second);
            }
            es::erase(g.e, x.second);
        }

        Graph& g;
        Vertex v;
    };

    template <typename Graph, typename Vertex>
    edge_clearer<Graph, Vertex> clear_edges(Graph& g, Vertex v)
    { return edge_clearer<Graph, Vertex>(g, v); }
}

/** Remove all edges incident to the given vertex. */
template <typename VL, typename EL, typename VS, typename ES, typename IS>
inline void
remove_edges(undirected_graph<VL,EL,VS,ES,IS>& g,
             typename undirected_graph<VL,EL,VS,ES,IS>::vertex_descriptor v)
{ incs::clear(vs::edges(g.v, v), detail::clear_edges(g, v)); }

/** Return the number of edges in the graph. */
template <typename VL, typename EL, typename VS, typename ES, typename IS>
inline typename undirected_graph<VL,EL,VS,ES,IS>::edges_size_type
num_edges(undirected_graph<VL,EL,VS,ES,IS> const& g)
{ return es::size(g.e); }



} } } /* namespace boost::graphs::adjacency_list */

#endif
