
#ifndef BOOST_GRAPHS_ALGORITHMS_SEARCH_VISITOR_HPP
#define BOOST_GRAPHS_ALGORITHMS_SEARCH_VISITOR_HPP


/**
 * The default search visitor provdes a number of "hooks" that can be overridden
 * to implement a number of various algorithms. Visitor functions can basically
 * be divided into two types:
 */
struct default_search_visitor
{
    /**
     * Called when a search originates from a vertex. This is called once for
     * the root of each search tree.
     */
    template <typename Graph, typename Vertex>
    inline void start_vertex(Graph const& g, Vertex v)
    { }

    /**
     * Called when the vertex is popped from the stack. Not all examined
     * vertices are discovered (e.g., a start vertex). Use this event to
     * record the depth-first ordering of vertices.
     */
    template <typename Graph, typename Vertex>
    inline void examine_vertex(Graph const& g, Vertex v)
    { }

    /**
     * Called when a vertex is encountered for the first time. Discovered
     * vertices are examined unless the algorithm terminates before they are
     * popped from the stack.
     */
    template <typename Graph, typename Vertex>
    inline void discover_vertex(Graph const& g, Vertex v)
    { }

    /**
     * Called when all of the incident edges of the vertex have been examined.
     */
    template <typename Graph, typename Vertex>
    inline void finish_vertex(Graph const& g, Vertex v)
    { }

    /**
     * Called for every incident edge of a vertex being examined. Examined
     * edges are classified by the color of their targets at the time of
     * investigation.
     */
    template <typename Graph, typename Edge>
    inline void examine_edge(Graph const& g, Edge e)
    { }

    /**
     * Called for each examined edge that becomes part of the search tree. A
     * tree edge is one whose source is being examined and whose target is
     * discovered.
     */
    template <typename Graph, typename Edge>
    inline void tree_edge(Graph const& g, Edge e)
    { }

    /**
     * Called for eacch edge that is not a tree edge (i.e., whose target has
     * already been discovered). Specific types of non-tree edges can be
     * classified as back or cross edges depending on the color of the target.
     */
    template <typename Graph, typename Edge>
    inline void non_tree_edge(Graph const& g, Edge e)
    { }

    /**
     * Called for each examined edge whose target is in the search buffer. Back
     * edges, cross edges, and forward edges are nontree edges.
     */
    template <typename Graph, typename Edge>
    inline void back_edge(Graph const& g, Edge e)
    { }

    /**
     * Called for each examined edge whose target has already been removed from
     * the search buffer. Technically, this is either a cross edge or a forward
     * edge, but there's no way to easily distinguish them.
     */
    template <typename Graph, typename Edge>
    inline void cross_edge(Graph const& g, Edge e)
    { }
};


/**
 * The search visitor provides a framework for housing an aggregation of other
 * search visitors. This enables users to construct compound visitors on the
 * fly. Use the make_search_visitor function to aggregate visitors at the
 * call site.
 */
template <typename... Visitors> struct search_visitor;

// Defines the base case for the visitor. The base case is derived from the
// default visitor, which implements a number of empty functions.
template<> struct search_visitor<> : default_search_visitor { };

// The recursive case splits the the visitors into the head and tail and derives
// from the dispatch as a mixin. The dispatch type implements the functions
// that are responsible for the executution of the visitors.
template <typename Head, typename... Tail>
struct search_visitor<Head, Tail...> : private search_visitor<Tail...>
{
    search_visitor()
        : search_visitor<Tail...>(), vis()
    { }

    search_visitor(Head head, Tail... tail)
        : search_visitor<Tail...>(tail...), vis(head)
    { }

    // This is the basic dispatch interface. Each of these functions dispatches
    // the visit call to the head, then distributes it to all other visitors
    // in the tail.
    template <typename Graph, typename Vertex>
    inline void start_vertex(Graph const& g, Vertex v)
    { vis.start_vertex(g, v); search_visitor<Tail...>(*this).start_vertex(g, v); }

    template <typename Graph, typename Vertex>
    inline void examine_vertex(Graph const& g, Vertex v)
    { vis.examine_vertex(g, v); search_visitor<Tail...>(*this).examine_vertex(g, v); }

    template <typename Graph, typename Vertex>
    inline void discover_vertex(Graph const& g, Vertex v)
    { vis.discover_vertex(g, v); search_visitor<Tail...>(*this).discover_vertex(g, v); }

    template <typename Graph, typename Vertex>
    inline void finish_vertex(Graph const& g, Vertex v)
    { vis.finish_vertex(g, v); search_visitor<Tail...>(*this).finish_vertex(g, v); }

