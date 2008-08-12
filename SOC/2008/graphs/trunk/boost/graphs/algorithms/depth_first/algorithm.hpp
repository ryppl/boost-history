
#ifndef DEPTH_FIRST_ALGORITHM_HPP
#define DEPTH_FIRST_ALGORITHM_HPP

#include <stack>

template <typename Graph, typename Vertex, typename Edge, typename Visitor, typename ColorMap, typename Stack>
void
depth_first_visit_edge(Graph const& g, Vertex u, Edge e, Visitor vis, ColorMap color, Stack& stack)
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;

    // Examine the edge before checking it.
    vis.examine_edge(g, e);

    // Look at the color of the target vertex and decide what to do with it.
    Vertex v = e.target();
    typename ColorMap::value_type c = color(v);
    if(c == ColorTraits::white()) {
        color(v) = ColorTraits::gray();
        stack.push(v);

        // Discover the vertex and claim that its a tree edge.
        vis.discover_vertex(g, v);
        vis.tree_edge(g, e);
    }
    else {
        // Handle other cases - nontree edges, gray targets, black, etc.
        if(c == ColorTraits::gray()) {
            vis.back_edge(g, e);
        }
        else {
            vis.cross_edge(g, e);
        }
    }
}

template <typename Graph, typename Vertex, typename Visitor, typename ColorMap, typename Stack>
void
depth_first_visit_vertex(Graph const& g, Vertex u, Visitor vis, ColorMap color, Stack& stack)
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;
    typedef typename Graph::edge_descriptor Edge;
    typedef typename Graph::incident_edge_range EdgeRange;

    // Examine the vertex.
    vis.examine_vertex(g, u);

    // Visit adjacent vertices
    EdgeRange edges = g.incident_edges(u);
    for( ; edges.first != edges.second; ++edges.first) {
        directional_edge<Edge> e(*edges.first, u);
        depth_first_visit_edge(g, u, e, vis, color, stack);
    }
    color(u) = ColorTraits::black();
    vis.finish_vertex(g, u);
}

template <typename Graph, typename Visitor, typename ColorMap, typename Stack>
void
depth_first_walk(Graph const& g, Visitor vis, ColorMap color, Stack& stack)
{
    typedef typename Graph::vertex_descriptor Vertex;

    // Continue the search until we're done with the stack.
    while(!stack.empty()) {
        Vertex u = stack.top();
        stack.pop();
        depth_first_visit_vertex(g, u, vis, color, stack);
    }
}

template <
    typename Graph,
    typename Vertex,
    typename Visitor,
    typename ColorMap = optional_vertex_map<Graph, color>,
    typename Stack = std::stack<typename Graph::vertex_descriptor>>
void
depth_first_visit(Graph const& g, Vertex start, Visitor vis, ColorMap color = ColorMap(), Stack stack = Stack())
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;
    typedef typename Graph::edge_descriptor Edge;
    typedef typename Graph::incident_edge_range EdgeRange;

    detail::optional_prop_init(g, color, ColorTraits::white());

    // Initialize the start vertex and put it in the stack.
    color(start) = ColorTraits::gray();
    stack.push(start);
    vis.discover_vertex(g, start);
    depth_first_walk(g, vis, color, stack);
}

template <
    typename Graph,
    typename Iter,
    typename Visitor,
    typename ColorMap = optional_vertex_map<Graph, color>,
    typename Stack = std::stack<typename Graph::vertex_descriptor>>
void
depth_first_visit(Graph const& g, Iter f, Iter l, Visitor vis, ColorMap color = ColorMap(), Stack stack = Stack())
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;
    typedef typename Graph::vertex_descriptor Vertex;

    detail::optional_prop_init(g, color, ColorTraits::white());

    // Initialize the start vertices and put them into the stack.
    for( ; f != l; ++f) {
        Vertex v = *f;
        color(v) = ColorTraits::gray();
        stack.push(v);
        vis.discover_vertex(g, *f);
    }
    depth_first_walk(g, vis, color, stack);
}


#endif
