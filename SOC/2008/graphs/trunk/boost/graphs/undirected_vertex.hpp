
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
public:
    typedef VertexProps vertex_properties;

    typedef IncStore incidence_store;
    typedef typename incidence_store::incidence_descriptor incidence_descriptor;
    typedef typename incidence_store::vertex_descriptor vertex_descriptor;
    typedef typename incidence_store::property_descriptor property_descriptor;
    typedef typename incidence_store::iterator iterator;
    typedef typename incidence_store::size_type size_type;

    inline undirected_vertex();
    inline undirected_vertex(vertex_properties const& vp);

    /** @name Edge Connection and Disconnection
     * These functions control how edges are added to and removed from the
     * vertex. The allow() function determines whether or not the edge
     * connection is allowed and/or already existing.
     */
    //@{
    inline insertion_result<incidence_descriptor> connect(vertex_descriptor v)
    { return _edges.add(v); }

    inline void bind(incidence_descriptor i, property_descriptor p)
    { _edges.bind(i, p); }

    inline void disconnect(vertex_descriptor, property_descriptor)
    { }

    inline void disconnect(incidence_descriptor)
    { }
    //@}

    /** Find return an iterator the edge end with the given vertex. */
    inline iterator find(vertex_descriptor v) const
    { return _edges.find(v); }

    /** Return the properties of the given incident edge. */
    inline property_descriptor edge_properties(incidence_descriptor i) const
    { return _edges.properties(i); }


    /** @name Property Accessors */
    //@{
    inline vertex_properties& properties()
    { return _props; }

    inline vertex_properties const& properties() const
    { return _props; }

    //@}

    /** @name Iterators */
    //@{
    inline iterator begin() const
    { return _edges.begin(); }

    inline iterator end() const
    { return _edges.end(); }
    //@}

    inline void clear()
    { _edges.clear(); }

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


#endif
