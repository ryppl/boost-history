
#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <stack>
#include <queue>

#include <boost/graphs/colors.hpp>
#include <boost/graphs/properties.hpp>
#include <boost/graphs/algorithms/utility.hpp>

// Things to consider:
// A predicate for visiting vertices. Can only visit if white and pred(v).
// A predicate for stopping, invoked 

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

namespace detail
{
    // Adapters for getting the current element of a buffer.
    template <typename T, typename Seq>
    inline T& peek(std::stack<T, Seq>& s)
    { return s.top(); }

    template <typename T, typename Seq>
    inline T& peek(std::queue<T, Seq>& s)
    { return s.front(); }
}

template <typename Graph, typename Vertex, typename Edge, typename Visitor, typename ColorMap, typename Buffer>
void
search_edge(Graph const& g, Vertex u, Edge e, Visitor vis, ColorMap color, Buffer& buf)
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;

    // Examine the edge before checking it.
    vis.examine_edge(g, e);

    // Look at the color of the target vertex and decide what to do with it.
    Vertex v = e.target();
    typename ColorMap::value_type c = color(v);
    if(c == ColorTraits::white()) {
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

template <typename Graph, typename Vertex, typename Visitor, typename ColorMap, typename Buffer>
void
search_vertex(Graph const& g, Vertex u, Visitor vis, ColorMap color, Buffer& buf)
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
        search_edge(g, u, e, vis, color, buf);
    }
    color(u, ColorTraits::black());
    vis.finish_vertex(g, u);
}

template <typename Graph, typename Visitor, typename ColorMap, typename Buffer>
void
search_vertices(Graph const& g, Visitor vis, ColorMap color, Buffer& buf)
{
    typedef typename Graph::vertex_descriptor Vertex;

    // Continue the search until we're done with the buf.
    while(!buf.empty()) {
        Vertex u = detail::peek(buf);
        buf.pop();
        search_vertex(g, u, vis, color, buf);
    }
}

// @requires SameType<Vertex, Graph::vertex_descriptor>
template <typename Graph, typename Vertex, typename Visitor, typename ColorMap, typename Buffer>
void
search_from_vertex(Graph const& g, Vertex start, Visitor vis, ColorMap color, Buffer& buf)
{
    typedef color_traits<typename ColorMap::value_type> ColorTraits;

    // Initialize the start vertex and put it in the buf.
    color(start, ColorTraits::gray());
    buf.push(start);
    vis.start_vertex(g, start);
    vis.discover_vertex(g, start);
    search_vertices(g, vis, color, buf);
}

// @requires InputIterator Iter
// @requires SameType<Dereferenceable<Iter>::reference, Graph::vertex_descriptor>
template <typename Graph, typename Iter, typename Visitor, typename ColorMap, typename Buffer>
void
search_from_vertices(Graph const& g, Iter f, Iter l, Visitor vis, ColorMap color, Buffer buf)
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
    search_vertices(g, vis, color, buf);
}

template <typename Graph, typename Visitor, typename Buffer, typename ColorMap>
void
search_graph(Graph const& g, Visitor vis, ColorMap color, Buffer& buf)
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
            search_from_vertex(g, v, vis, color, buf);
        }
    }
}

#endif
