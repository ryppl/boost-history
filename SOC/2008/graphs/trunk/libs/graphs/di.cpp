
#include <iostream>

#include <boost/graphs/directed_graph.hpp>

using namespace std;
using namespace boost;
namespace d = directed;

int main()
{
    typedef directed_graph<int, int, vertex_vector, d::edge_vector> Graph;
    Graph g;

    return 0;
}
