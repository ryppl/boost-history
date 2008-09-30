
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
    typedef VertexProps vertex_label;

    typedef typename out_store::vertex_descriptor vertex_descriptor;
    typedef typename out_store::edge_label edge_label;
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

    inline directed_vertex(vertex_label const& vp)
        : _props(vp), _out(), _in()
    { }
    //@}

    /** @name Edge Connection and Disconnection
     * These functions provide an interface to the in and out stores of the
     * vertex. The connection of this vertex to a target is a two-phase
     * operation. First, the connection is made to the target and vice-versa
     * from the source. Then, the target is bound with the in edge descriptor
     * of the source.
     */
    //@{
    insertion_result<out_descriptor> connect_target(vertex_descriptor v, edge_label const& ep)
    { return _out.add(v, ep); }

    insertion_result<in_descriptor> connect_source(vertex_descriptor v, out_descriptor o)
    { return _in.add(v, o); }

    void bind_target(out_descriptor o, in_descriptor i)
    { _out.bind(o, i); }

    void disconnect_target(out_descriptor o)
    { _out.remove(o); }

    void disconnect_source(in_descriptor i)
    { _in.remove(i); }

    /** Find the (at least one?) out edge connected to the given vertex. */
    out_descriptor find_target(vertex_descriptor v)
    { return _out.find(v); }
    //@}

    /** @name Vertex Property Accessors */
    //@{
    inline vertex_label& label()
    { return _props; }

    inline vertex_label const& label() const
    { return _props; }

    // Return the edge label for the given out edge descriptor.
    inline edge_label& get_edge_label(out_descriptor o)
    { return _out.label(o); }

    inline edge_label const& get_edge_label(out_descriptor o) const
    { return _out.label(o); }
    //@}

    /** Return the reverse out descriptor for the given in edge. */
    inline out_descriptor out_edge(in_descriptor i) const
    { return _in.out_edge(i); }

    /** Return the reverse in descriptor for the given out edge. */
    inline in_descriptor in_edge(out_descriptor o) const
    { return _out.in_edge(o); }


    /** Return a descriptor for the iterator into the underlying container. */
    inline out_descriptor out_edge(out_iterator o) const
    { return _out.out_edge(o) ;}

    /** Return a descriptor for the iterator into the underlying container */
    inline in_descriptor in_edge(in_iterator i) const
    { return _in.in_edge(i); }


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
    vertex_label   _props;
    out_store           _out;
    in_store            _in;
};


#endif
