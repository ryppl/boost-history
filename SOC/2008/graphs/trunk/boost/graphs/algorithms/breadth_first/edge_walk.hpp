
#ifndef BREADTH_FIRST_EDGE_WALK_HPP
#define BREADTH_FIRST_EDGE_WALK_HPP

#include <queue>

#include "edge_iterator.hpp"

// Multi-rooted BFS: A variant of a BFS that seeds the queue with multiple
// initial vertices, but otherwise proceeds normally. The effect is of searching
// for a vertex in the middle. Should work with both directed and undirected
// graphs. This only applies to the walk, not the traversal.
//
// BF Topological Sort: (Directed only): A variant that postpones the visitation
// of a vertex until all vertices on incoming edges have been visited. This
// actually requires a fundamental change in the structure of the algorithm
// and is not trivially implemented inside this framework. It requries a
// prooperty map that counts the number of dependent edges, only visiting when
// the count reaches 0. This probably only applies to the traversal.


/**
 * The edge-walk implements a breadth-first walk that provides access to the
 * current edge. This is one of the most fundamental algorithm abstractions
 * since it can be used to build any kind of breadth first search including
 * a vertex walk because the source of each edge is the current vertex.
 *
 * This algorithm also supports a multirooted version of the walk. In a multi-
 * homed (or rooted) breadth-first walk, the queue is seeded with multiple
 * vertices. The algorithm proceeds normally.
 *
 * This algorithm object essentially provides the next tree edge at each step.
 *
 * @requires IncidenceGraph<Graph>
 * @requires ReadWritePropertyMap<ColorMap>
 * @requires Queue<Queue>
 */
template <
    typename Graph,
    typename ColorMap = generic_vertex_map<Graph, color>,
    typename Queue = std::queue<typename Graph::vertex_descriptor>>
struct breadth_first_edge_walk
{
    typedef breadth_first_edge_walk<Graph, ColorMap, Queue> this_type;

    typedef typename ColorMap::value_type Color;
    typedef color_traits<Color> ColorTraits;

    typedef Graph graph_type;
    typedef typename Graph::vertex_descriptor vertex_descriptor;
    typedef typename Graph::edge_descriptor edge_descriptor;
    typedef typename Graph::incident_edge_range IncidenceRange;

    typedef breadth_first_edge_iterator<this_type> iterator;

    breadth_first_edge_walk(Graph& g, vertex_descriptor v)
        : graph(g), color(g, ColorTraits::white()), queue(), range()
    { start(v); }

    breadth_first_edge_walk(Graph& g, vertex_descriptor v, ColorMap color)
         : graph(g),  color(color), queue(), range()
    { start(v); }

    // Multi-rooted constructor.
    template <typename Iter>
    breadth_first_edge_walk(Graph& g, Iter f, Iter l, ColorMap color)
        : graph(g), color(color), queue(), range()
    { start(f, l); }

    template <typename Iter>
    breadth_first_edge_walk(Graph& g, Iter f, Iter l)
        : graph(g), color(g, ColorTraits::white()), queue(), range()
    { start(f, l); }

    /** Initialize the walk to the given vertex. */
    void start(vertex_descriptor v)
    {
        // Iniitalize the starting vertex
        color(v) = ColorTraits::gray();
        queue.push(v);

        // Set the starting range and select the first valid edge.
        range = graph.incident_edges(v);
        next();
    }

    /** Initialize the walk with the sequence of roots. */
    template <typename Iter>
    void start(Iter f, Iter l)
    {
        for( ; f != l; ++f) {
            vertex_descriptor v = *f;
            color(v) = ColorTraits::gray();
            queue.push(v);
        }

        // Set the starting range and select the first valid edge.
        range = graph.incidenct_edges(queue.front());
        next();
    }

    /** Move to the next tree edge. */
    void next()
    {
        // Start by looking for the next target.
        next_target();
        if(queue.empty()) {
            return;
        }

        // Set the color of the target to gray and push it into the queue.
        vertex_descriptor v = target();
        color(v) = ColorTraits::gray();
        queue.push(v);
    }

    /**
     * Move to the next vertex by popping the queue and resetting the incident
     * edge range to that of the next vertex. Note that the current vertex is
     * marked done or black at this point.
     */
    void next_vertex()
    {
        // We're done with the current vertex. Mark it black.
        vertex_descriptor u = queue.front();
        color(u) = ColorTraits::black();

        // Move the the next vertex and reset the incident edge range.
        queue.pop();
        if(!queue.empty()) {
            vertex_descriptor v = queue.front();
            range = graph.incident_edges(v);
        }
    }

