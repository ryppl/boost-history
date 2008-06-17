
#ifndef EDGE_HPP
#define EDGE_HPP

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


#endif
