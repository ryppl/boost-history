
#ifndef ALGORITHMS_BREADTH_FIRST_HPP
#define ALGORITHMS_BREADTH_FIRST_HPP

#include <boost/graphs/properties.hpp>
#include <boost/graphs/colors.hpp>
#include <boost/graphs/directional_edge.hpp>

#include <boost/graphs/algorithms/utility.hpp>
#include <boost/graphs/algorithms/iterator.hpp>

#include <boost/graphs/algorithms/search.hpp>

// A BFS search visitor is a basically just a search visitor.
struct bfs_visitor : search_visitor { };


template <typename Graph, typename Visitor, typename ColorMap = optional_vertex_map<Graph, color>>
void
breadth_first_search(Graph const& g, Visitor vis, ColorMap color = ColorMap())
{
    std::queue<typename Graph::vertex_descriptor> buf;
    detail::optional_prop_init(g, color, color_traits<typename ColorMap::value_type>::white());
    search_graph(g, vis, color, buf);
}

#endif
