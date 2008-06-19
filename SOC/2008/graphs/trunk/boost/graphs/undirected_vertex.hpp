
#ifndef UNDIRECTED_VERTEX_HPP
#define UNDIRECTED_VERTEX_HPP

#include "incidence_iterator.hpp"

/**
 * A vertex, at least for an undirected graph, is simply an repository
 * for the vertex' properties and an interface for the its incidence
 * list.
 *
 * For directed graphs, it's basically the same, but has an out edge
 * list and/or an in edge list, and the edge properties are stored on
 * the out edge list.
 * The default comparison of vertices always delegates the comparison to the
 * stored vertex properties. This allows developers to express custom
 * comparitors with respect to the properties and have the vertex sets or other
 * vertex ordering operations work as they might expect.
 *
 * @requires LessThanComparable<vertex_properties>.
 */
template <typename VertexProps, typename IncStore>
class undirected_vertex
{
    typedef IncStore incidence_store;
public:
    typedef VertexProps vertex_properties;
    typedef typename incidence_store::vertex_descriptor vertex_descriptor;
    typedef typename incidence_store::property_descriptor property_descriptor;

    typedef typename incidence_store::const_iterator iterator;
    typedef typename incidence_store::size_type size_type;

    inline undirected_vertex();
    inline undirected_vertex(vertex_properties const& vp);

    /** @name Edge Connection and Disconnection
     * These functions control how edges are added to and removed from the
     * vertex. The allow() function determines whether or not the edge
     * connection is allowed and/or already existing.
     */
    inline std::pair<iterator, bool> allow(vertex_descriptor) const;
    inline void connect(vertex_descriptor, property_descriptor);
    inline void disconnect();
    inline void disconnect(vertex_descriptor, property_descriptor);
    template <typename Eraser> inline void disconnect(vertex_descriptor, Eraser);

    inline vertex_properties& properties();
    inline vertex_properties const& properties() const;


    inline iterator begin() const
    { return _edges.begin(); }

    inline iterator end() const
    { return _edges.end(); }

    inline size_type degree() const
    { return _edges.size(); }

    inline bool operator==(undirected_vertex const& x) const
    { return _props == x._props; }

    inline bool operator!=(undirected_vertex const& x) const
    { return !this->operator==(x); }

    inline bool operator<(undirected_vertex const& x) const
    { return _props < x._props; }

private:
    vertex_properties _props;
    incidence_store _edges;
};

template <typename VP, typename IS>
undirected_vertex<VP,IS>::undirected_vertex()
    : _props()
    , _edges()
{ }

template <typename VP, typename IS>
undirected_vertex<VP,IS>::undirected_vertex(vertex_properties const& vp)
    : _props(vp)
    , _edges()
{ }

/**
 * Deteremine whether or not the edge exists or is even allowed to be added.
 * This returns a pair containing an iterator indicating the position of the
 * edge if it already exists and a bool value indicating whether or not the
 * addition would even be allowed by policy.
 *
 * @complexity O(lg(d))
 */
template <typename VP, typename IS>
std::pair<typename undirected_vertex<VP,IS>::iterator, bool>
undirected_vertex<VP,IS>::allow(vertex_descriptor v) const
{
    return _edges.allow(v);
}

/**
 * Connect this vertex to the vertex v with edge properties p.
 */
template <typename VP, typename IS>
void
undirected_vertex<VP,IS>::connect(vertex_descriptor v, property_descriptor p)
{
    _edges.add(make_pair(v, p));
}

/**
 * Disconect all edges from this vertex. This does not remove the connection
 * from adjacent vertices to this vertex.
 */
template <typename VP, typename IS>
void
undirected_vertex<VP,IS>::disconnect()
{
    _edges.clear();
}

/**
 * Disconnect the incidedent edge given by the vertex v with edge properties p.
 */
template <typename VP, typename IS>
void
undirected_vertex<VP,IS>::disconnect(vertex_descriptor v, property_descriptor p)
{
    _edges.remove(make_pair(v, p));
}

template <typename VP, typename IS>
template <typename Eraser>
void
undirected_vertex<VP,IS>::disconnect(vertex_descriptor v, Eraser erase)
{
    _edges.remove(v, erase);
}

/**
 * Return the properties associated with this vertex (if any).
 */
template <typename VP, typename IS>
typename undirected_vertex<VP,IS>::vertex_properties&
undirected_vertex<VP,IS>::properties()
{
    return _props;
}

/**
 * Return the properties associated with this vertex (if any).
 */
template <typename VP, typename IS>
typename undirected_vertex<VP,IS>::vertex_properties const&
undirected_vertex<VP,IS>::properties() const
{
    return _props;
}

#endif
