
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_DIRECTED_VERTEX_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_DIRECTED_VERTEX_HPP

#include <boost/graphs/adjacency_list/vertex.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * A directed vertex provides access to two separate sets of incident
 * edges - both in-edges and out-edges.
 *
 * This varies from the original BGL, which provided directed (which
 * was only the out edges) and bidirectional (both in and out edges).
 * The cost of providing both as a directed graph doesn't particularly
 * phase me too much. I'm basically saying that I haven't seen too
 * many useful applications of half-directed graphs.
 */
template <
        typename VertexProps,
        typename EdgeProps,
        typename VertexDesc,
        typename EdgeDesc,
        template <typename> class VertexEdgeStore
    >
class directed_vertex
    : public vertex<VertexProps>
{
public:
    typedef VertexDesc descriptor_type;
    typedef EdgeDesc edge_descriptor;
    typedef typename vertex<VertexProps>::properties_type properties_type;
private:
    typedef VertexEdgeStore<edge_descriptor> vertex_edge_store;
public:
    // Edge iterator types
    typedef typename vertex_edge_store::const_iterator incidence_iterator;
    typedef std::pair<incidence_iterator, incidence_iterator> incidence_range;
    typedef typename vertex_edge_store::size_type degree_type;

    directed_vertex();
    directed_vertex(VertexProps const& vp);

    // Connection interface.
    void connect_to(edge_descriptor e);
    void connect_from(edge_descriptor e);

    // Disconnection interface.
    void disconnect_to(edge_descriptor e);
    void disconnect_from(edge_descriptor e);
    void disconnect_all();

    // Edge iterators
    incidence_range out_edges() const;
    incidence_iterator begin_out_edges() const;
    incidence_iterator end_out_edges() const;

    incidence_range in_edges() const;
    incidence_iterator begin_in_edges() const;
    incidence_iterator end_in_edges() const;

    degree_type out_degree() const;
    degree_type in_degree() const;

private:
    vertex_edge_store _out;
    vertex_edge_store _in;
};

// Functions

#define BOOST_GRAPH_DV_PARAMS \
    typename VP, typename EP, typename V, typename E, template <typename> class VES

template <BOOST_GRAPH_DV_PARAMS>
directed_vertex<VP,EP,V,E,VES>::directed_vertex()
    : vertex<VP>()
    , _out()
    , _in()
{ }

template <BOOST_GRAPH_DV_PARAMS>
directed_vertex<VP,EP,V,E,VES>::directed_vertex(VP const& vp)
        : vertex<VP>(vp)
        , _out()
        , _in()
    { }

/**
 * Connect this vertex to the vertex at the opposite end of this edge. Note
 * that this vertex is neither truly the source nor target of the edge. This
 * does not guarantee that source(e) == this.
 */
template <BOOST_GRAPH_DV_PARAMS>
void
directed_vertex<VP,EP,V,E,VES>::connect_to(edge_descriptor e)
{
    _out.add(e);
}

/**
 * Connect this vertex to the vertex at the opposite end of the edge. Note that
 * this does not guarantee that the target(e) == this.
 */
template <BOOST_GRAPH_DV_PARAMS>
void
directed_vertex<VP,EP,V,E,VES>::connect_from(edge_descriptor e)
{ 
    _in.add(e);
}

/**
 * Locally remove the given edge that connects this vertex to another endpoint.
 */
template <BOOST_GRAPH_DV_PARAMS>
void
directed_vertex<VP,EP,V,E,VES>::disconnect_to(edge_descriptor e)
{
    _out.remove(e);
}

/**
 * Locally remove the given edge that connects this vertex to another endpoint.
 */
template <BOOST_GRAPH_DV_PARAMS>
void
directed_vertex<VP,EP,V,E,VES>::disconnect_from(edge_descriptor e)
{ 
    _in.remove(e);
}

/**
 * Locally disconnect of all the incident edges from this vertex. Note that
 * this is really only used by the disconnect algorithm.
 */
template <BOOST_GRAPH_DV_PARAMS>
void
directed_vertex<VP,EP,V,E,VES>::disconnect_all()
{
    _out.clear();
    _in.clear();
}

