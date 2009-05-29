#include <iostream>
#include <boost/function.hpp>
#include <functional>
#include <algorithm>
#include <vector>
#include "function_graph.hpp"

struct int_less_than {
    bool operator()(int a, int b) { return a < b; }
}; 

int main()
{
    boost::function2<bool,int,int> h = int_less_than();
    boost::function2<bool,int,int> g = int_less_than();
    boost::graph::function_graph<bool, int> implicitG1(h);
    implicitG1.set_edge_function(g);
    implicitG1.edge(1,2);
    
    
    return 0;
}
