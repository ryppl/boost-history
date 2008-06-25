
#ifndef DIRECTED_EDGE_HPP
#define DIRECTED_EDGE_HPP

#include <iosfwd>

/**
 * A directed edge represents an edge in a directed graph. A directed edge is
 * uniquely identified by its source and target vertices and edge property.
 *
 * @param VertexDesc A vertex descriptor
 * @param OutDesc An out edge descriptor.
 */
template <typename OutIter, typename InIter>
class directed_edge
{
    template <typename O, typename I> friend bool operator==(directed_edge<O,I> const&, directed_edge<O,I> const&);
    template <typename O, typename I> friend bool operator<(directed_edge<O,I> const&, directed_edge<O,I> const&);
public:
    typedef OutIter out_iterator;
    typedef InIter in_iterator;
    typedef typename OutIter::value_type out_tuple;
    typedef typename out_tuple::first_type vertex_descriptor;
    typedef typename out_tuple::second_type edge_properties;

    /** @name Constructors */
    //@{
    inline directed_edge()
        : _src()
        , _out(0)
    { }

    inline directed_edge(vertex_descriptor v, OutIter o)
        : _src(v)
        , _out(o)
    { }
    //@}

    /** Return the source of the edge. */
    inline vertex_descriptor source() const
    { return _src; }

    /** Return the target of the edge. */
    inline vertex_descriptor target() const
    { return _out->first; }

    /** Return the iterator to the out edge. */
    inline OutIter out_edge() const
    { return _out; }

    inline InIter in_edge() const
    { return _out->third.template get<InIter>(); }

    /** @name Property Accessors
     * Return the properties associated with the edge.
     */
    //@{
    inline edge_properties& properties()
    { return _out->second; }

    inline edge_properties const& properties() const
    { return _out->second; }
    //@}

private:
    vertex_descriptor   _src;
    OutIter             _out;
};

template <typename O, typename I>
inline bool
operator==(directed_edge<O,I> const& x, directed_edge<O,I> const& y)
{ return (x._src == y._src) && (x._out == y._out); }

template <typename O, typename I>
inline bool
operator!=(directed_edge<O,I> const& x, directed_edge<O,I> const& y)
{ return !(x == y); }

template <typename O, typename I>
inline bool
operator<(directed_edge<O,I> const& x, directed_edge<O,I> const& y)
{ return std::make_pair(x._src, x._out) < std::make_pair(y._src, y._out); }

template <typename O, typename I>
std::ostream& operator<<(std::ostream& os, const directed_edge<O,I>& e)
{ return os << "(" << e.source() << " " << e.target() << ")"; }

#endif
