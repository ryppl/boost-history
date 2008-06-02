
#include <iostream>
#include <string>
#include <vector>

#include <boost/graphs/adjacency_list.hpp>

#include "demangle.hpp"
#include "add_vertices.hpp"
#include "add_edges.hpp"

using namespace std;
using namespace boost::graphs;
using namespace boost::graphs::adj_list;

// Instantiate a number of graph types. This are what I would think that
// some of the more common types are.
//
// type_vertex_edge_incident

void undirected_vector_vector_vector();
void undirected_list_list_list();
void undirected_set_set_set();
void undirected_map_set_set();

static const int V = 100;
static const int E = 100;


int
main(int argc, char *argv[])
{
    undirected_vector_vector_vector();
    undirected_list_list_list();
    undirected_set_set_set();
    undirected_map_set_set();

    return 0;
}

// Best used for fast graph construction. Note that you can't remove elements
// from this graph, and it allows duplicate vertices and edges.
void undirected_vector_vector_vector()
{
    typedef adjacency_list<
            undirected, int, int, vertex_vector, edge_vector, vertex_edge_vector
        > Graph;
    Graph g;
    add_vertices(g, V);
    add_edges(g, E);

    // cout << demangle(typeid(Graph::this_type).name()) << endl;
}

// Best used for fast graph construction. This graph supports vertex and edge
// removal, but the edge removal is slower than if you use an incidence set.
// Also, this allows duplicate vertices and edges.
void undirected_list_list_list()
{
    typedef adjacency_list<
            undirected, int, int, vertex_list, edge_list, vertex_edge_list
        > Graph;
    Graph g;
    add_vertices(g, V);
    add_edges(g, E);
}

// This is probably the best all-around implementation of simple graphs
// if vertices can be uniquely identified. Construction is relatively
// slow, but the graph is fully mutable at "optimal" complexity. At least
// its mutation operations are faster than non-sorted/hashed containers.
void undirected_set_set_set()
{
    typedef adjacency_list<
            undirected, int, int, vertex_set, edge_set, vertex_edge_set
        > Graph;
    Graph g;
    add_vertices(g, V);
}

// Pretty much the same as above, but vertices are mapped to a key.
//
// The map here is basically int-to-int, and kind of worthless, but we're just
// using this to make sure that the class instantiates correctly.
void undirected_map_set_set()
{
    typedef adjacency_list<
            undirected, int, int, int_to_vertex_map, edge_set, vertex_edge_set
        > Graph;
    Graph g;
    add_mapped_vertices(g, V);
}

