
#ifndef DIRECTED_EDGE_HPP
#define DIRECTED_EDGE_HPP

#include <boost/tuple/tuple.hpp>

/**
 * A directed edge represents an edge in a directed graph. A directed edge is
 * uniquely identified by its source and target vertices and edge property.
 *
 * @param VertexDesc A vertex descriptor
 * @param OutDesc An out edge descriptor.
 */
template <typename OutTuple>
class directed_edge
{
public:
    typedef OutTuple out_tuple;
    typedef typename boost::tuples::element<0, OutTuple>::type vertex_descriptor;
    typedef typename boost::tuples::element<1, OutTuple>::type edge_properties;

    /** @name Constructors */
    //@{
    inline directed_edge()
        : _src()
        , _out(0)
    { }

    inline directed_edge(vertex_descriptor v, OutTuple const* o)
        : _src(v)
        , _out(o)
    { }
    //@}

    /** Return the source of the edge. */
    inline vertex_descriptor source() const
    { return _src; }

    /** Return the target of the edge. */
    inline vertex_descriptor target() const
    { return boost::get<0>(*_out); }

    /** @name Property Accessors
     * Return the properties associated with the edge.
     */
    //@{
    inline edge_properties& properties()
    { return boost::get<1>(*_out); }

    inline edge_properties const& properties() const
    { return boost::get<1>(*_out); }
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
    vertex_descriptor   _src;
    OutTuple const*     _out;
};


#endif
