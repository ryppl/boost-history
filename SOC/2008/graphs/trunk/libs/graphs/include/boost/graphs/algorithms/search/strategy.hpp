
#ifndef BOOST_GRAPHS_ALGORITHMS_SEARCH_STRATEGY_HPP
#define BOOST_GRAPHS_ALGORITHMS_SEARCH_STRATEGY_HPP

/**
 * The default search strategy is the control counterpoint of the search
 * visitor. Specifically, it provides a number of functions that can be
 * implemented to control various aspects of the search. Note that strategies
 * work just like visitors - they are copied into eqch subsequent call.
 */
struct default_search_strategy
{
    /**
     * This control point is queried at the top of the search loop when the
     * search buffer is not empty. Returning false will cause the search to
     * end prematurely. This control point should not be called when the buffer
     * is empty.
     */
    template <typename Graph>
    inline bool continue_vertices(Graph const& g)
    { return true; }

    /**
     * This control point is queried for each incident of the the vertex v.
     * The vertex here is the one that was most recently examined. Returning
     * true will allow the search of edges to continue, returning false will
     * cause the edge loop to end prematurely.
     */
    template <typename Graph, typename Vertex>
    inline bool continue_edges(Graph const& g, Vertex v)
    { return true; }

    /**
     * This control point is when when a previousl undiscovered (white) vertex
     * is found for the first time. Returning true allows the discovery of the
     * vertex, returning false will prevent the search from proceeding along
     * this edge.
     */
    template <typename Graph, typename Vertex>
    inline bool allow_discovery(Graph const& g, Vertex v)
    { return true; }
};

#endif
