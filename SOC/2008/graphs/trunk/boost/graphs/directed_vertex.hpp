
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
    typedef typename out_store::edge_properties edge_properties;
    typedef typename out_store::out_descriptor out_descriptor;
    typedef typename out_store::iterator out_iterator;
    typedef typename out_store::size_type out_size_type;

    typedef typename in_store::in_descriptor in_descriptor;
    typedef typename in_store::iterator in_iterator;
    typedef typename in_store::size_type in_size_type;

    typedef out_size_type incident_size_type;

    /** @name Constructors */
    //@{
    inline directed_vertex()
        : _props(), _out(), _in()
    { }

    inline directed_vertex(vertex_properties const& vp)
        : _props(vp), _out(), _in()
    { }
    //@}

    /** @name Edge Connection and Disconnection
     * These functions provide an interface to the in and out stores of the
     * vertex.
     */
    //@{
    insertion_result<out_descriptor> connect_target(vertex_descriptor v, edge_properties const& ep)
    { return _out.add(v, ep); }

    insertion_result<in_descriptor> connect_source(vertex_descriptor v, out_descriptor o)
    { return _in.add(v, o); }

    void bind_target(out_descriptor o, in_descriptor i)
    { _out.bind(o, i); }

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


#endif
