#ifndef BOOST_GRAPHS_ADJLIST_DIRECTED_EDGE_HPP
#define BOOST_GRAPHS_ADJLIST_DIRECTED_EDGE_HPP

#include <iosfwd>
#include <iterator>

#include <boost/graphs/directional_edge.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

/**
 * A directed edge represents an edge in a directed graph. A directed edge is
 * uniquely identified by its source and target vertices the descriptor into
 * the source vertex's out edge list, which also happens to uniquely describe
 * the edge property, if applicable.
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

    /** @name End Points
     * Provide access to the source and target of the directed edge. The
     * opposite member provides parity with undirected edge type. The first and
     * second accessors provide direction-free semantics, but return the source
     * and target() directly.
     */
    //@{
    inline vertex_descriptor source() const
    { return ends.first; }

    inline vertex_descriptor target() const
    { return ends.second; }

    inline vertex_descriptor first() const
    { return ends.first; }

    inline vertex_descriptor second() const
    { return ends.second; }

    inline vertex_descriptor opposite(vertex_descriptor v) const
    { return v == source() ? target() : source(); }
    //@}

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
 * This is gonna be a little funky.
 */
template <typename O, typename I>
std::size_t hash_value(directed_edge<O,I> const& e)
{
    using boost::hash;
    return hash_value(e.out);
}

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
        : store(0), vert(), edge()
    { }

    directed_edge_iterator(vertex_store& s)
        : store(&s), vert(store->end_vertices()), edge()
    { }

    directed_edge_iterator(vertex_store& s, vertex_iterator i)
        : store(&s)
        , vert(i)
        , edge(next(vertex().begin_out(), true))
    { }

    /** Locate the next valid edge iterator, skipping the update on init. */
    inline edge_iterator next(edge_iterator e, bool first = false)
    {
        if(vert == store->end_vertices()) {
            // If we're at the end already, don't do anything, Just return the same
            // iterator that we got.
            return e;
        }
        else {
            if(e != vertex().end_out() && !first) {
                ++e;
            }

            // If after incrementing, we reach the end of the vertices, then
            // we have to skip to the next non-empty vertex or the end of the
            // sequence, whichever comes first.
            while(vert != store->end_vertices() && e == vertex().end_out())
            {
                ++vert;
                e = vertex().begin_out();
            }
            return vert != store->end_vertices() ? e : edge_iterator();
        }
    }

    /** Return the vertex, given the descriptor. */
    inline vertex_type& vertex()
    { return store->vertex(*vert); }

    /** Return the vertex, given the descriptor. */
    inline vertex_type const& vertex() const
    { return store->vertex(*vert); }

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
    { return vertex().out_edge(edge); }

    inline directed_edge_iterator& operator++()
    {
        if(vert != store->end_vertices()) {
            edge = next(edge);
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

    vertex_store*       store;
    vertex_iterator     vert;
    edge_iterator       edge;
};

} } } /* namespace boost::graphs::adjacency_list */

#endif
