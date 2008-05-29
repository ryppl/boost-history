
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_GRAPHS_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_GRAPHS_HPP

// Common graph types provide cleaner interfaces to the basic adjacency list
// type. This is done by providing a number of default template arguments and
// rearranging the parameters to better suit the graph.

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * A simple graph is undirected, does not allow parallel edges or loops and
 * has anonymous vertices.
 *
 * Changing the vertex store to a set will allow vertices to be inserted
 * according to uniqueness, but will slow down the construction of the graph.
 * However, searches for specific will be significantly faster.
 *
 * The default preference tends towards better mutability. Changing the vertex
 * store from a list to a vertex will yield better performance, but vertices
 * cannot be removed.
 */
template <
    typename VertexProps = none,
    typename EdgeProps = none,
    template <typename> class VertexStore = vertex_list,
    template <typename> class EdgeStore = edge_set,
    template <typename> class VertexEdgeStore = vertex_edge_list
>
struct graph
    : adjacency_list<
        undirected,
        VertexProps,
        EdgeProps,
        VertexStore,
        EdgeStore,
        VertexEdgeStore,
        no_loops_policy>
{ };

/**
 * A multigraph is undirected, allows parallel edges and loops, and has
 * anonymous vertices.
 *
 * Changing the vertex store to a set will allow vertices to be inserted
 * according to uniqueness, but will slow down the construction of the graph.
 * However, searches for specific will be significantly faster.
 *
 * The default preference tends towards better mutability. Changing the vertex
 * and edge stores will yield better performance, but removals will not be
 * allowed.
 */
template <
    typename VertexProps = none,
    typename EdgeProps = none,
    template <typename> class VertexStore = vertex_list,
    template <typename> class EdgeStore = edge_list,
    template <typename> class VertexEdgeStore = vertex_edge_list
>
struct multigraph
    : adjacency_list<
        undirected,
        VertexProps,
        EdgeProps,
        VertexStore,
        EdgeStore,
        VertexEdgeStore,
        allow_loops_policy>
{ };


} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
