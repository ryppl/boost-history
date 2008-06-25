
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

    inline undirected_edge();
    inline undirected_edge(vertex_descriptor u, vertex_descriptor v, property_descriptor p);

    inline property_descriptor properties() const
    { return _prop; }

    inline edge_pair const& edge() const
    { return _edge; }

    inline vertex_descriptor first() const
    { return _edge.first(); }

    inline vertex_descriptor second() const
    { return _edge.second(); }

    /** Return true if the edge connects the two vertices. */
    inline bool connects(vertex_descriptor u, vertex_descriptor v) const
    {
        reorder(u, v);
        return first() == u && second() == v;
    }

    inline vertex_descriptor opposite(vertex_descriptor v)
    { return v == first() ? second() : first(); }

    inline bool operator==(undirected_edge const& x);
    inline bool operator!=(undirected_edge const& x);
    inline bool operator<(undirected_edge const& x);

private:
    edge_pair _edge;
    PropDesc _prop;
};

template <typename VD, typename PD>
undirected_edge<VD,PD>::undirected_edge()
    : _edge()
    , _prop()
{ }

template <typename VD, typename PD>
undirected_edge<VD,PD>::undirected_edge(vertex_descriptor u,
                                        vertex_descriptor v,
                                        property_descriptor p)
    : _edge(u, v)
    , _prop(p)
{ }

template <typename VD, typename PD>
bool
undirected_edge<VD,PD>::operator==(undirected_edge const& x)
{
    return _edge == x._edge && _prop == x._prop;
}

template <typename VD, typename PD>
bool
undirected_edge<VD,PD>::operator!=(undirected_edge const& x)
{
    return !(*this->operator==(x));
}

template <typename VD, typename PD>
bool
undirected_edge<VD,PD>::operator<(undirected_edge const& x)
{
    return _edge < x._edge || (!(x._edge < _edge) && _prop < x._prop);
}

template <typename VD, typename PD>
std::ostream& operator<<(std::ostream& os, undirected_edge<VD,PD> const& e)
{ return os << "{" << e.first() << " " << e.second() << "}"; }

/**
 * The undirected edge iterator simply wraps the iterator over the global edge
 * property store of undirected graphs.
 */
template <typename Graph>
struct undirected_edge_iterator
{
    typedef typename Graph::property_store store_type;
    typedef typename Graph::vertex_type vertex_type;
    typedef typename store_type::iterator prop_iterator;
    typedef typename vertex_type::iterator edge_iterator;

    typedef std::forward_iterator_tag iterator_category;
    typedef std::size_t size_type;

    typedef typename Graph::edge_descriptor value_type;
    typedef value_type reference;
    typedef value_type pointer;

    undirected_edge_iterator(prop_iterator i)
        : iter(i)
    { }

    inline undirected_edge_iterator& operator++()
    { ++iter; return *this; }

    inline reference operator*() const
    {
        edge_iterator p = iter->second.template get<edge_iterator>();
        edge_iterator q = iter->third.template get<edge_iterator>();
        return reference(p->first, q->first, iter->first);
    }

    prop_iterator iter;
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
