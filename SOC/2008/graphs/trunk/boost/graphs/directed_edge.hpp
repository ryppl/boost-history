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
template <typename VertexDesc, typename OutDesc>
class directed_edge
{
public:
    typedef VertexDesc vertex_descriptor;
    typedef OutDesc out_descriptor;
    typedef std::pair<vertex_descriptor, vertex_descriptor> edge_pair;

    /** @name Constructors */
    //@{
    inline directed_edge()
        : ends(), out()
    { }

    inline directed_edge(vertex_descriptor u, vertex_descriptor v, out_descriptor o)
        : ends(u, v), out(o)
    { }

    inline directed_edge(std::pair<vertex_descriptor, vertex_descriptor> x, out_descriptor o)
        : ends(x), out(o)
    { }
    //@}

    /** @name Descriptor-like Functions. */
    //@{
    inline bool is_null() const
    { return out.is_null(); }

    inline operator bool() const
    { return !is_null(); }
    //@}

    /** Return the source vertex descriptor. */
    inline vertex_descriptor source() const
    { return ends.first; }

    /** Return the target vertex descriptor. */
    inline vertex_descriptor target() const
    { return ends.second; }

    /** Return the descriptor to the out edge, where the property is stored. */
    inline out_descriptor out_edge() const
    { return out; }

    /** @name Equality Comparable */
    //@{
    inline bool operator==(directed_edge const& x) const
    { return (ends == x.ends) && (out == x.out); }

    inline bool operator!=(directed_edge const& x) const
    { return !operator==(x); }
    //@}

    /** @name Less Than Comparable */
    //@{
    inline bool operator<(directed_edge const& x) const
    { return std::make_pair(ends, out) < std::make_pair(x.ends < x.out); }

    inline bool operator>(directed_edge const& x) const
    { return x.operator<(*this); }

    inline bool operator<=(directed_edge const& x) const
    { return !x.operator<(*this); }

    inline bool operator>=(directed_edge const& x) const
    { return !operator<(x); }
    //@}

    edge_pair       ends;
    out_descriptor  out;
};

template <typename O, typename I>
std::ostream& operator<<(std::ostream& os, const directed_edge<O,I>& e)
{ return os << "(" << e.source() << " " << e.target() << ")"; }

/**
 * The directed edge iterator provides functionality for iterating over the
 * edges of a directed graph. This is essenitally done by iterating over the
 * out edges of each vertex in the order in which they were added to the graph.
 */
template <typename VertexStore, typename Edge>
struct directed_edge_iterator
{
    typedef VertexStore vertex_store;
    typedef typename vertex_store::vertex_type vertex_type;
    typedef typename vertex_store::vertex_iterator vertex_iterator;

    typedef typename vertex_type::vertex_descriptor vertex_descriptor;
    typedef typename vertex_type::out_iterator edge_iterator;
    typedef typename vertex_type::out_descriptor out_descriptor;

    typedef std::forward_iterator_tag iterator_category;
    typedef typename vertex_iterator::difference_type difference_type;
    typedef Edge value_type;
    typedef value_type reference;
    typedef value_type pointer;

    directed_edge_iterator()
        : verts(0), vert(), edge()
    { }

    directed_edge_iterator(vertex_store& store, vertex_iterator i)
        : verts(&store)
        , vert(i)
        , edge((i == store.end_vertices())
            ? edge_iterator()               // empty out iterator
            : vertex(*i).begin_out())       // first out iterator of v
    { }

    /** Return the vertex, given the descriptor. */
    inline vertex_type& vertex(vertex_descriptor v)
    { return verts->vertex(v); }

    /** Return the vertex, given the descriptor. */
    inline vertex_type const& vertex(vertex_descriptor v) const
    { return verts->vertex(v); }

    /** Return the source vertex of the current edge. */
    inline vertex_descriptor source() const
    { return *vert; }

    /** Return the target vertex of the current edge. */
    inline vertex_descriptor target() const
    { return edge->first; }

    /**
     * Return the current out edge by translating the current iterator to a
     * descriptor
     */
    inline out_descriptor out_edge() const
    { return vertex(*vert).out_edge(edge); }

    inline directed_edge_iterator& operator++()
    {
        // If we're not done, increment the current edge. If that increment
        // pushes the edge iterator off the vertex, move to the next vertex
        // and reset the edge iterator.
        if(vert != verts->end_vertices()) {
            ++edge;
            if(edge == vertex(*vert).end_out()) {
                // Move to the next vertex, resetting the edge iterator.
                ++vert;
                edge = (vert != verts->end_vertices())
                    ? vertex(*vert).begin_out()
                    : edge_iterator();
            }
        }
        return *this;
    }

    inline directed_edge_iterator operator++(int)
    { directed_edge_iterator x(*this); operator++(); return x; }

    /** @name Equality Comparable */
    //@{
    inline bool operator==(directed_edge_iterator const& x) const
    { return (vert == x.vert) && (edge == x.edge); }

    inline bool operator!=(directed_edge_iterator const& x) const
    { return !operator==(x); }
    //@}

    inline reference operator*() const
    { return Edge(source(), target(), out_edge()); }

    vertex_store*       verts;
    vertex_iterator     vert;
    edge_iterator       edge;
};

#endif