    /**
     * Find the next target. This will move the algorithm state so that the
     * range points to the next valid edge.
     */
    void next_target()
    {
        // Basically, just keep looping until finished...
        while(!queue.empty()) {
            // If the range is empty, then get a new one by moving to the next
            // vertex. This will color vertices as needed. If we exhaust the
            // queue, break.
            if(range.first == range.second) {
                next_vertex();
                if(queue.empty()) {
                    break;
                }
            }

            // As long as we can check the range, try to find a vertex in this
            // range that hasn't been discovered just yet.
            while(range.first != range.second && !discoverable()) {
                ++range.first;
            }

            // If we found something, then bail.
            if(range.first != range.second) {
                break;
            }
        }
    }

    /**
     * Return the current edge "pointed" to by the algorithm, which will be
     * null if the queue is empty (the algorithm has finished).
     */
    inline edge_descriptor edge() const
    { return queue.empty() ? edge_descriptor() : *range.first; }

    /**
     * Returns the source vertex of the current edge. This is the vertex in the
     * front of the queue, and the origin of the current incident edge.
     */
    inline vertex_descriptor source() const
    { return queue.empty() ? vertex_descriptor() : queue.front(); }

    /**
     * Return the target vertex of the current edge. Note that the the edge
     * pointed to by the algorithm.
     */
    inline vertex_descriptor target() const
    { return queue.empty() ? vertex_descriptor() : edge().opposite(source()); }

    /** Return true if the current target has already been visited. */
    bool discoverable() const
    { return color(target()) == ColorTraits::white(); }

    inline iterator begin()
    { return iterator(this); }

    inline iterator end()
    { return iterator(); }

    Graph& graph;
    ColorMap color;
    Queue queue;
    IncidenceRange range;
};

/**
 * Perform a breadth first walk that reaches all vertices of the graph. This
 * is done by performing walks at each vertex, maintaing the color map between
 * individual executions.
 *
 * The choice of starting vertex is not given in this algorithm since all
 * vertices and edges will be traversed.
 */
template <
    typename Graph,
    typename ColorMap = generic_vertex_map<Graph, color>,
    typename Queue = std::queue<typename Graph::vertex_descriptor>>
struct breadth_first_edge_traversal
{
    typedef breadth_first_edge_traversal<Graph, ColorMap, Queue> this_type;

    typedef Graph graph_type;
    typedef typename Graph::edge_descriptor edge_descriptor;
    typedef typename Graph::vertex_descriptor vertex_descriptor;
    typedef typename Graph::vertex_range vertex_descriptorRange;

    typedef typename ColorMap::value_type Color;
    typedef color_traits<Color> ColorTraits;

    typedef breadth_first_edge_walk<Graph, ColorMap, Queue> Walk;
    typedef breadth_first_edge_iterator<this_type> iterator;

    breadth_first_edge_traversal(Graph& g)
        : graph(g), color(g, ColorTraits::white()), range(g.vertices())
        , walk(graph, *range.first, color)
    { }

    breadth_first_edge_traversal(Graph& g, ColorMap color)
         : graph(g), color(color), range(g.vertices())
         , walk(graph, *range.first, color)
    { }

    void next()
    {
        // We can usually delegate the next step of the traversal to that of
        // the current walk. However, if the next step of the traversal takes
        // us to a null edge, then we have to search for the next unvisited
        // vertex in the graph.
        if(range.first != range.second) {
            walk.next();
        }

        // Did we just walk off the edge?
        if(!walk.edge()) {
            // Find the next connected component.
            while(range.first != range.second && !discoverable()) {
                ++range.first;
            }

            // If we haven't walked off the edge yet, update the walk over
            // the new vertex.
            if(range.first != range.second) {
                walk.start(*range.first);
            }
        }
    }

    inline edge_descriptor edge() const
    { return walk.edge(); }

    inline vertex_descriptor source() const
    { return walk.source(); }

    inline vertex_descriptor target() const
    { return walk.target(); }

    // Returns true if the current edge is discoverable.
    inline bool discoverable() const
    { return color(*range.first) == ColorTraits::white(); }

    inline iterator begin()
    { return iterator(this); }

    inline iterator end()
    { return iterator(); }

    Graph& graph;
    ColorMap color;
    vertex_descriptorRange range;
    Walk walk;
};

#endif
