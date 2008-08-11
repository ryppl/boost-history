
#include <iostream>

#include <boost/graphs/adjacency_list/directed_graph.hpp>
#include <boost/graphs/adjacency_list/undirected_graph.hpp>

#include "demangle.hpp"
#include "square.hpp"

using namespace std;

typedef vertex_vector<> vvec;
typedef edge_vector<> evec;


void directed();
void undirected();

int main()
{
    directed();
    undirected();

    return 0;
}

void directed()
{
    cout << "--- directed (vec/vec) ---" << endl;
    typedef directed_graph<int, int, vvec, evec> Graph;
    Graph g;
    make_square(g);

    Graph::edge_range rng = g.edges();
    for(Graph::edge_iterator i = rng.first; i != rng.second; ++i) {
        cout << *i << endl;
    }
}

void undirected()
{
    cout << "--- undirected (vec/vec) ---" << endl;
    typedef undirected_graph<int, int, vvec, evec> Graph;
    Graph g;
    make_square(g);

    Graph::edge_range rng = g.edges();
    for(Graph::edge_iterator i = rng.first ; i != rng.second; ++i) {
        cout << *i << endl;
    }
}
