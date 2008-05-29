
#include <iostream>
#include <string>
#include <vector>

#include <boost/graphs/adjacency_list.hpp>

#include "add_vertices.hpp"

using namespace std;
using namespace boost::graphs;
using namespace boost::graphs::adj_list;

// The prurpose of this file is to instantiate every possible combination
// of structural components for the adjacency list. There are a quite a few.
// Testing functions are named for the storage components that define each
// part of the adjacency list. They are:
//
// type_vertex_edge_incident
//
// Where type is the predominant structural type of the graph, vertex is the
// vertex store, edge is the edge store and incidedent is the vertex edge store.

// Anonymous vertices, multigraph, not reducible.
void undirected_vector_vector_vector();
void undirected_vector_vector_list();
void undirected_vector_vector_set();

// Anonymous vertices, multigraph, fully mutable
void undirected_vector_list_vector();
void undirected_vector_list_list();
void undirected_vector_list_set();

// Anonymous vertices, simple graph, fully mutable
void undirected_vector_set_vector();
void undirected_vector_set_list();
void undirected_vector_set_set();

// Anonymous vertices, simple graph, fully mutable
// Not implemented
void undirected_vector_multiset_vector();
void undirected_vector_multiset_list();
void undirected_vector_multiset_set();

static const int N = 100;

int
main(int argc, char *argv[])
{
    undirected_vector_vector_vector();
    undirected_vector_vector_list();
    undirected_vector_vector_set();

    undirected_vector_list_vector();
    undirected_vector_list_list();
    undirected_vector_list_set();

    return 0;
}

void undirected_vector_vector_vector()
{
    typedef adjacency_list<
            undirected, int, int, vertex_vector, edge_vector, vertex_edge_vector
        > Graph;
    Graph g;
    add_vertices(g, N);
}

void undirected_vector_vector_list()
{
    typedef adjacency_list<
            undirected, int, int, vertex_vector, edge_vector, vertex_edge_list
        > Graph;
    Graph g;
    add_vertices(g, N);
}

void undirected_vector_vector_set()
{
    typedef adjacency_list<
            undirected, int, int, vertex_vector, edge_vector, vertex_edge_set
        > Graph;
    Graph g;
    add_vertices(g, N);
}

void undirected_vector_list_vector()
{
    typedef adjacency_list<
            undirected, int, int, vertex_vector, edge_list, vertex_edge_vector
        > Graph;
    Graph g;
    add_vertices(g, N);
}

void undirected_vector_list_list()
{
    typedef adjacency_list<
            undirected, int, int, vertex_vector, edge_list, vertex_edge_vector
        > Graph;
    Graph g;
    add_vertices(g, N);
}

void undirected_vector_list_set()
{
    typedef adjacency_list<
            undirected, int, int, vertex_vector, edge_list, vertex_edge_vector
        > Graph;
    Graph g;
    add_vertices(g, N);
}

void undirected_vector_set_vector();
void undirected_vector_set_list();
void undirected_vector_set_set();

void undirected_vector_multiset_vector();
void undirected_vector_multiset_list();
void undirected_vector_multiset_set();
