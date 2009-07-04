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
    // Typedefs for the graph and iterator range
    typedef boost::function<bool(int,int)> boolean_function;
    typedef std::pair<std::vector<unsigned int>::iterator,
                      std::vector<unsigned int>::iterator> iterator_range;
    typedef boost::function_graph<boolean_function, iterator_range> FuncGraph;

    ////////
    // Create vertices and function_graph
    unsigned int tempArray[5] = {5, 4, 9, 2, 7};
    std::vector<unsigned int> vectorOfInts(tempArray, tempArray + 5);
    FuncGraph graph(std::less<int>(),
                std::make_pair(vectorOfInts.begin(), vectorOfInts.end()));
    ////////
    // Create iterators
    iterator_range graphDataRange = vertices(graph);
    FuncGraph::in_edge_iterator graphIterator(std::less<int>(),
                                              ++vectorOfInts.begin(),
                                              vectorOfInts.begin(),
                                              vectorOfInts.end());

    ////////
    // Check iteration
    print_edge(*graphIterator);
    ++graphIterator;
    print_edge(*graphIterator);
    ++graphIterator;
    print_edge(*graphIterator);

    return 0;
}