    template <typename Graph, typename Edge>
    inline void examine_edge(Graph const& g, Edge e)
    { vis.examine_edge(g, e); search_visitor<Tail...>(*this).examine_edge(g, e); }

    template <typename Graph, typename Edge>
    inline void tree_edge(Graph const& g, Edge e)
    { vis.tree_edge(g, e); search_visitor<Tail...>(*this).tree_edge(g, e); }

    template <typename Graph, typename Edge>
    inline void non_tree_edge(Graph const& g, Edge e)
    { vis.non_tree_edge(g, e); search_visitor<Tail...>(*this).non_tree_edge(g, e); }

    template <typename Graph, typename Edge>
    inline void back_edge(Graph const& g, Edge e)
    { vis.back_edge(g, e); search_visitor<Tail...>(*this).back_edge(g, e); }

    template <typename Graph, typename Edge>
    inline void cross_edge(Graph const& g, Edge e)
    { vis.cross_edge(g, e); search_visitor<Tail...>(*this).cross_edge(g, e); }

    Head vis;
};

// The make function....
template <typename... Visitors>
search_visitor<Visitors...>
make_search_visitor(Visitors const&... visitors)
{ return search_visitor<Visitors...>(visitors...); }


// These are a bunch of helpers that can turn function objects (free functions,
// Boost.Functions, and lambda functions?) into search visitors. This is done
// by overriding the member function call in the default visitor with a function
// object defined in the derived visitor. For the time being, it would seem that
// we need to have visitor objects for each desired overload.
//
// NOTE: Although these functions don't really give signatures, they certainly
// have to follow certian conventsion. Specifically:
//  - Vertex visiting functions must have the signature void (Graph, Vertex).
//  - Edge visiting functions must have the signature void (Graph, Edge).
//
// TODO: Extend these visitors with overloads that accept Boost.Lambda objects.

template <typename Function>
struct start_vertex_visitor : default_search_visitor
{
    start_vertex_visitor(Function f) : start_vertex(f) { }
    Function start_vertex;
};

template <typename Function>
struct examine_vertex_visitor : default_search_visitor
{
    examine_vertex_visitor(Function f) : examine_vertex(f) { }
    Function examine_vertex;
};

template <typename Function>
struct discover_vertex_visitor : default_search_visitor
{
    discover_vertex_visitor(Function f) : discover_vertex(f) { }
    Function discover_vertex;
};

template <typename Function>
struct finish_vertex_visitor : default_search_visitor
{
    finish_vertex_visitor(Function f) : finish_vertex(f) { }
    Function finish_vertex;
};

template <typename Function>
struct examine_edge_visitor : default_search_visitor
{
    examine_edge_visitor(Function f) : examine_edge(f) { }
    Function examine_edge;
};

template <typename Function>
struct tree_edge_visitor : default_search_visitor
{
    tree_edge_visitor(Function f) : tree_edge(f) { }
    Function tree_edge;
};

template <typename Function>
struct non_tree_edge_visitor : default_search_visitor
{
    non_tree_edge_visitor(Function f) : non_tree_edge(f) { }
    Function non_tree_edge;
};

template <typename Function>
struct cross_edge_visitor : default_search_visitor
{
    cross_edge_visitor(Function f) : cross_edge(f) { }
    Function cross_edge;
};

template <typename Function>
struct back_edge_visitor : default_search_visitor
{
    back_edge_visitor(Function f) : back_edge(f) { }
    Function back_edge;
};

template <typename Function>
start_vertex_visitor<Function>
on_start_vertex(Function f)
{ return start_vertex_visitor<Function>(f); }

template <typename Function>
examine_vertex_visitor<Function>
on_examine_vertex(Function f)
{ return examine_vertex_visitor<Function>(f); }

template <typename Function>
discover_vertex_visitor<Function>
on_discover_vertex(Function f)
{ return discover_vertex_visitor<Function>(f); }

template <typename Function>
finish_vertex_visitor<Function>
on_finish_vertex(Function f)
{ return finish_vertex_visitor<Function>(f); }

template <typename Function>
examine_edge_visitor<Function>
on_examine_edge(Function f)
{ return examine_edge_visitor<Function>(f); }

template <typename Function>
tree_edge_visitor<Function>
on_tree_edge(Function f)
{ return tree_edge_visitor<Function>(f); }

template <typename Function>
non_tree_edge_visitor<Function>
on_non_tree_edge(Function f)
{ return non_tree_edge_visitor<Function>(f); }

template <typename Function>
cross_edge_visitor<Function>
on_cross_edge(Function f)
{ return cross_edge_visitor<Function>(f); }

template <typename Function>
back_edge_visitor<Function>
on_back_edge(Function f)
{ return back_edge_visitor<Function>(f); }

#endif
