
#ifndef DIRECTED_VERTEX_HPP
#define DIRECTED_VERTEX_HPP

#include "directed_edge.hpp"

/**
 * A directed vertex provides storage for both outgoing edges and in edges.
 * Each of these stores are parameterizable, although they should generally
 * share the same underlying data structure (e.g., vector, list, or set).
 */
template <typename VertexProps, typename OutStore, typename InStore>
class directed_vertex
{
    typedef OutStore out_store;
    typedef InStore in_store;
public:
    typedef VertexProps vertex_properties;
    typedef typename out_store::vertex_descriptor vertex_descriptor;

    typedef typename OutStore::edge_properties edge_properties;

    typedef typename out_store::iterator out_iterator;
    typedef typename out_store::size_type out_size_type;

    typedef typename in_store::iterator in_iterator;
    typedef typename in_store::size_type in_size_type;

    typedef out_size_type incident_size_type;

    typedef directed_edge<out_iterator> edge_descriptor;

    /** @name Constructors */
    //@{
    inline directed_vertex()
        : _props()
        , _out()
        , _in()
    { }

    inline directed_vertex(vertex_properties const& vp)
        : _props(vp)
        , _out()
        , _in()
    { }
    //@}

    std::pair<out_iterator, bool> allow(vertex_descriptor) const;

    out_iterator connect_target(vertex_descriptor, edge_properties const&);
    in_iterator connect_source(vertex_descriptor);
    static edge_descriptor bind_connection(out_iterator, in_iterator);

    out_size_type disconnect(directed_vertex&, vertex_descriptor);
    void disconnect_target(edge_descriptor);
    void disconnect_source(edge_descriptor);

    /** @name Property Accessors */
    //@{
    inline vertex_properties& properties()
    { return _props; }

    inline vertex_properties const& properties() const
    { return _props; }
    //@}


    /** @name Out Edges */
    //@{
    inline out_iterator begin_out() const
    { return _out.begin(); }

    inline out_iterator end_out() const
    { return _out.end(); }

    inline out_iterator find_out(vertex_descriptor v) const
    { return _out.find(v); }

    inline void clear_out()
    { _out.clear(); }
    //@}

    /** @name In Edges */
    //@{
    inline in_iterator begin_in() const
    { return _in.begin(); }

    inline in_iterator end_in() const
    { return _in.end(); }

    inline in_iterator find_in(vertex_descriptor v) const
    { return _in.find(v); }

    inline void clear_in()
    { return _in.clear(); }
    //@}


    /** @name Degree */
    //@{
    inline out_size_type out_degree() const
    { return _out.size(); }

    inline in_size_type in_degree() const
    { return _in.size(); }

    inline incident_size_type degree() const
    { return out_degree() + in_degree(); }
    //@}


    /** @name Operators */
    //@{
    inline bool operator==(directed_vertex const& x) const
    { return _props == x._props; }

    inline bool operator!=(directed_vertex const& x) const
    { return !this->operator==(x); }

    inline bool operator<(directed_vertex const& x) const
    { return _props < x._props; }
    //@}

private:
    vertex_properties   _props;
    out_store           _out;
    in_store            _in;
};

/**
 * Determine if the connection from this vertex to the other should be allwed.
 * This depends in part upon the structure of the out out edge store and the
 * policies used to configure the graph. This function returns a pair containing
 * an iterator to an existing edge and a bool, determining if the edge should
 * be added anyways.
 */
template <typename V, typename O, typename I>
std::pair<typename directed_vertex<V,O,I>::out_iterator, bool>
directed_vertex<V,O,I>::allow(vertex_descriptor v) const
{
    return _out.allow(v);
}

/**
 * Connect this vertex to the vertex v with the given properties. Return an out
 * edge descriptor for the new edge.
 */
template <typename V, typename O, typename I>
typename directed_vertex<V,O,I>::out_iterator
directed_vertex<V,O,I>::connect_target(vertex_descriptor v, edge_properties const& ep)
{
    return _out.add(v, ep);
}

/**
 * Add a incoming edge from the vertex v with out edge descriptor o.
 *
 * Unfortunately, we can't combine this operation with connet_to() because we
 * don't actually have a descriptor for this vertex.
 */
template <typename V, typename O, typename I>
typename directed_vertex<V,O,I>::in_iterator
directed_vertex<V,O,I>::connect_source(vertex_descriptor v)
{
    return _in.add(v);
}

/**
 * Having added the edge stubs, bind them together and return a descriptor over
 * the edge. This is a static method because it doesn't pertain to single
 * object.
 */
template <typename V, typename O, typename I>
typename directed_vertex<V,O,I>::edge_descriptor
directed_vertex<V,O,I>::bind_connection(out_iterator i, in_iterator j)
{
    i->template get<2>().put(j);
    j->second.put(i);
    return edge_descriptor(j->first, i);
}

/**
 * Disconnect all edges that connect this vertex to the target, returning the
 * number of edges actually removed.
 */
template <typename V, typename O, typename I>
typename directed_vertex<V,O,I>::out_size_type
directed_vertex<V,O,I>::disconnect(directed_vertex& vert, vertex_descriptor d)
{
    out_size_type ret = 0;
    out_iterator i = _out.begin(), end = _out.end();
    for( ; i != end; ) {
        vertex_descriptor t = i->template get<0>();

        // If the target vertex is one that we want to remove, remove it from
        // the out list of this vertex. Also, brute-force remove it from the
        // in list of other vertex.
        if(t == d) {
            vert._in.remove(i->template get<2>().template get<in_iterator>());
            i = _out.remove(i);
            ++ret;
       }
       else {
            ++i;
       }
    }
    return ret;
}

/**
 * Disconnect this vertex from its target, removing the outgoing edge.
 */
template <typename V, typename O, typename I>
void
directed_vertex<V,O,I>::disconnect_target(edge_descriptor e)
{
    _out.remove(e.out_edge());
}

/**
 * Disconnect this vertex from its source, removing the incoming edge.
 */
template <typename V, typename O, typename I>
void
directed_vertex<V,O,I>::disconnect_source(edge_descriptor e)
{
    // Get the input iterator from the edge.
    out_iterator o = e.out_edge();
    in_iterator i = o->template get<2>().template get<in_iterator>();
    _in.remove(i);
}

#endif
