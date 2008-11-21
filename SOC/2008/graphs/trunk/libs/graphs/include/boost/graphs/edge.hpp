
#ifndef BOOST_GRAPHS_EDGE_HPP
#define BOOST_GRAPHS_EDGE_HPP

#include <boost/none.hpp>

namespace boost { namespace graphs {

// There are two faces of the edge interface - the generic specification for
// edge objects is used internally by varios graph components and relies
// entirely on implementations defined by the edge stores.
// - Ends ends(Edge)
// - VertexDesc first(Edge)
// - VertexDesc second(Edge)
// - VertexDesc source(Edge) - Same as first()
// - VertexDesc target(Edge) - Same as second()
// - VertexDesc opposite(Edge, VertexDesc)

// The public, generic interface is written in terms of graph objects and
// descriptors.

/**
 * The edge traits class provides information about the end points and semantics
 * of edges, but not their labels.
 */
template <typename Edge>
struct edge_traits
{
    typedef typename Edge::vertex_descriptor vertex_descriptor;
    typedef typename Edge::edge_ends edge_ends;
};

} } /* namespace boost::graphs */


#endif
