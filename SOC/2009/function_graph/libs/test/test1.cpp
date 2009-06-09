/**
 * Testing a function that returns a boolean
 */

#include <iostream>
#include <boost/function.hpp>
#include <functional>
#include <algorithm>
#include <vector>
#include <utility>
#include "function_graph.hpp"
#include <cassert>

template <typename T>
struct less_than {
    bool operator() (T a, T b) { return a < b; }
};

int main()
{
    ////////
    // Create a boost function and function graph.
    typedef boost::function<bool(int,int)> function_type;
    typedef boost::function_graph<function_type> graph;
    typedef graph::edge_descriptor edge_descriptor;
    function_type f = less_than<int>();
    graph funcGraph(f);
    graph::vertex_descriptor x = 1;
    graph::vertex_descriptor y = 2;

    ////////
    // Assert an edge taken from 
    // Note that edge().first is the edge object
    graph::edge_descriptor e = edge(x, y, funcGraph).first;
    assert(source(e,funcGraph) == x);
    assert(target(e,funcGraph) == y);

    ////////
    // Check the adjacency matrix edge
    /*std::pair<edge_descriptor, bool> edge_pair = boost::edge(1, 2, funcGraph);
    std::cout << edge_pair.first << "\n";*/

    
    
    return 0;
}
