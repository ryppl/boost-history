
#ifndef ALGORITHMS_BREADTH_FIRST_HPP
#define ALGORITHMS_BREADTH_FIRST_HPP

#include <boost/graphs/properties.hpp>
#include <boost/graphs/colors.hpp>
#include <boost/graphs/directional_edge.hpp>

#include <boost/graphs/algorithms/utility.hpp>
#include <boost/graphs/algorithms/iterator.hpp>

struct bfs_visitor
{
    template <typename Graph, typename Vertex>
    inline void discover_vertex(Graph const& g, Vertex v)
    { }

    template <typename Graph, typename Vertex>
    inline void finish_vertex(Graph const& g, Vertex v)
    { }

    template <typename Graph, typename Vertex>
    inline void gray_target(Graph const& g, Vertex v)
    { }

    template <typename Graph, typename Vertex>
    inline void black_target(Graph const& g, Vertex v)
    { }

    template <typename Graph, typename Edge>
    inline void tree_edge(Graph const& g, Edge e)
    { }

    template <typename Graph, typename Edge>
    inline void non_tree_edge(Graph const& g, Edge e)
    { }

};

#include <boost/graphs/algorithms/breadth_first/algorithm.hpp>
#include <boost/graphs/algorithms/breadth_first/vertex_walk.hpp>
#include <boost/graphs/algorithms/breadth_first/edge_walk.hpp>

#endif
