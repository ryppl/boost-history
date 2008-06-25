
#include <iostream>

#include <boost/graphs/directed_graph.hpp>

#include "square.hpp"

using namespace std;

typedef vertex_vector<> vvec;
typedef edge_vector<> evec;

void directed();

int main()
{
    directed();

    return 0;
}

void directed()
{
    typedef directed_graph<int, int, vvec, evec> Graph;
    Graph g;
    make_square(g);

    Graph::edge_range rng = g.edges();
    for(Graph::edge_iterator i = rng.first; i != rng.second; ++i) {
        cout << *i << endl;
    }


    /*
    // How can we iterate over the edges of a directed graph.. Iterate over
    // the out edges of each vertex.
    Graph::vertex_range v = g.vertices();
    for(Graph::vertex_iterator i = v.first; i != v.second; ++i) {
        Graph::out_edge_range o = g.out_edges(*i);
        for(Graph::out_edge_iterator j = o.first; j != o.second; ++j) {
            cout << g[*i] << " " << g[*j] << endl;
        }
    }
    */
}
