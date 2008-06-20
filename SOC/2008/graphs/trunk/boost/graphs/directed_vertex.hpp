
#ifndef DIRECTED_VERTEX_HPP
#define DIRECTED_VERTEX_HPP

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
    typedef typename OutStore::out_descriptor out_descriptor;

    typedef typename out_store::const_iterator out_iterator;
    typedef typename out_store::size_type out_size_type;

    typedef typename in_store::const_iterator in_iterator;
    typedef typename in_store::size_type in_size_type;

    typedef out_size_type incident_size_type;


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

    out_descriptor connect_target(vertex_descriptor, edge_properties const&);
    void connect_source(vertex_descriptor, out_descriptor);

    void disconnect_target(vertex_descriptor);
    void disconnect_source(vertex_descriptor);

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
    //@}

    /** @name In Edges */
    //@{
    inline in_iterator begin_in() const
    { return _in.begin(); }

    inline in_iterator end_in() const
    { return _in.end(); }
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
typename directed_vertex<V,O,I>::out_descriptor
directed_vertex<V,O,I>::connect_target(vertex_descriptor v, edge_properties const& ep)
{
    return _out.add(std::make_pair(v, ep));
}

/**
 * Add a incoming edge from the vertex v with out edge descriptor o.
 *
 * Unfortunately, we can't combine this operation with connet_to() because we
 * don't actually have a descriptor for this vertex.
 */
template <typename V, typename O, typename I>
void
directed_vertex<V,O,I>::connect_source(vertex_descriptor v, out_descriptor o)
{
    return _in.add(std::make_pair(v, o));
}

/**
 * Disconnect this vertex from its target, removing the outgoing edge.
 */
template <typename V, typename O, typename I>
void
directed_vertex<V,O,I>::disconnect_target(vertex_descriptor v)
{
    _out.remove(v);
}

/**
 * Disconnect this vertex from its source, removing the incoming edge.
 */
template <typename V, typename O, typename I>
void
directed_vertex<V,O,I>::disconnect_source(vertex_descriptor v)
{
    _in.remove(v);
}

#endif
