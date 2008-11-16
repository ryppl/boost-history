
#ifndef BOOST_GRAPHS_ADJMATRIX_UNDIRECTED_EDGE_HPP
#define BOOST_GRAPHS_ADJMATRIX_UNDIRECTED_EDGE_HPP

#include <iosfwd>

#include <boost/unordered_pair.hpp>
#include <boost/graphs/directional_edge.hpp>

namespace boost { namespace graphs { namespace adjacency_matrix {

/**
 * This structure implements an undirected edge for an adjacency matrix. The
 * undirected edge is a combination of unordered endpoints, representing the
 * two vertices and a (mostly) unused matrix descriptor that defines the
 * position of the edge label within each matrix element.
 */
template <typename VertexDesc, typename LabelDesc>
class undirected_edge
{
public:
    typedef VertexDesc vertex_descriptor;
    typedef LabelDesc label_descriptor;
    typedef unordered_pair<vertex_descriptor> edge_pair;

    /** @name Constructors */
    //@{
    inline undirected_edge()
        : _ends(), _label()
    { }

    inline undirected_edge(vertex_descriptor u,
                           vertex_descriptor v,
                           label_descriptor l = label_descriptor())
        : _ends(u, v), _label(l)
    { }

    inline undirected_edge(std::pair<vertex_descriptor, vertex_descriptor> const& e,
                           label_descriptor l = label_descriptor())
        : _ends(e.first, e.second), _label(l)
    { }

    inline undirected_edge(unordered_pair<vertex_descriptor, vertex_descriptor> const& e,
                           label_descriptor l = label_descriptor())
        : _ends(e), _label(l)
    { }
    //@}

    /** @name Descriptor-like Functions
     * These allow you to treat the edge descriptor like a typical descriptor.
     * That is, it can be tested for null status (which defers to the _labelerty
     * descriptor).
     */
    //@{
    inline bool is_null() const
    { return _ends.first().is_nul() || _ends.second().is_null(); }

    inline operator bool() const
    { return !is_null(); }
    //@}

    inline label_descriptor label() const
    { return _label; }

    inline edge_pair const& edge() const
    { return _ends; }

    /** @name End Points
     * Provide access to the end points of the undirected edge. Because the
     * _ends of this edge are unordered, they do not provide a source and target
     * interface. See the rooted_undirected_edge class for a variant of this
     * type that imposes a source/target interface on these edges.
     */
    //@{
    inline vertex_descriptor first() const
    { return _ends.first(); }

    inline vertex_descriptor second() const
    { return _ends.second(); }

    inline vertex_descriptor opposite(vertex_descriptor v) const
    { return v == first() ? second() : first(); }
    //@}

    /** @name Equality Comparable */
    //@{
    inline bool operator==(undirected_edge const& x) const
    { return (_ends == x._ends) && (_label == x._label); }

    inline bool operator!=(undirected_edge const& x) const
    { return !operator==(x); }
    //@}

    /** @name Less Than Comparable */
    //@{
    inline bool operator<(undirected_edge const& x) const
    { return std::make_pair(_ends, _label) < std::make_pair(x._ends < x._label); }

    inline bool operator>(undirected_edge const& x) const
    { return x.operator<(*this); }

    inline bool operator<=(undirected_edge const& x) const
    { return !x.operator<(*this); }

    inline bool operator>=(undirected_edge const& x) const
    { return !operator<(x); }
    //@}

private:
    edge_pair           _ends;
    label_descriptor    _label;
};

template <typename V, typename P>
std::ostream& operator<<(std::ostream& os, undirected_edge<V,P> const& e)
{ return os << "{" << e.first() << " " << e.second() << "}"; }

/**
 * The hash value of edges can be computed over the hash value of the property
 * descriptors. This is because the property descriptor provides a global
 * context for the edge.
 */
template <typename V, typename P>
inline std::size_t
hash_value(undirected_edge<V,P> const& e)
{
    using boost::hash;
    return hash_value(e.label());
}

} } }

#endif
