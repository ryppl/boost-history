/**
 * This file is intended test compilation only.
 */


#include <iostream>
#include <boost/function.hpp>
#include <functional>
#include <algorithm>
#include <vector>
#include "function_graph.hpp"

// test boost function
struct int_less_than {
    bool operator()(int a, int b) { return a < b; }
}; 

int main()
{
    // create two boost functions
    boost::function2<bool,int,int> h = int_less_than();
    boost::function2<bool,int,int> g = int_less_than();

    // test constructor
    boost::graph::function_graph<bool, int> implicitG1(h);

    // set the edge function
    implicitG1.set_edge_function(g);

    // test the edge function
    implicitG1.edge(1,2);
    
    
    return 0;
}
