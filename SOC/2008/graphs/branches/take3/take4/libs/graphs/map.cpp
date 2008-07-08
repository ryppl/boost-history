
#include <iostream>

#include <boost/graphs/undirected_graph.hpp>

using namespace std;
using namespace boost;

int main()
{
    typedef undirected_graph<int, none, vertex_map<string>, edge_set<> > Graph;
    Graph g;
    g.add_vertex("Jan", 1);
    g.add_vertex("Feb", 2);
    g.add_vertex("Mar", 3);
    g.add_vertex("Apr", 4);
    g.add_vertex("May", 5);

    g.remove_vertex("Mar");

    cout << g[g.find_vertex("Apr")] << endl;
}
