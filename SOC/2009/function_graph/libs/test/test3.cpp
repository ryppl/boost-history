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

namespace test3 {
// print an edge
template<typename Graph>
void print_edge(typename Graph::edge_descriptor const& e, Graph const& g)
{
    std::cout << "\nEdge:\n" << "  Source: " << e.source << "\n";
    std::cout << "  Target: " << e.target << "\n";
}
} // namespace

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
            > in_edge_bool_range;
    typedef std::pair<
                weighted_graph::in_edge_iterator,
                weighted_graph::in_edge_iterator
            > in_edge_wght_range;
    typedef std::pair<
                boolean_graph::out_edge_iterator,
                boolean_graph::out_edge_iterator
            > out_edge_bool_range;
    typedef std::pair<
                weighted_graph::out_edge_iterator,
                weighted_graph::out_edge_iterator
            > out_edge_wght_range;



    ////////
    // Concept checking



    ////////
    // Create function graphs
    boolean_graph booleanGraph(std::less<long>(),
                               std::make_pair(numbers.begin(), numbers.end()));
    weighted_graph weightedGraph(std::minus<long>(),
                                std::make_pair(numbers.begin(), numbers.end()));



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
    in_edge_bool_range in_edges_bool = boost::in_edges(*aVertex, booleanGraph);
    // Print all in_edges from booleanGraph to 572
    // In other words, print pairs of numbers that are less than 572
    while(in_edges_bool.first != in_edges_bool.second)
    {
        test3::print_edge(*in_edges_bool.first, booleanGraph);
        ++in_edges_bool.first;
    }

    in_edge_wght_range in_edges_wght = boost::in_edges(*aVertex, weightedGraph);
    // Print all in_edges from weightedGraph to 572
    // By the function, this prints in_edge - 572
    while(in_edges_wght.first != in_edges_wght.second)
    {
        std::cout << boost::source(*in_edges_wght.first, weightedGraph)
                  << " - 572 = " << (*in_edges_wght.first).result
                  << "\n";
        ++in_edges_wght.first;
    }


    ////////
    // Check out edges
    ++ ++ ++aVertex; // aVertex now holds -2
     out_edge_bool_range out_edges_bool = boost::out_edges(*aVertex, booleanGraph);
    // Print all out_edges from booleanGraph to -2
    // In other words, print pairs of numbers that are not less than -2
    while(out_edges_bool.first != out_edges_bool.second)
    {
        test3::print_edge(*out_edges_bool.first, booleanGraph);
        ++out_edges_bool.first;
    }

    out_edge_wght_range out_edges_wght = boost::out_edges(*aVertex, weightedGraph);
    // Print all in_edges from weightedGraph to 572
    // By the function, this prints in_edge - 572
    while(out_edges_wght.first != out_edges_wght.second)
    {
        std::cout << "-2 - "
                  << boost::target(*out_edges_wght.first, weightedGraph)
                  << " = "
                  << (*out_edges_wght.first).result
                  << "\n";
        ++out_edges_wght.first;
    }



    ////////
    // Check adjacency iterators

    return 0;
}
