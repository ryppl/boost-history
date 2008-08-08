
#ifndef ALGORITHMS_DEPTH_FIRST_HPP
#define ALGORITHMS_DEPTH_FIRST_HPP

#include <boost/graphs/properties.hpp>
#include <boost/graphs/colors.hpp>
#include <boost/graphs/directional_edge.hpp>

#include <boost/graphs/algorithms/utility.hpp>
#include <boost/graphs/algorithms/iterator.hpp>

struct dfs_visitor
{
    // Called when the vertex is popped from the stack. Not all examined
    // vertices are discovered (e.g., a start vertex). Use this event to
    // record the depth-first ordering of vertices.
    template <typename Graph, typename Vertex>
    inline void examine_vertex(Graph const& g, Vertex v)
    { }

    // Called when a vertex is encountered for the first time. Discovered
    // vertices are examined unless the algorithm terminates before they are
    // popped from the stack.
    template <typename Graph, typename Vertex>
    inline void discover_vertex(Graph const& g, Vertex v)
    { }

    // Called when all of the incident edges of the vertex have been examined.
    template <typename Graph, typename Vertex>
    inline void finish_vertex(Graph const& g, Vertex v)
    { }

    // Called for every incident edge of a vertex being examined. Examined
    // edges are classified by the color of their targets at the time of
    // investigation.
    template <typename Graph, typename Edge>
    inline void examine_edge(Graph const& g, Edge e)
    { }

    // Called for each examined edge that becomes part of the search tree. A
    // tree edge is one whose source is being examined and whose target is
    // discovered.
    template <typename Graph, typename Edge>
    inline void tree_edge(Graph const& g, Edge e)
    { }

    // Called for each examined edge whose target is in the search buffer. Back
    // edges, cross edges, and forward edges are nontree edges.
    template <typename Graph, typename Edge>
    inline void back_edge(Graph const& g, Edge e)
    { }

    // Called for each examined edge whose target has already been removed from
    // the search buffer. Technically, this is either a cross edge or a forward
    // edge, but there's no way to easily distinguish them.
    template <typename Graph, typename Edge>
    inline void cross_edge(Graph const& g, Edge e)
    { }
};

#include <boost/graphs/algorithms/depth_first/algorithm.hpp>

#endif
