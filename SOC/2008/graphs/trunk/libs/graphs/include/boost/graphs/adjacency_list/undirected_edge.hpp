
#ifndef BOOST_GRAPHS_ADJLIST_UNDIRECTED_EDGE_HPP
#define BOOST_GRAPHS_ADJLIST_UNDIRECTED_EDGE_HPP

#include <iosfwd>

#include <boost/functional/hash.hpp>

#include <boost/graphs/edge.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

/**
 * This structure implements an undirected edge. An undirected edge maintains
 * a pair of vertices and an edge label.
 */
template <typename EdgeLabel, typename VertexDesc>
class undirected_edge
{
public:
    typedef EdgeLabel label_type;

    typedef VertexDesc vertex_descriptor;
    typedef std::pair<vertex_descriptor, vertex_descriptor> ends_pair;

    /** @name Constructors */
    //@{
    undirected_edge()
        : _ends(), _label()
    { }

    undirected_edge(vertex_descriptor u, vertex_descriptor v,  label_type const& l)
        : _ends(u, v), _label(l)
    { }

    undirected_edge(ends_pair const& e, label_type const& l)
        : _ends(e.first, e.second), _label(l)
    { }
    //@}

    label_type& label()                 { return _label; }
    label_type const& label() const     { return _label; }

    ends_pair const& ends() const       { return _ends; }

private:
    ends_pair       _ends;
    label_type      _label;
};

/** @name Equality Comparable */
//@{
template <typename L, typename VD>
inline bool operator==(undirected_edge<L,VD> const& a, undirected_edge<L,VD> const& b)
{ return (a.ends() == b.ends()) && (a.label() == b.label()); }

template <typename L, typename VD>
inline bool operator!=(undirected_edge<L,VD> const& a, undirected_edge<L,VD> const& b)
{ return !(a == b); }
//@}

/** @name Less Than Comparable */
//@{
template <typename L, typename VD>
inline bool operator<(undirected_edge<L,VD> const& a, undirected_edge<L,VD> const& b)
{ return std::make_pair(a.ends(), a.label()) < std::make_pair(b.ends(), b.label()); }

template <typename L, typename VD>
inline bool operator>(undirected_edge<L,VD> const& a, undirected_edge<L,VD> const& b)
{ return b < a; }

template <typename L, typename VD>
inline bool operator<=(undirected_edge<L,VD> const& a, undirected_edge<L,VD> const& b)
{ return !(b < a); }

template <typename L, typename VD>
inline bool operator>=(undirected_edge<L,VD> const& a, undirected_edge<L,VD> const& b)
{ return !(a < b); }
//@}


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
{ return hash_value(e.label()); }

/**
 * The undirected edge iterator simply wraps the iterator over the global edge
 * property store of undirected graphs.
 */
template <typename Store, typename Edge>
struct undirected_edge_iterator
{
    typedef typename Store::iterator iterator;
    typedef typename Store::vertex_descriptor vertex_descriptor;
    typedef typename Store::incidence_descriptor incidence_descriptor;
    typedef typename Store::label_descriptor label_descriptor;

    typedef typename iterator::iterator_category iterator_category;
    typedef typename iterator::difference_type difference_type;
    typedef Edge value_type;
    typedef value_type reference;
    typedef value_type pointer;

    undirected_edge_iterator()
        : store(0), iter()
    { }

    undirected_edge_iterator(undirected_edge_iterator const& x)
        : store(x.store), iter(x.iter)
    { }

    undirected_edge_iterator(Store& store, iterator i)
        : store(&store), iter(i)
    { }

    inline undirected_edge_iterator& operator++()
    { ++iter; return *this; }

    inline undirected_edge_iterator& operator--()
    { --iter; return *this; }

    inline undirected_edge_iterator operator++(int)
    { undirected_edge_iterator x(*this); operator++(); return x; }

    inline undirected_edge_iterator operator--(int)
    { undirected_edge_iterator x(*this); operator--(); return x; }

    inline undirected_edge_iterator& operator+=(difference_type n)
    { iter += n; return *this; }

    inline undirected_edge_iterator& operator-=(difference_type n)
    { iter -= n; return *this; }

    /** @name Equality Comparable */
    //@{
    inline bool operator==(undirected_edge_iterator const& x) const
    { return iter == x.iter; }

    inline bool operator!=(undirected_edge_iterator const& x) const
    { return iter != x.iter; }
    //@{

    inline difference_type operator-(undirected_edge_iterator x) const
    { return iter - x.iter; }

    inline reference operator*() const
    {
        label_descriptor p = store->describe(iter);
        return Edge(store->first_vertex(p), store->second_vertex(p), p);
    }

    Store*      store;
    iterator    iter;
};

} } } /* namespace boost::graphs::adjacency_list */

#endif
