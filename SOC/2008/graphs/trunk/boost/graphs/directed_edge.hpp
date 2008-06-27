#ifndef DIRECTED_EDGE_HPP
#define DIRECTED_EDGE_HPP

#include <iosfwd>
#include <iterator>

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

   /**
     * Return a value that uniquely describes the edge. This is basically going
     * to be the address of the property. Note that directed graphs can't use
     * indexed exterior edge properties because there's no method for maintaining
     * a global edge index.
     */
    inline edge_properties* get() const
    { return &properties(); }

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

/**
 * The directed edge iterator provides functionality for iterating over the
 * edges of a directed graph. This is essenitally done by iterating over the
 * out edges of each vertex in the order in which they were added to the graph.
 */
template <typename Graph>
struct directed_edge_iterator
{
    typedef typename Graph::vertex_iterator vertex_iterator;
    typedef typename Graph::out_edge_iterator edge_iterator;

    typedef std::forward_iterator_tag iterator_category;
    typedef std::size_t difference_type;

    typedef typename Graph::edge_descriptor value_type;
    typedef value_type reference;
    typedef value_type pointer;

    // Used to construct the end iterator.
    directed_edge_iterator(Graph const& g, vertex_iterator v)
        : graph(g), vert(v), edge()
    { }

    // Typically used to construct the begin iterator.
    directed_edge_iterator(Graph const& g, vertex_iterator v, edge_iterator e)
        : graph(g), vert(v), edge(e)
    { }

    inline directed_edge_iterator& operator++()
    {
        // If we're not done, increment the current edge. If that increment
        // pushes the edge iterator off the vertex, move to the next vertex
        // and reset the edge iterator.
        if(vert != graph.end_vertices()) {
            ++edge;
            if(edge == graph.end_out_edges(*vert)) {
                ++vert;
                if(vert != graph.end_vertices()) {
                    edge = graph.begin_out_edges(*vert);
                }
                else {
                    edge = edge_iterator();
                }
            }
        }
        return *this;
    }

    inline reference operator*() const
    { return  *edge; }

    Graph const& graph;
    vertex_iterator vert;
    edge_iterator edge;
};

template <typename Graph>
inline bool
operator==(directed_edge_iterator<Graph> const& a, directed_edge_iterator<Graph> const& b)
{ return (a.vert == b.vert) && (a.edge == b.edge); }

template <typename Graph>
inline bool
operator!=(directed_edge_iterator<Graph> const& a, directed_edge_iterator<Graph> const& b)
{ return !(a == b); }

#endif
