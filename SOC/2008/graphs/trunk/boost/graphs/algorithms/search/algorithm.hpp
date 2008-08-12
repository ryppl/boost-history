
#ifndef BOOST_GRAPHS_ALGORITHMS_SEARCH_ALGORITHM_HPP
#define BOOST_GRAPHS_ALGORITHMS_SEARCH_ALGORITHM_HPP

#include <boost/graphs/algorithms/search/detail.hpp>

// This file contains the implementatin of the search core. Specifically, this
// includes:
// - search_edge
// - search_vertex
// - search_vertices
// - search_from_vertex
// - search_from_vertices
// - search_graph
// - search_for (x2)

/**
 * Examine the edge e rooted at vertex u to determine the search action to be
 * taken (or not) for the opposite vertex.
 *
 * The visit predicate is called to determine if undiscovered (white) vertices
 * should be visited. Default search actions will always visit white vertices,
 * but these visits can be constrained or deferred by providing a predicate
 * that examines the vertex and returns false.
 *
 * @requires ReadWritePropertyMap<ColorMap>
 * @requires InOutBuffer<Buffer>
 * @requires Predicate<VisitPredicate, Graph, Vertex>
 *
 * @requires SameType<ColorMap::key_type, Vertex>
 * @requires SameType<Buffer::value_type, Vertex>
 */
template <
    typename Graph,
    typename Vertex,
    typename Edge,
    typename ColorMap,
    typename Buffer,
    typename Visitor,
    typename Strategy>
void
search_edge(Graph const& g,
            Vertex u,
            Edge e,
            ColorMap color,
            Buffer& buf,
            Visitor vis,
            Strategy strat)
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;

    // Examine the edge before checking it.
    vis.examine_edge(g, e);

    // Look at the color of the target vertex and decide what to do with it.
    // Vertices are only visited if the visit() predicate returns true.
    Vertex v = e.target();
    typename ColorMap::value_type c = color(v);
    if(c == ColorTraits::white() && strat.allow_discovery(g, v)) {
        color(v, ColorTraits::gray());
        buf.push(v);

        // Discover the vertex and claim that its a tree edge.
        vis.discover_vertex(g, v);
        vis.tree_edge(g, e);
    }
    else {
        // Signal a non-tree edge and classify it.
        vis.non_tree_edge(g, e);
        if(c == ColorTraits::gray()) {
            vis.back_edge(g, e);
        }
        else {
            vis.cross_edge(g, e);
        }
    }
}

template <
    typename Graph,
    typename Vertex,
    typename ColorMap,
    typename Buffer,
    typename Visitor,
    typename Strategy>
void
search_vertex(Graph const& g,
              Vertex u,
              ColorMap color,
              Buffer& buf,
              Visitor vis,
              Strategy strat)
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;
    typedef typename Graph::edge_descriptor Edge;
    typedef typename Graph::incident_edge_range EdgeRange;

    // Examine the vertex.
    vis.examine_vertex(g, u);

    // Visit adjacent vertices.
    EdgeRange edges = g.incident_edges(u);
    while(edges.first != edges.second && strat.continue_edges(g, u)) {
        directional_edge<Edge> e(*edges.first, u);
        search_edge(g, u, e, color, buf, vis, strat);
        ++edges.first;
    }
    color(u, ColorTraits::black());
    vis.finish_vertex(g, u);
}

/**
 * Exucte a search (either breadth or depth, depending on the search buffer)
 * over the vertices given in the queue. This is done by popping a vertex out
 * of the buffer, and searching its adjacent neighbors.
 *
 * The visit predicate is called to determine if undiscovered (white) vertices
 * should be visited. Default search actions will always visit white vertices,
 * but these visits can be constrained or deferred by providing a predicate
 * that examines the vertex and returns false.
 *
 * The end predicate is called to determine if the search should continue or
 * not. The end predicate returning true will end the search prematurely.
 * Default search actions will not end prematurely.
 *
 * @requires ReadWritePropertyMap<ColorMap>
 * @requires Predicate<VisitPredicate, Graph, Vertex>
 * @requries Predicate<EndPredicate, Graph>
 */
template <
    typename Graph,
    typename ColorMap,
    typename Buffer,
    typename Visitor,
    typename Strategy>
void
search_vertices(Graph const& g,
                ColorMap color,
                Buffer& buf,
                Visitor vis,
                Strategy strat)
{
    typedef typename Graph::vertex_descriptor Vertex;

    // Continue the search until we're done with the buf.
    while(!buf.empty() && strat.continue_vertices(g)) {
        Vertex u = detail::peek(buf);
        buf.pop();
        search_vertex(g, u, color, buf, vis, strat);
    }
}

/**
 * Search from the given vertex to all others connected to it.
 */
