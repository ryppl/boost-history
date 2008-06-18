
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

    typedef typename out_store::const_iterator out_iterator;
    typedef typename out_store::size_type out_size_type;
    
    typedef typename in_store::const_iterator in_iterator;
    typedef typename in_store::size_type in_size_type;

    inline directed_vertex();
    inline directed_vertex(vertex_properties const& vp);

    inline vertex_properties& properties();
    inline vertex_properties const& properties() const;

    /** @name Out Edges */
    //@{
    inline out_iterator begin_out() const
    { return _out.begin(); }

    inline out_iterator end_out() const
    { return _out.end(); }

    inline out_size_type out_degree() const
    { return _out.size(); }
    //@}
    
    /** @name In Edges */
    //@{
    inline in_iterator begin_in() const
    { return _in.begin(); }
    
    inline in_iterator end_in() const
    { return _in.end(); }
    
    inline in_size_type in_degree() const
    { return _in.size(); }
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
