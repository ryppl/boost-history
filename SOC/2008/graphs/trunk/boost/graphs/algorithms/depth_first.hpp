
#ifndef ALGORITHMS_DEPTH_FIRST_HPP
#define ALGORITHMS_DEPTH_FIRST_HPP

#include <boost/graphs/properties.hpp>
#include <boost/graphs/colors.hpp>
#include <boost/graphs/directional_edge.hpp>

#include <boost/graphs/algorithms/search.hpp>

/**
 * Execute a breadth-first search over the graph.
 */
template <
    typename Graph,
    typename Visitor = default_search_visitor,
    typename ColorMap = optional_vertex_label<Graph, color>>
void
depth_first_search(Graph const& g,
                   Visitor vis = Visitor(),
                   ColorMap color = ColorMap())
{
    typedef typename Graph::vertex_descriptor Vertex;
    typedef std::stack<Vertex> Stack;
    typedef color_traits<typename ColorMap::value_type> ColorTraits;

    Stack s;
    initialize(g, color, ColorTraits::white());
    search_graph(g, color, s, vis);
}

#endif