template <
    typename Graph,
    typename Vertex,
    typename ColorMap,
    typename Buffer,
    typename Visitor = default_search_visitor,
    typename Strategy = default_search_strategy>
void
search_from_vertex(Graph const& g,
                   Vertex start,
                   ColorMap color,
                   Buffer& buf,
                   Visitor vis = Visitor(),
                   Strategy strat = Strategy())
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;

    // Initialize the start vertex and put it in the buf.
    color(start, ColorTraits::gray());
    buf.push(start);
    vis.start_vertex(g, start);
    vis.discover_vertex(g, start);
    search_vertices(g, color, buf, vis, strat);
}

/**
 * Perform a multi-root search from the vertices given in the range [f, l).
 * This is done by seeding the search buffer with the given vertices prior
 * to executing a standard search.
 */
template <
    typename Graph,
    typename Iter,
    typename Visitor,
    typename ColorMap,
    typename Buffer = default_search_visitor,
    typename Strategy = default_search_strategy>
void
search_from_vertices(Graph const& g,
                     Iter first, Iter last,
                     ColorMap color,
                     Buffer buf,
                     Visitor vis = Visitor(),
                     Strategy strat = Strategy())
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;
    typedef typename Graph::vertex_descriptor Vertex;

    for( ; first != last; ++first) {
        Vertex v = *first;
        color(v, ColorTraits::gray());
        buf.push(v);
        vis.start_vertex(g, v);
        vis.discover_vertex(g, v);
    }
    search_vertices(g, color, buf, vis, strat);
}

/**
 * Perform a search over the entire graph, visiting all vertices.
 *
 * This is done by performing a search at each undiscovered vertex in the
 * vertex set of the graph.
 */
template <
    typename Graph,
    typename ColorMap,
    typename Buffer,
    typename Visitor = default_search_visitor,
    typename Strategy = default_search_strategy>
void
search_graph(Graph const& g,
             ColorMap color,
             Buffer& buf,
             Visitor vis = Visitor(),
             Strategy strat = Strategy())
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;
    typedef typename Graph::vertex_descriptor Vertex;
    typedef typename Graph::vertex_range VertexRange;

    VertexRange verts = g.vertices();
    for( ; verts.first != verts.second; ++verts.first) {
        Vertex v = *verts.first;
        if(color(v) == ColorTraits::white()) {
            // TODO: Interesting problemm... Suppose v is white, but has no
            // out edges, but can be reached from some other vertex v'. Here,
            // v will not be part of the tree rooted by v' even though it is
            // reachable.
            search_from_vertex(g, v, color, buf, vis, strat);
        }
    }
}

// The search_for() functions use a specific search strategy that terminates
// the search when a target vertex is located.

// This strategy will continue the search until the target vertex has been
// found. This strategy does not visit the target vertex.
template <typename Graph, typename Vertex>
struct search_for_strategy
{
    search_for_strategy(Vertex const& v, bool& f)
        : target(v), found(f)
    { }

    // Return true if the target vertex has been found.
    inline bool continue_vertices(Graph const& g)
    { return !found; }

    inline bool continue_edges(Graph const& g, Vertex v)
    { return !found; }

    // Set found to true if the target vertex has been found. Return
    inline bool allow_discovery(Graph const& g, Vertex v)
    {
        if(v == target) found = true;
        return !found;
    }

    Vertex const& target;
    bool& found;
};

/**
 * Search for the target vertex tgt starting from the source vertex src. This
 * function returns true if the vertex is found, false otherwise.
 *
 * This works best as a BFS.
 */
template <
    typename Graph,
    typename Vertex,
    typename ColorMap,
    typename Buffer,
    typename Visitor = default_search_visitor>
bool
search_for(Graph const& g,
           Vertex src,
           Vertex tgt,
           ColorMap color,
           Buffer& buf,
           Visitor vis = Visitor())
{
    bool found = (src == tgt);
    search_for_strategy<Graph, Vertex> strat(tgt, found);
    search_from_vertex(g, src, color, buf, vis, strat);
    return strat.found;
}

/**
 * Search for the target vertex tgt starting from the vertices given in the
 * range [first, last). Return true if the vertex is found, false otherwise.
 *
 * This works best as a BFS.
 */
template <
    typename Graph,
    typename Vertex,
    typename Iter,
    typename ColorMap,
    typename Buffer,
    typename Visitor = default_search_visitor>
bool
search_for(Graph const& g,
           Iter first, Iter last,
           Vertex tgt,
           ColorMap color,
           Buffer& buf,
           Visitor vis = Visitor())
{
    bool found = (std::find(first, last, tgt) != last);
    search_for_strategy<Graph, Vertex> strat(tgt, found);
    search_from_vertices(g, first, last, color, buf, vis, strat);
    return strat.found;
}

#endif
