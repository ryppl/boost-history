
#include <iostream>

#include <boost/graphs/adjacency_list.hpp>

using namespace std;
using namespace boost;
using namespace boost::graphs;
using namespace boost::graphs::adj_list;

int main()
{
    typedef undirected_graph<int, int> Graph;
    Graph g;

    // Add a bunch of vertices.
    for(int i = 0; i < 5; ++i) {
        g.add_vertex(i);
    }

    // Connect them to create a star graph with 0 at the center.
    g.add_edge(g.find_vertex(0), g.find_vertex(1), 10);
    g.add_edge(g.find_vertex(0), g.find_vertex(2), 11);
    g.add_edge(g.find_vertex(0), g.find_vertex(3), 12);
    g.add_edge(g.find_vertex(0), g.find_vertex(4), 13);

    cout << "verts: " << g.num_vertices() << endl;
    cout << "edges: " << g.num_edges() << endl;

    Graph::incidence_range ir = g.incident_edges(g.find_vertex(0));
    for( ; ir.first != ir.second; ++ir.first) {
        cout << "inc test: " << g.properties(*ir.first) << endl;
    }

    Graph::adjacency_range ar = g.adjacent_vertices(g.find_vertex(0));
    for( ; ar.first != ar.second; ++ar.first) {
        cout << "adj test: " << g.properties(*ar.first) << endl;
    }

    return 0;
}
