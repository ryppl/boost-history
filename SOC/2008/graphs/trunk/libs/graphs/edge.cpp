
#include <iostream>

#include <boost/graphs/directed_graph.hpp>
#include <boost/graphs/undirected_graph.hpp>

#include "square.hpp"

using namespace std;

typedef vertex_vector<> vvec;
typedef edge_vector<> evec;


void directed();
void undirected();

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
}

void undirected()
{
    typedef undirected_graph<int, int, vvec, evec> Graph;
    Graph g;
    make_square(g);

    Graph::edge_range rng = g.edges();
    for(Graph::edge_iterator i = rng.first ; i != rng.second; ++i) {
        cout << *i << endl;
    }
}
