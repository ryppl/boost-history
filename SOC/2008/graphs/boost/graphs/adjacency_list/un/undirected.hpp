
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_UNDIRECTED_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_UNDIRECTED_HPP

#include <list>

#include <boost/graphs/utility/unordered_pair.hpp>
#include <boost/graphs/adjacency_list/vertex.hpp>
#include <boost/graphs/adjacency_list/edge.hpp>
#include <boost/graphs/adjacency_list/storage_traits.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

// Implementation of undirected adjacency lists.

// Forward declarations
template <typename VP, typename EP, typename V, typename E, template <typename> class VES> struct undirected_vertex;
template <typename VP, typename EP, typename V, typename E> struct undirected_edge;

// Unfortunately, I have to hack out a little tag dispatch here... This will
// go away with the concepts.
struct undirected_tag { };

/**
 * The undirected template is essentially responsible for generating the types
 * of verties and edges. Note that the definition of these types also depends,
 * to some degree, on the underlying storage mechanisms. Specifically, these
 * stores contribute information about the specific types of descriptors
 * employed by the adjacency list type.
 *
 * @todo Unfortunately, we are actually instantiating these types over dummy
 * stores in order to access their storage traits - basically deciding whether
 * the descriptor will be an integer or a void ptr. There doesn't really seem
 * to be any other way to do this since we're actually passing the stores as
 * template template parameters - which have no type. Perhaps the new language
 * will offer better solutions.
 */
template <
        typename VertexProps,
        typename EdgeProps,
        typename VertexStore,
        typename EdgeStore,
        template <typename> class VertexEdgeStore
    >
struct undirected
{
    typedef undirected_tag tag;

    typedef vertex_desc<
            typename storage_traits<VertexStore>::descriptor_type
        > vertex_descriptor;

    typedef edge_desc<
            typename storage_traits<EdgeStore>::descriptor_type
        > edge_descriptor;

    typedef undirected_vertex<
            VertexProps, EdgeProps, vertex_descriptor, edge_descriptor, VertexEdgeStore
        > vertex_type;

    typedef undirected_edge<
            VertexProps, EdgeProps, vertex_descriptor, edge_descriptor
        > edge_type;
};

/**
 * An undirected vertex tracks the incident edges of the given vertex. Note
 * that the edges actually being stored are pointers to edges in the graph's
 * edge list. The reason for this is so we don't duplicate the properties
 * of each edge.
 *
 * @todo What happens if I store incident edges as a pair... The edge and the
 * opposite end. That might be kind of interesting.
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
    typedef typename vertex_edge_store::const_iterator incident_edge_iterator;
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

    std::pair<incident_edge_iterator, incident_edge_iterator> incident_edges() const;
    incident_edge_iterator begin_incident_edges() const;
    incident_edge_iterator end_incident_edges() const;

    degree_type degree() const;

private:
    vertex_edge_store _edges;
};

/**
 * An undirected edge is an unordered pair of pointers to its endpoints. Note
 * that because the unordered pair is instantiated over pointers, these are
 * trivially ordered by their addresses. There is no other way available to
 * order the vertices (i.e., by custom comparison).
 *
 * Note that the edge does allow construction over null endpoints, but that
 * isn't really recommended since we don't offer any real mutators for the
 * endpoints. For constructors that do take vertex descriptors, we might also
 * point out that the ordering is not guaranteed after construction.
 */
template <
        typename VertexProps,
        typename EdgeProps,
        typename VertexDesc,
        typename EdgeDesc
    >
class undirected_edge
    : public edge<EdgeProps>
{
public:
    typedef EdgeDesc descriptor_type;
    typedef typename edge<EdgeProps>::properties_type properties_type;

    // This seems a little funky, but it turns out that some edge stores can
    // leverage vertex properties for add/insert functions.
    typedef VertexDesc vertex_descriptor;

    undirected_edge();
    undirected_edge(properties_type const& ep);
    undirected_edge(vertex_descriptor u, vertex_descriptor v);
    undirected_edge(vertex_descriptor u, vertex_descriptor v, properties_type const& ep);

    unordered_pair<vertex_descriptor> const& ends() const;

    vertex_descriptor source() const;
    vertex_descriptor target() const;
    vertex_descriptor opposite(vertex_descriptor v) const;

private:
    unordered_pair<vertex_descriptor> _ends;
};

