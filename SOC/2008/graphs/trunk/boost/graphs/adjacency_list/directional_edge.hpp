
#ifndef DIRECTIONAL_EDGE_HPP
#define DIRECTIONAL_EDGE_HPP

// This basically wraps a concept called DirectionalEdge. A DirectionalEdge
// is one that has directionality being imposed on it. Specializations of
// these edge should be found with the graph types.

namespace detail
{
    // By default, we assume that the edge is directed.
    template <typename Edge>
    struct directional_edge_adapter : Edge
    {
        typedef typename Edge::vertex_descriptor Vertex;

        inline directional_edge_adapter()
            : Edge()
        { }

        inline directional_edge_adapter(Edge e, Vertex)
            : Edge(e)
        { }

        inline directional_edge_adapter(Vertex s, Vertex t)
            : Edge(s, t)
        { }
    };
}

/**
 * The diretional edge type forces directionality onto the given edge structure
 * even when non exists. That this is not the same as a directed edge, which has
 * a distinct direction to it. This simply imposes a direction over the edge
 * with respect to the a source vertex. For directed graphs, this essentially
 * does nothing. For undirected graphs, the source vertex is used to compute
 * the opposite end (target).
 *
 * Directional edges are intended for use in algorithms that walk the graph
 * structure. The act of walking from one vertex to another adds an implicit
 * directionality to the edge. This class embodies that directionality for both
 * directed and undirected graphs.
 *
 * Directional edge descriptors can be used interchangeably with normal edge
 * descriptors.
 */
template <typename Edge>
struct directional_edge
    : detail::directional_edge_adapter<Edge>
{
    typedef Edge edge_descriptor;
    typedef typename Edge::vertex_descriptor vertex_descriptor;

    inline directional_edge()
        : detail::directional_edge_adapter<Edge>()
    { }

    inline directional_edge(edge_descriptor e, vertex_descriptor v)
        : detail::directional_edge_adapter<Edge>(e, v)
    { }
};

#endif
