/**
 * Testing a function that has a range
 *
 * Functions used in test:
 *   vertices(g)
 */

#include <iostream>
#include <boost/function.hpp>
#include <functional>
#include <vector>
#include "function_graph.hpp"
#include <boost/range.hpp>
#include <cassert>

// print an edge
template<typename Result, typename Vertex>
void print_edge(boost::detail::func_graph_edge<Result, Vertex> const& e)
{
    std::cout << "\nEdge:\n" << "  Source: " << e.source_ << "\n";
    std::cout << "  Target: " << e.target_ << "\n";
}

int main()
{
    ////////
    // Data for graph
    long arr[8] = {56233, 572, -7851, 956242, -2, 893437, 63, 4474443};
    std::vector<long> numbers(arr, arr + 8);

    ////////
    // Typedefs
    typedef std::pair<
                std::vector<long>::iterator,
                std::vector<long>::iterator
            > iterator_range;
    typedef boost::function<bool(long, long)> boolean_func;
    typedef boost::function<long(long, long)> weighted_func;
    typedef boost::function_graph<boolean_func, iterator_range> boolean_graph;
    typedef boost::function_graph<weighted_func, iterator_range> weighted_graph;
    typedef std::pair<
                boolean_graph::in_edge_iterator,
                boolean_graph::in_edge_iterator
            > in_edge_range;

    ////////
    // Create function graphs
    boolean_graph booleanGraph(std::less<long>(),
                               std::make_pair(numbers.begin(), numbers.end()));
    weighted_graph weightedGraph(std::minus<long>(),
                                std::make_pair(numbers.begin(), numbers.end()));

    ////////
    // Check graph boolean operators
    assert(booleanGraph == booleanGraph);
    assert(weightedGraph == weightedGraph);

    ////////
    // Check vertices(g)
    iterator_range correctRange = std::make_pair(numbers.begin(),
                                                    numbers.end());
    assert(vertices(booleanGraph) == correctRange);
    assert(vertices(weightedGraph) == correctRange);

    ////////
    // Check num_vertices
    std::vector<long>::size_type numVertices = numbers.size();
    assert(num_vertices(booleanGraph) == numVertices);
    assert(num_vertices(weightedGraph) == numVertices);

    ////////
    // Check in edges
    std::vector<long>::iterator aVertex = ++numbers.begin(); // aVector = 572
    in_edge_range in_edges_bool = in_edges(*aVertex, booleanGraph);
    // print all in_edges
    while(in_edges_bool.first != in_edges_bool.second)
    {
        ++in_edges_bool.first;
    }
    //iterator_range in_edges_wght = in_edges(*aVertex, weightedGraph);
    
    std::cerr << "\nCompiled - What? It worked?\n\n";

    return 0;
}
