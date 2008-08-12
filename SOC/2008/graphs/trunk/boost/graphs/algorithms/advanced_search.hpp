
#ifndef ADVANCED_SEARCH_HPP
#define ADVANCED_SEARCH_HPP

#include <stack>
#include <queue>

#include <boost/graphs/colors.hpp>
#include <boost/graphs/properties.hpp>
#include <boost/graphs/algorithms/utility.hpp>

// TODO: One of the big TODO's here is improving the means of constructing
// visitors. It would be nice to provide a set of functions that build and
// aggregate visitors so we could do something like:
// breadth_first_search(g,
//                      on_start_vertex(lambda).
//                      on_discover_vertex(labmda));
// Or something like this. This would probably have to aggregate visitor
// functions through some kind visitor aggregator. Not really sure how to do
// this yet.

/**
 * The default search visitor provdes a number of "hooks" that can be overridden
 * to implement a number of various algorithms. Visitor functions can basically
 * be divided into two types:
 */
struct default_search_visitor
{
    // Called when a search originates from a vertex. This is called once for
    // the root of each search tree.
    template <typename Graph, typename Vertex>
    inline void start_vertex(Graph const& g, Vertex v)
    { }

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

    // Called for eacch edge that is not a tree edge (i.e., whose target has
    // already been discovered). Specific types of non-tree edges can be
    // classified as back or cross edges depending on the color of the target.
    template <typename Graph, typename Edge>
    inline void non_tree_edge(Graph const& g, Edge e)
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

// The default search strategy is the control counterpoint of the search
// visitor. Specifically, it provides a number of functions that can be
// implemented to control various aspects of the search. Note that strategies
// work just like visitors - they are copied into eqch subsequent call.
struct default_search_strategy
{
    // This control point is queried at the top of the search loop when the
    // search buffer is not empty. Returning false will cause the search to
    // end prematurely. This control point should not be called when the buffer
    // is empty.
    template <typename Graph>
    inline bool continue_vertices(Graph const& g)
    { return true; }

    // This control point is queried for each incident of the the vertex v.
    // The vertex here is the one that was most recently examined. Returning
    // true will allow the search of edges to continue, returning false will
    // cause the edge loop to end prematurely.
    template <typename Graph, typename Vertex>
    inline bool continue_edges(Graph const& g, Vertex v)
    { return true; }

    // This control point is when when a previousl undiscovered (white) vertex
    // is found for the first time. Returning true allows the discovery of the
    // vertex, returning false will prevent the search from proceeding along
    // this edge.
    template <typename Graph, typename Vertex>
    inline bool allow_discovery(Graph const& g, Vertex v)
    { return true; }
};

namespace detail
{
    // TODO: We need a concept that abstracts stacks and queues into a in/out
    // search buffer. Concept maps can easily provide default functions for
    // these things - or build a concept-class. For now, we're just sticking
    // a couple of overloads in this namespace.
#if BOOST_HAS_CONCEPTS
concept InOutBuffer<typename Container>
{
    typename value_type;

    void push(Container& c, value_type const& x)
    { c.push(x); }

    void pop(Container& c)
    { c.pop(); }

    value_type const& peek(Container&);
};

template <typename T, typename Container>
concept_map InOutBuffer<std::stack<T, Container>>
{
    typedef std::stack<T, Container>::value_type value_type;

    value_type const& peek(std::stack<T, Container>& c)
    { return c.top(); }
};

template <typename T, typename Container>
concept_map InOutBuffer<std::queue<T, Container>>
{
    typedef std::queue<T, Container>::value_type value_type;

    value_type const& peek(std::queue<T, Container>& c)
    { return c.front(); }
};
#endif

    // Adapters for getting the current element of a buffer.
    template <typename T, typename Seq>
    inline T& peek(std::stack<T, Seq>& s)
    { return s.top(); }

    template <typename T, typename Seq>
    inline T& peek(std::queue<T, Seq>& s)
    { return s.front(); }
}

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