// Vertex Functions

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
        typename undirected_vertex<VP,EP,V,E,VES>::incident_edge_iterator,
        typename undirected_vertex<VP,EP,V,E,VES>::incident_edge_iterator
    >
undirected_vertex<VP,EP,V,E,VES>::incident_edges() const
{
    return std::make_pair(_edges.begin(), _edges.end());
}

/**
 * Get an iterator to the first incident edge.
 */
template <BOOST_GRAPH_UV_PARAMS>
typename undirected_vertex<VP,EP,V,E,VES>::incident_edge_iterator
undirected_vertex<VP,EP,V,E,VES>::begin_incident_edges() const
{
    return _edges.begin();
}

/**
 * Get an iterator pas the end of the incident edges.
 */
template <BOOST_GRAPH_UV_PARAMS>
typename undirected_vertex<VP,EP,V,E,VES>::incident_edge_iterator
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

    typedef typename vertex::incident_edge_iterator incidence_iterator;

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

// Edge Functions

#define BOOST_GRAPH_UE_PARAMS \
    typename VP, typename EP, typename V, typename E

template <BOOST_GRAPH_UE_PARAMS>
undirected_edge<VP,EP,V,E>::undirected_edge()
    : edge<EP>()
    , _ends()
{ }

template <BOOST_GRAPH_UE_PARAMS>
undirected_edge<VP,EP,V,E>::undirected_edge(properties_type const& ep)
    : edge<EP>(ep)
    , _ends()
{ }

template <BOOST_GRAPH_UE_PARAMS>
undirected_edge<VP,EP,V,E>::undirected_edge(vertex_descriptor u, vertex_descriptor v)
    : edge<EP>()
    , _ends(u, v)
{ }

template <BOOST_GRAPH_UE_PARAMS>
undirected_edge<VP,EP,V,E>::undirected_edge(vertex_descriptor u,
                                            vertex_descriptor v,
                                            properties_type const& ep)
    : edge<EP>(ep)
    , _ends(u, v)
{ }

/**
 * Return the endpoints of the edge.
 */
template <BOOST_GRAPH_UE_PARAMS>
unordered_pair<typename undirected_edge<VP,EP,V,E>::vertex_descriptor> const&
undirected_edge<VP,EP,V,E>::ends() const
{ return _ends; }

/**
 * Return the source of this edge. The source of an undirected edge is not
 * necessarily the same as the way it is connected.
 */
template <BOOST_GRAPH_UE_PARAMS>
typename undirected_edge<VP,EP,V,E>::vertex_descriptor
undirected_edge<VP,EP,V,E>::source() const
{ return _ends.first(); }

/**
 * Return the target of this edge. The source of an undirected edge is not
 * necessarily the same as the way it is connected.
 */
template <BOOST_GRAPH_UE_PARAMS>
typename undirected_edge<VP,EP,V,E>::vertex_descriptor
undirected_edge<VP,EP,V,E>::target() const
{ return _ends.second(); }

/**
 * Return the vertex opposite the given vertex on this edge.
 */
template <BOOST_GRAPH_UE_PARAMS>
typename undirected_edge<VP,EP,V,E>::vertex_descriptor
undirected_edge<VP,EP,V,E>::opposite(vertex_descriptor u) const
{
    return u == _ends.first()  ? _ends.second() : _ends.first();
}

// Operators

// @todo I don't know that I like this. It might be better to build these
// operators as functors and then typedef them into the edge type rather than
// simply make these the default operators for the edges.

/**
 * Edges can be ordered by their end points. This is a lexicographical
 * comparison of the vertex descriptors in the ends of the edge.
 */
template <BOOST_GRAPH_UE_PARAMS>
bool
operator<(undirected_edge<VP,EP,V,E> const& a,
          undirected_edge<VP,EP,V,E> const& b)
{
    return a.ends() < b.ends();
}

/**
 * Edges are also equality comparable by their end points. Note that this does
 * /not/ compare the properties of vertices.
 */
template <BOOST_GRAPH_UE_PARAMS>
bool
operator==(undirected_edge<VP,EP,V,E> const& a,
           undirected_edge<VP,EP,V,E> const& b)
{
    return a.ends() == b.ends();
}

#undef BOOST_GRAPH_UE_PARAMS

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
