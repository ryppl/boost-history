
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_UNDIRECTED_VERTEX_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_UNDIRECTED_VERTEX_HPP

#include <boost/graphs/adjacency_list/vertex.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

template <typename VertexProps, typename EdgeStore>
struct undirected_vertex
{
};

#if 0

/**
 */
template <
        typename VertexProps,
        typename EdgeProps,
        typename VertexDesc,
        typename EdgeDesc,
        template <typename> class VertexEdgeStore
    >
class undirected_vertex
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

    undirected_vertex();
    undirected_vertex(VertexProps const& vp);

    // Connection interface.
    void connect_to(edge_descriptor e);
    void connect_from(edge_descriptor e);

    // Disconnection interface.
    void disconnect_to(edge_descriptor e);
    void disconnect_from(edge_descriptor e);
    void disconnect_all();

    std::pair<incidence_iterator, incidence_iterator> incident_edges() const;
    incidence_iterator begin_incident_edges() const;
    incidence_iterator end_incident_edges() const;

    degree_type degree() const;

private:
    vertex_edge_store _edges;
};

// Functions

#define BOOST_GRAPH_UV_PARAMS \
    typename VP, typename EP, typename V, typename E, template <typename> class VES

template <BOOST_GRAPH_UV_PARAMS>
undirected_vertex<VP,EP,V,E,VES>::undirected_vertex()
    : vertex<VP>()
    , _edges()
{ }

template <BOOST_GRAPH_UV_PARAMS>
undirected_vertex<VP,EP,V,E,VES>::undirected_vertex(VP const& vp)
        : vertex<VP>(vp)
        , _edges()
    { }

/**
 * Connect this vertex to the vertex at the opposite end of this edge. Note
 * that this vertex is neither truly the source nor target of the edge. This
 * does not guarantee that source(e) == this.
 */
template <BOOST_GRAPH_UV_PARAMS>
void
undirected_vertex<VP,EP,V,E,VES>::connect_to(edge_descriptor e)
{
    _edges.add(e);
}

/**
 * Connect this vertex to the vertex at the opposite end of the edge. Note that
 * this does not guarantee that the target(e) == this.
 */
template <BOOST_GRAPH_UV_PARAMS>
void
undirected_vertex<VP,EP,V,E,VES>::connect_from(edge_descriptor e)
{
    _edges.add(e);
}

/**
 * Locally remove the given edge that connects this vertex to another endpoint.
 */
template <BOOST_GRAPH_UV_PARAMS>
void
undirected_vertex<VP,EP,V,E,VES>::disconnect_to(edge_descriptor e)
{
    _edges.remove(e);
}

/**
 * Locally remove the given edge that connects this vertex to another endpoint.
 */
template <BOOST_GRAPH_UV_PARAMS>
void
undirected_vertex<VP,EP,V,E,VES>::disconnect_from(edge_descriptor e)
{
    _edges.remove(e);
}

/**
 * Locally disconnect of all the incident edges from this vertex. Note that
 * this is really only used by the disconnect algorithm.
 */
template <BOOST_GRAPH_UV_PARAMS>
void
undirected_vertex<VP,EP,V,E,VES>::disconnect_all()
{
    _edges.clear();
}

/**
 * Get an iterator range over the incident edges of this vertex.
 */
template <BOOST_GRAPH_UV_PARAMS>
std::pair<
        typename undirected_vertex<VP,EP,V,E,VES>::incidence_iterator,
        typename undirected_vertex<VP,EP,V,E,VES>::incidence_iterator
    >
undirected_vertex<VP,EP,V,E,VES>::incident_edges() const
{
    return std::make_pair(_edges.begin(), _edges.end());
}

/**
 * Get an iterator to the first incident edge.
 */
template <BOOST_GRAPH_UV_PARAMS>
typename undirected_vertex<VP,EP,V,E,VES>::incidence_iterator
undirected_vertex<VP,EP,V,E,VES>::begin_incident_edges() const
{
    return _edges.begin();
}

/**
 * Get an iterator pas the end of the incident edges.
 */
template <BOOST_GRAPH_UV_PARAMS>
typename undirected_vertex<VP,EP,V,E,VES>::incidence_iterator
undirected_vertex<VP,EP,V,E,VES>::end_incident_edges() const
{
    return _edges.end();
}

/**
 * Return the degree of this vertex. The degree of the vertex is the number
 * of incident edges.
 */
template <BOOST_GRAPH_UV_PARAMS>
typename undirected_vertex<VP,EP,V,E,VES>::degree_type
undirected_vertex<VP,EP,V,E,VES>::degree() const
{
    return _edges.size();
}

/**
 * Disconnect the given vertex from the given graph. This is not part of the
 * public interface to this function.
 *
 * @todo This is actually kind of a generic algorithm.
 */
template <typename Graph>
void
disconnect(Graph& g, typename Graph::vertex_type& u, undirected_tag)
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
            i = u.begin_incident_edges(),
            end = u.end_incident_edges();
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

#undef BOOST_GRAPH_UV_PARAMS

#endif

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
