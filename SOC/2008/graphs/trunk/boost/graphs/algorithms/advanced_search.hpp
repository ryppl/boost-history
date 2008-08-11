
#ifndef SEARCH_HPP
#define SEARCH_HPP

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
struct search_visitor
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

// The default visit predicate always returns true, indicating that the target
// vertex will be visited (discovered and pushed into the buffer).
struct default_visit_predicate
{
    template <typename Graph, typename Vertex>
    inline bool operator()(Graph const&, Vertex)
    { return true; }
};

// The default end predicate always returns false, indicating that the search
// will continue.
struct default_end_predicate
{
    template <typename Graph>
    inline bool operator()(Graph const&)
    { return false; }
};

namespace detail
{
    // TODO: We need a concept that abstracts stacks and queues into a in/out 
    // search buffer. Concept maps can easily provide default functions for
    // these things - or build a concept-class. For now, we're just sticking
    // a couple of overloads in this namespace.
#if BOOST_HAS_CONCEPTS
concept InOutSequence<typename Container>
{
    typename value_type;
    
    void push(Container& c, value_type const& x)
    { c.push(x); }
    
    void pop(Container& c)
    { c.pop(); }

    value_type const& peek(Container&);
};

template <typename T, typename Container>
concept_map InOutSequence<std::stack<T, Container>>
{
    typedef std::stack<T, Container>::value_type value_type;
    
    value_type const& peek(std::stack<T, Container>& c)
    { return c.top(); }
};

template <typename T, typename Container>
concept_map InOutSequence<std::queue<T, Container>>
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
 * @requires InOutSequence<Buffer>
 * @requires Predicate<VisitPredicate, Graph, Vertex>
 *
 * @requires SameType<ColorMap::key_type, Vertex>
 * @requires SameType<Buffer::value_type, Vertex>
 */
template <
    typename Graph, 
    typename Vertex, 
    typename Edge, 
    typename Visitor, 
    typename ColorMap,
    typename Buffer,
    typename VisitPredicate>
void
search_edge(Graph const& g, 
            Vertex u, 
            Edge e, 
            Visitor vis, 
            ColorMap color, 
            Buffer& buf,
            VisitPredicate visit)
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;

    // Examine the edge before checking it.
    vis.examine_edge(g, e);

    // Look at the color of the target vertex and decide what to do with it.
    // Vertices are only visited if the visit() predicate returns true.
    Vertex v = e.target();
    typename ColorMap::value_type c = color(v);
    if(c == ColorTraits::white() && visit(g, v)) {
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
    typename Visitor, 
    typename ColorMap, 
    typename Buffer,
    typename VisitPredicate>
void
search_vertex(Graph const& g, 
              Vertex u, 
              Visitor vis, 
              ColorMap color, 
              Buffer& buf,
              VisitPredicate visit)
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
        search_edge(g, u, e, vis, color, buf, visit);
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
    typename Visitor, 
    typename ColorMap, 
    typename Buffer,
    typename VisitPredicate,
    typename EndPredicate>
void
search_vertices(Graph const& g, 
                Visitor vis, 
                ColorMap color, 
                Buffer& buf,
                VisitPredicate visit,
                EndPredicate end)
{
    typedef typename Graph::vertex_descriptor Vertex;

    // Continue the search until we're done with the buf.
    while(!buf.empty() && !end(g)) {
        Vertex u = detail::peek(buf);
        buf.pop();
        search_vertex(g, u, vis, color, buf, visit);
    }
}

/**
 * Search from the given vertex to all others connected to it.
 */
template <
    typename Graph, 
    typename Vertex, 
    typename Visitor, 
    typename ColorMap, 
    typename Buffer,
    typename VisitPredicate,
    typename EndPredicate>
void
search_from_vertex(Graph const& g, 
                   Vertex start, 
                   Visitor vis, 
                   ColorMap color, 
                   Buffer& buf,
                   VisitPredicate visit = default_visit_predicate(),
                   EndPredicate end = default_end_predicate())
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;

    // Initialize the start vertex and put it in the buf.
    color(start, ColorTraits::gray());
    buf.push(start);
    vis.start_vertex(g, start);
    vis.discover_vertex(g, start);
    search_vertices(g, vis, color, buf, visit, end);
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
    typename Buffer,
    typename VisitPredicate,
    typename EndPredicate>
void
search_from_vertices(Graph const& g, 
                     Iter f, Iter l, 
                     Visitor vis, 
                     ColorMap color, 
                     Buffer buf,
                     VisitPredicate visit = default_visit_predicate(),
                     EndPredicate end = default_end_predicate())
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;
    typedef typename Graph::vertex_descriptor Vertex;

    for( ; f != l; ++f) {
        Vertex v = *f;
        color(v, ColorTraits::gray());
        buf.push(v);
        vis.start_vertex(g, v);
        vis.discover_vertex(g, v);
    }
    search_vertices(g, vis, color, buf, visit, end);
}

/**
 * Perform a search over the entire graph, visiting all vertices.
 *
 * This is done by performing a search at each undiscovered vertex in the
 * vertex set of the graph.
 */
template <
    typename Graph, 
    typename Visitor, 
    typename Buffer, 
    typename ColorMap,
    typename VisitPredicate,
    typename EndPredicate>
void
search_graph(Graph const& g, 
             Visitor vis, 
             ColorMap color, 
             Buffer& buf,
             VisitPredicate visit = default_visit_predicate(),
             EndPredicate end = default_end_predicate())
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
            search_from_vertex(g, v, vis, color, buf, visit, end);
        }
    }
}

#endif
