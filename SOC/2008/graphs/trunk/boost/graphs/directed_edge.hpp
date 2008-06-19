
#ifndef DIRECTED_EDGE_HPP
#define DIRECTED_EDGE_HPP

/**
 * A directed edge represents an edge in a directed graph. A directed edge is
 * uniquely identified by its source and target verties and edge property.
 *
 * @param VertexDesc A vertex descriptor
 * @param OutDesc An out edge descriptor.
 */
template <typename VertexDesc, typename OutDesc>
class directed_edge
{
public:
    typedef VertexDesc vertex_descriptor;
    typedef OutDesc out_descriptor;
    typedef typename out_descriptor::edge_properties edge_properties;

    /** @name Constructors */
    //@{
    inline directed_edge()
        : _src()
        , _out()
    { }

    inline directed_edge(vertex_descriptor v, OutDesc o)
        : _src(v)
        , _out(o)
    { }
    //@}

    /** Return the source of the edge. */
    inline vertex_descriptor source() const
    { return _src; }

    /** Return the target of the edge. */
    inline vertex_descriptor target() const
    { return _out.target(); }

    /** @name Property Accessors
     * Return the properties associated with the edge.
     */
    //@{
    inline edge_properties& properties()
    { return _out.properties(); }

    inline edge_properties const& properties() const
    { return _out.properties(); }
    //@}

    /** @name Operators */
    //@{
    inline bool operator==(directed_edge const& x)
    { return _src == x._src && _out = x._out; }

    inline bool operator!=(directed_edge const& x)
    { return !operator==(x); }

    inline bool operator<(directed_edge const& x)
    { return std::make_pair(_src, _out) < std::make_pair(x._src, x._out); }
    //@}

private:
    VertexDesc  _src;
    OutDesc     _out;
};


#endif
