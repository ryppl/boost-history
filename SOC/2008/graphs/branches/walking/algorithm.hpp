
#ifndef BREADTH_FIRST_ALGORITHM_HPP
#define BREADTH_FIRST_ALGORITHM_HPP

#include <queue>

// Contents:
//
// breadth_first_visit_edge - Visits a single edge.
// breadth_first_visit_vertex - Visits a single vertex and all of its incident edges.
// breadth_first_walk - Visits all queued vertices
// breadth_first_visit (single) - Visits all vertices connected to the given vertex.
// breadth_first_visit (mulitple) - Visits all vertices connected to the given vertices
// breadth_first_search - Visits all vertices

// Interesting options for extension:
// visit_if - Only visit the vertex if the predicate is satsified.
// visit_until - Continue the search until some predicate has been reached.

template <typename Graph, typename Vertex, typename Edge, typename Visitor, typename ColorMap, typename Queue>
void breadth_first_visit_edge(Graph const& g, Vertex u, Edge e, Visitor vis, ColorMap color, Queue& queue)
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;

    // Look at the color of the target vertex and decide what to do with it.
    Vertex v = e.target();
    typename ColorMap::value_type c = color(v);
    if(c == ColorTraits::white()) {
        // Visit the new edge and vertex.
        vis.tree_edge(g, e);
        color(v) = ColorTraits::gray();
        queue.push(v);
        vis.discover_vertex(g, v);
    }
    else {
        // Handle other cases - nontree edges, gray targets, black, etc.
        vis.non_tree_edge(g, e);
        if(c == ColorTraits::gray()) {
            vis.gray_target(g, v);
        }
        else {
            vis.black_target(g, v);
        }
    }
}

template <typename Graph, typename Vertex, typename Visitor, typename ColorMap, typename Queue>
void
breadth_first_visit_vertex(Graph const& g, Vertex u, Visitor vis, ColorMap color, Queue& queue)
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;
    typedef typename Graph::edge_descriptor Edge;
    typedef typename Graph::incident_edge_range EdgeRange;

    EdgeRange edges = g.incident_edges(u);
    for( ; edges.first != edges.second; ++edges.first) {
        // Impose direction on this edge (if none exists).
        directional_edge<Edge> e(*edges.first, u);
        breadth_first_visit_edge(g, u, e, vis, color, queue);
    }
    color(u) = ColorTraits::black();
    vis.finish_vertex(g, u);
}

template <typename Graph, typename Visitor, typename ColorMap, typename Queue>
void
breadth_first_walk(Graph const& g, Visitor vis, ColorMap color, Queue& queue)
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;
    typedef typename Graph::vertex_descriptor Vertex;

    // Run the walk on the vertices attached to start.
    while(!queue.empty()) {
        Vertex u = queue.front();
        queue.pop();
        breadth_first_visit_vertex(g, u, vis, color, queue);
    }
}

template <
    typename Graph,
    typename Visitor,
    typename ColorMap = optional_vertex_map<Graph, color>,
    typename Queue = std::queue<typename Graph::vertex_descriptor>>
void
breadth_first_visit(
    Graph const& g,
    typename Graph::vertex_descriptor start,
    Visitor vis,
    ColorMap color = ColorMap(),
    Queue queue = Queue())
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;

    detail::optional_prop_init(g, color, ColorTraits::white());

    // Initialize the starting vertex and run the search.
    color(start) = ColorTraits::white();
    queue.push(start);
    vis.discover_vertex(g, start);
    breadth_first_walk(g, vis, color, queue);
}


template <
    typename Graph,
    typename Iter,
    typename Visitor,
    typename ColorMap = optional_vertex_map<Graph, color>,
    typename Queue = std::queue<typename Graph::vertex_descriptor>>
void
breadth_first_visit(Graph const& g, Iter f, Iter l, Visitor vis, ColorMap color = ColorMap(), Queue queue = Queue())
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;
    typedef typename Graph::vertex_descriptor Vertex;

    detail::optional_prop_init(g, color, ColorTraits::white());

    // Initialize the strarting vertices.
    for( ; f != l; ++f) {
        Vertex v = *f;
        color(v) = ColorTraits::gray();
        queue.push(v);
        vis.discover_vertex(v, g);
    }
    breadth_first_walk(g, vis, color, queue);
}

template <
    typename Graph,
    typename Visitor,
    typename ColorMap = optional_vertex_map<Graph, color>,
    typename Queue = std::queue<typename Graph::vertex_descriptor>>
void
breadth_first_search(Graph& g, Visitor vis, ColorMap color = ColorMap(), Queue queue = Queue())
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;
    typedef typename Graph::vertex_descriptor Vertex;
    typedef typename Graph::vertex_range VertexRange;

    detail::optional_prop_init(g, color, ColorTraits::white());

    // Iterate over all vertices, running BFS visit on each that hasn't been
    // discovered yet.
    VertexRange verts = g.vertices();
    for( ; verts.first != verts.second; ++verts.first) {
        Vertex v = *verts.first;
        if(color(v) == ColorTraits::white()) {
            breadth_first_visit(g, v, vis, color, queue);
        }
    }
}

#endif
