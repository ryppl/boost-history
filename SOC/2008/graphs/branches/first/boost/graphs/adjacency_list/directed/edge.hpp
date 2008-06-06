
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_DIRECTED_EDGE_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_DIRECTED_EDGE_HPP

#include <boost/graphs/utility/ordered_pair.hpp>
#include <boost/graphs/adjacency_list/edge.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * A directed edge 
 */
template <
        typename VertexProps,
        typename EdgeProps,
        typename VertexDesc,
        typename EdgeDesc
    >
class directed_edge
    : public edge<EdgeProps>
{
public:
    typedef EdgeDesc descriptor_type;
    typedef typename edge<EdgeProps>::properties_type properties_type;

    // This seems a little funky, but it turns out that some edge stores can
    // leverage vertex properties for add/insert functions.
    typedef VertexDesc vertex_descriptor;

    directed_edge();
    directed_edge(properties_type const& ep);
    directed_edge(vertex_descriptor u, vertex_descriptor v);
    directed_edge(vertex_descriptor u, vertex_descriptor v, properties_type const& ep);

    unordered_pair<vertex_descriptor> const& ends() const;

    vertex_descriptor source() const;
    vertex_descriptor target() const;
    vertex_descriptor opposite(vertex_descriptor v) const;

private:
    unordered_pair<vertex_descriptor> _ends;
};


// Edge Functions

#define BOOST_GRAPH_UE_PARAMS \
    typename VP, typename EP, typename V, typename E

template <BOOST_GRAPH_UE_PARAMS>
directed_edge<VP,EP,V,E>::directed_edge()
    : edge<EP>()
    , _ends()
{ }

template <BOOST_GRAPH_UE_PARAMS>
directed_edge<VP,EP,V,E>::directed_edge(properties_type const& ep)
    : edge<EP>(ep)
    , _ends()
{ }

template <BOOST_GRAPH_UE_PARAMS>
directed_edge<VP,EP,V,E>::directed_edge(vertex_descriptor u, vertex_descriptor v)
    : edge<EP>()
    , _ends(u, v)
{ }

template <BOOST_GRAPH_UE_PARAMS>
directed_edge<VP,EP,V,E>::directed_edge(vertex_descriptor u,
                                            vertex_descriptor v,
                                            properties_type const& ep)
    : edge<EP>(ep)
    , _ends(u, v)
{ }

/**
 * Return the endpoints of the edge.
 */
template <BOOST_GRAPH_UE_PARAMS>
unordered_pair<typename directed_edge<VP,EP,V,E>::vertex_descriptor> const&
directed_edge<VP,EP,V,E>::ends() const
{ return _ends; }

/**
 * Return the source of this edge. The source of an undirected edge is not
 * necessarily the same as the way it is connected.
 */
template <BOOST_GRAPH_UE_PARAMS>
typename directed_edge<VP,EP,V,E>::vertex_descriptor
directed_edge<VP,EP,V,E>::source() const
{ return _ends.first(); }

/**
 * Return the target of this edge. The source of an undirected edge is not
 * necessarily the same as the way it is connected.
 */
template <BOOST_GRAPH_UE_PARAMS>
typename directed_edge<VP,EP,V,E>::vertex_descriptor
directed_edge<VP,EP,V,E>::target() const
{ return _ends.second(); }

/**
 * Return the vertex opposite the given vertex on this edge.
 */
template <BOOST_GRAPH_UE_PARAMS>
typename directed_edge<VP,EP,V,E>::vertex_descriptor
directed_edge<VP,EP,V,E>::opposite(vertex_descriptor u) const
{
    return u == _ends.first()  ? _ends.second() : _ends.first();
}

// Operators

// @todo I don't know that I like this. It might be better to build these
// operators as functors and then typedef them into the edge type rather than
// simply make these the default operators for the edges.

/**
 * Edges can be ordered by their end points. This is a lexicographical
 * comparison of the vertex descriptors in the ends of the edge.
 */
template <BOOST_GRAPH_UE_PARAMS>
bool
operator<(directed_edge<VP,EP,V,E> const& a,
          directed_edge<VP,EP,V,E> const& b)
{
    return a.ends() < b.ends();
}

/**
 * Edges are also equality comparable by their end points. Note that this does
 * /not/ compare the properties of vertices.
 */
template <BOOST_GRAPH_UE_PARAMS>
bool
operator==(directed_edge<VP,EP,V,E> const& a,
           directed_edge<VP,EP,V,E> const& b)
{
    return a.ends() == b.ends();
}

#undef BOOST_GRAPH_UE_PARAMS


} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