/**
 * Get an iterator range over the out edges of this vertex.
 */
template <BOOST_GRAPH_DV_PARAMS>
typename directed_vertex<VP,EP,V,E,VES>::incidence_range
directed_vertex<VP,EP,V,E,VES>::out_edges() const
{
    return std::make_pair(_out.begin(), _out.end());
}

/**
 * Get an iterator to the first in edge.
 */
template <BOOST_GRAPH_DV_PARAMS>
typename directed_vertex<VP,EP,V,E,VES>::incidence_iterator
directed_vertex<VP,EP,V,E,VES>::begin_out_edges() const
{
    return _out.begin();
}

/**
 * Get an iterator pas the end of the out edges.
 */
template <BOOST_GRAPH_DV_PARAMS>
typename directed_vertex<VP,EP,V,E,VES>::incidence_iterator
directed_vertex<VP,EP,V,E,VES>::end_out_edges() const
{
    return _out.end();
}

/**
 * Get an iterator range to the in-edges.
 */
template <BOOST_GRAPH_DV_PARAMS>
typename directed_vertex<VP,EP,V,E,VES>::incidence_range
directed_vertex<VP,EP,V,E,VES>::in_edges() const
{
    return std::make_pair(_in.begin(), _in.end());
}

/**
 * Get an iterator to the first in-edge.
 */
template <BOOST_GRAPH_DV_PARAMS>
typename directed_vertex<VP,EP,V,E,VES>::incidence_iterator
directed_vertex<VP,EP,V,E,VES>::begin_in_edges() const
{
    return _in.begin();
}

/**
 * Get an iterator past the end of in edges.
 */
template <BOOST_GRAPH_DV_PARAMS>
typename directed_vertex<VP,EP,V,E,VES>::incidence_iterator
directed_vertex<VP,EP,V,E,VES>::end_in_edges() const
{
    return _in.end();
}

/**
 * Return the out-degree of this vertex.
 */
template <BOOST_GRAPH_DV_PARAMS>
typename directed_vertex<VP,EP,V,E,VES>::degree_type
directed_vertex<VP,EP,V,E,VES>::out_degree() const
{
    return _out.size();
}

/**
 * Return the out-degree of this vertex.
 */
template <BOOST_GRAPH_DV_PARAMS>
typename directed_vertex<VP,EP,V,E,VES>::degree_type
directed_vertex<VP,EP,V,E,VES>::in_degree() const
{
    return _in.size();
}

/**
 * Disconnect the given vertex from the given graph. This is not part of the
 * public interface to this function.
 *
 * @todo This is actually kind of a generic algorithm.
 */
template <typename Graph>
void
disconnect(Graph& g, typename Graph::vertex_type& u, directed_tag)
{
    // Undirected - iterate over each of the incident edges I and get the
    // opposite vertex w. Remove all edges from the adjacent vertex that
    // connect to v. Remove all edges I from E.

    typedef typename Graph::vertex_type vertex;
    typedef typename vertex::descriptor_type vertex_descriptor;

    typedef typename Graph::edge_type edge;
    typedef typename edge::descriptor_type edge_descriptor;
    // typedef typename Graph::edge_store edge_store;

    typedef typename vertex::incidence_iterator incidence_iterator;

    // Get a descriptor for this vertex.
    vertex_descriptor ud = g.descriptor(u);

    incidence_iterator
            i = u.begin_incident_out(),
            end = u.end_incident_out();
    for( ; i != end; ++i) {
        // Get the vertex at the opposite end of the current edge.
        edge_descriptor ed = *i;
        edge& e = g.edge(ed);
        vertex& v = g.vertex(e.opposite(ud));

        // Remove the edge containing this vertex from v.
        v.disconnect_from(ed);

        // Remove this edge from the graph's edge set. Note that this is
        // basically just discards the edge from the edge set without actually
        // trying to disconnect it from the edges.
        g.template Graph::edge_store::remove_edge(ed);
    }
    u.disconnect_all();
}

#undef BOOST_GRAPH_DV_PARAMS

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
