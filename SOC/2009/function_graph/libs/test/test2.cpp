#include <iostream>
#include <boost/function.hpp>
#include <functional>
#include <algorithm>
#include <vector>
#include "function_graph.hpp"

template <typename T>
struct less_than {
    bool operator()(T a, T b) { return a < b; }
}; 

int main()
{
    ////////
    // Create a boost function and function graph.
    typedef boost::function<bool(int,int)> function_type;
    typedef boost::graph::function_graph<function_type> graph;
    function_type f = less_than<int>();
    function_type g = less_than<int>();
    graph funcGraph(f);
    
    ////////
    // Set a new function to the graph.
    funcGraph.set_function(g);
    
    ////////
    // Check the edge output.
    std::cout << "2 < 1 check ";
    if(funcGraph.edge(2,1)) std::cout << "passes." << "\n";
    else std::cout << "fails." << "\n";
    
    return 0;
}
