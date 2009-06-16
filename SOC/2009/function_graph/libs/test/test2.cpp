/**
 * Testing a function that has a range
 */

#include <iostream>
#include <boost/function.hpp>
#include <functional>
#include <algorithm>
#include <vector>
//#include <utility>
#include "function_graph_alt.hpp"
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
    typedef boost::function_graph<function_boolean> graph_boolean;

    ////////
    // Create functions, graphs and edges
    graph_boolean funcGraph_boolean(std::less<int>());

    typedef std::pair<std::vector<unsigned int>::iterator,
                      std::vector<unsigned int>::iterator> iter_pair;
    
    
    boost::iterator_range<std::vector<unsigned int>::iterator>
        rangeIter(vectorOfInts.begin(), vectorOfInts.end());
    boost::iterator_range<std::vector<unsigned int>::iterator>
        rangeIter2(vectorOfInts);
    boost::iterator_range<std::vector<unsigned int>::iterator>
        rangeIter3(std::make_pair(vectorOfInts.begin(), vectorOfInts.end()));
    
    return 0;
}
