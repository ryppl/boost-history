
#ifndef BOOST_GRAPHS_ADJLIST_UNDIRECTED_EDGE_HPP
#define BOOST_GRAPHS_ADJLIST_UNDIRECTED_EDGE_HPP

#include <iosfwd>

#include <boost/unordered_pair.hpp>
#include <boost/graphs/directional_edge.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

/**
 * This structure implements an unordered edge - sort of. Because the undirected
 * adjacency list class doesn't ever store a concrete edge type, this edge
 * type simply aggregates descriptors in such a way that it defines an edge.
 * This means that this class can also act (and does) as a descriptor itself.
 */
template <typename VertexDesc, typename PropDesc>
class undirected_edge
{
public:
    typedef VertexDesc vertex_descriptor;
    typedef PropDesc label_descriptor;
    typedef unordered_pair<vertex_descriptor> edge_pair;

    /** @name Constructors */
    //@{
    inline undirected_edge()
        : _ends(), _label()
    { }

    inline undirected_edge(vertex_descriptor u, vertex_descriptor v, label_descriptor p)
        : _ends(u, v), _label(p)
    { }

    inline undirected_edge(std::pair<vertex_descriptor, vertex_descriptor> const& e, label_descriptor p)
        : _ends(e.first, e.second), _label(p)
    { }

    inline undirected_edge(unordered_pair<vertex_descriptor, vertex_descriptor> const& e, label_descriptor p)
        : _ends(e), _label(p)
    { }
    //@}

    /** @name Descriptor-like Functions
     * These allow you to treat the edge descriptor like a typical descriptor.
     * That is, it can be tested for null status (which defers to the _labelerty
     * descriptor).
     */
    //@{
    inline bool is_null() const
    { return _label.is_null(); }

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

    /** Return true if the edge connects the two vertices. */
    inline bool connects(vertex_descriptor u, vertex_descriptor v) const
    { return make_unordered_pair(u, v) == _ends; }


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

} /* namespace adjacency_list */

namespace detail
{
    // Provide an implementation of directionality for undirected edges.
    template <typename Vert, typename Prop>
    struct directional_edge_adapter<adjacency_list::undirected_edge<Vert,Prop>>
        : adjacency_list::undirected_edge<Vert,Prop>
    {
        inline directional_edge_adapter()
            : adjacency_list::undirected_edge<Vert, Prop>(), src()
        { }

        inline directional_edge_adapter(adjacency_list::undirected_edge<Vert,Prop> e, Vert s)
            : adjacency_list::undirected_edge<Vert,Prop>(e), src(s)
        { }

        inline directional_edge_adapter(Vert s, Vert t)
            : adjacency_list::undirected_edge<Vert,Prop>(s, t), src(s)
        { }

        inline Vert source() const
        { return src; }

        inline Vert target() const
        { return this->opposite(src); }

        Vert src;
    };
}

} } /* namespace boost::graphs */

#endif
