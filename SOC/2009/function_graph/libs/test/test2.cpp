/**
 * Testing a function that has a range
 */

#include <iostream>
#include <boost/function.hpp>
#include <functional>
#include <algorithm>
#include <vector>
//#include <utility>
#include "function_graph.hpp"
#include <cmath>
#include <boost/range.hpp>

////////
// Boolean function
template <typename T>
struct less_than {
    bool operator() (T a, T b) { return a < b; }
};

int main()
{
    unsigned int tempArray[5] = {1, 4, 9, 2, 7};
    std::vector<unsigned int> vectorOfInts(tempArray, tempArray + 5);

    ////////
    // Create typedefs for functions and function graphs
    typedef boost::function<bool(int,int)> function_boolean;
    typedef std::pair<std::vector<unsigned int>::iterator,
                      std::vector<unsigned int>::iterator> iterator_range;
    typedef boost::function_graph<function_boolean> G;
    typedef boost::function_graph<function_boolean, iterator_range> GwithRange;

    ////////
    // Create functions, graphs and edges
    //G funcGraph = boost::make_function_graph(less_than<int>());
    G funcGraph(less_than<int>());
    GwithRange funcGraphWithRange(less_than<int>(),
                                  std::make_pair(vectorOfInts.begin(),
                                                 vectorOfInts.end()));

    return 0;
}
