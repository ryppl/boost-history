
#ifndef UNDIRECTED_EDGE_HPP
#define UNDIRECTED_EDGE_HPP

#include <iosfwd>

#include "unordered_pair.hpp"

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
    typedef PropDesc property_descriptor;
    typedef unordered_pair<vertex_descriptor> edge_pair;

    inline undirected_edge()
        : ends(), prop()
    { }

    inline undirected_edge(vertex_descriptor u, vertex_descriptor v, property_descriptor p)
        : ends(u, v), prop(p)
    { }

    /**
     * Return the underlying value of the property that uniquely describes the
     * edge. This is primarily used to support mapping applications for exterior
     * edge properties.
     */
    inline typename property_descriptor::index_type get() const
    { return prop.get(); }

    inline property_descriptor properties() const
    { return prop; }

    inline edge_pair const& edge() const
    { return ends; }

    inline vertex_descriptor first() const
    { return ends.first(); }

    inline vertex_descriptor second() const
    { return ends.second(); }

    /** Return true if the edge connects the two vertices. */
    inline bool connects(vertex_descriptor u, vertex_descriptor v) const
    {
        reorder(u, v);
        return first() == u && second() == v;
    }

    inline vertex_descriptor opposite(vertex_descriptor v)
    { return v == first() ? second() : first(); }

    edge_pair               ends;
    property_descriptor     prop;
};

template <typename V, typename P>
inline bool
operator==(undirected_edge<V,P> const& a, undirected_edge<V,P> const& b)
{ return a.ends == b.ends && a.prop == b.prop; }

template <typename V, typename P>
inline bool
operator!=(undirected_edge<V,P> const& a, undirected_edge<V,P> const& b)
{ return !(a == b); }

template <typename V, typename P>
inline bool
operator<(undirected_edge<V,P> const& a, undirected_edge<V,P> const& b)
{ return std::make_pair(a.ends, a.prop) < std::make_pair(b.ends < b.props); }

template <typename V, typename P>
std::ostream& operator<<(std::ostream& os, undirected_edge<V,P> const& e)
{ return os << "{" << e.first() << " " << e.second() << "}"; }

template <typename V, typename P>
inline std::size_t
hash_value(undirected_edge<V,P> const& e)
{
    std::size_t seed;
    boost::hash_combine(seed, e.ends);
    boost::hash_combine(seed, e.prop);
    return seed;
}

/**
 * The undirected edge iterator simply wraps the iterator over the global edge
 * property store of undirected graphs.
 */
template <typename Graph>
struct undirected_edge_iterator
{
    typedef typename Graph::property_store store_type;
    typedef typename Graph::vertex_type vertex_type;
    typedef typename Graph::property_descriptor property_descriptor;
    typedef typename Graph::vertex_descriptor vertex_descriptor;

    // This is an iterator directly into the global property store.
    typedef typename store_type::iterator prop_iterator;

    typedef std::forward_iterator_tag iterator_category;
    typedef std::size_t size_type;

    typedef typename Graph::edge_descriptor value_type;
    typedef value_type reference;
    typedef value_type pointer;

    undirected_edge_iterator(store_type const& store, prop_iterator i)
        : store(&store), iter(i)
    { }

    inline undirected_edge_iterator& operator++()
    { ++iter; return *this; }

    inline reference operator*() const
    {
        // Grab the vertex descrip
        vertex_descriptor u = iter->second.template get<vertex_descriptor>();
        vertex_descriptor v = iter->third.template get<vertex_descriptor>();
        return reference(u, v, store->describe(iter));
    }

    store_type const*   store;
    prop_iterator       iter;
};

template <typename Graph>
inline bool
operator==(undirected_edge_iterator<Graph> const& a, undirected_edge_iterator<Graph> const& b)
{ return a.iter == b.iter; }

template <typename Graph>
inline bool
operator!=(undirected_edge_iterator<Graph> const& a, undirected_edge_iterator<Graph> const& b)
{ return !(a == b); }

#endif
