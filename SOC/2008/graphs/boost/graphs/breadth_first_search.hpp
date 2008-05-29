
#ifndef BOOST_GRAPHS_BREADTH_FIRST_SEARCH_HPP
#define BOOST_GRAPHS_BREADTH_FIRST_SEARCH_HPP

#include <boost/graphs/properties.hpp>

namespace boost {
namespace graphs {

template <typename Graph>
void breadth_first_search(const Graph& g,
                          typename Graph::vertex_descriptor start)
{

}

/**
 * Run a BFS on the graph starting at the given start vertex and stopping when
 * then end vertex is found (or visiting all vertices if never found).
 */
template <typename Graph>
void breadth_first_search(const Graph& g,
                          typename Graph::vertex_descriptor start,
                          typename Graph::vertex_descriptor end)
{
}

/**
 * Run a BFS on the graph starting at an arbitrary vertex (actually the first
 * vertex).
 */
template <typename Graph>
void breadth_first_search(const Graph& g)
{
    breadth_first_search(g, *g.begin_vertices());
}

} /* namespace graphs */
} /* namespace boost */


#endif
