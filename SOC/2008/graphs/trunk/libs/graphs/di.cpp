
#include <iostream>

#include <boost/graphs/directed_graph.hpp>

#include "test.hpp"

using namespace std;
using namespace boost;


int main()
{
    typedef directed_graph<int, int, vertex_vector, edge_vector> Graph;
    typedef Graph::vertex_descriptor Vertex;
    typedef Graph::edge_descriptor Edge;

    Graph g;
    Vertex u = g.add_vertex(1);
    Vertex v = g.add_vertex(2);
    Vertex w = g.add_vertex(3);

    Edge e1 = g.add_edge(u, v, 1);
    Edge e2 = g.add_edge(w, v, 2);

    cout << "d_out(u): " << g.out_degree(u) << endl;
    cout << "d_in(u): " << g.in_degree(u) << endl;
    cout << "d(u): " << g.degree(u) << endl;

    cout << "d_out(v): " << g.out_degree(v) << endl;
    cout << "d_in(v): " << g.in_degree(v) << endl;
    cout << "d(v): " << g.degree(v) << endl;

    return 0;
}
