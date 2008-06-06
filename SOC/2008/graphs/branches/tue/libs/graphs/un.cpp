
#include <iostream>
#include <set>

#include <boost/graphs/undirected_graph.hpp>

#include "demangle.hpp"

using namespace std;

struct City { };
struct Road { };

void list_list();
void vec_vec();

template <typename Graph>
void print_types(const Graph&)
{
    cout << demangle(typeid(typename Graph::property_descriptor).name()) << endl;
    cout << demangle(typeid(typename Graph::vertex_descriptor).name()) << endl;
    cout << demangle(typeid(typename Graph::edge_descriptor).name()) << endl;
    cout << demangle(typeid(typename Graph::property_store).name()) << endl;
    cout << demangle(typeid(typename Graph::vertex_store).name()) << endl;
    cout << demangle(typeid(typename Graph::incidence_store).name()) << endl;
}

template <typename Graph>
void add_verts(Graph& g)
{
    for(int i = 0; i < 10; ++i) {
        g.add_vertex();
    }
    cout << "num_verts: " << g.num_vertices() << std::endl;
}

template <typename Graph>
void add_edges(Graph& g)
{
    typename Graph::vertex_descriptor u = g.add_vertex();
    typename Graph::vertex_descriptor v = g.add_vertex();
    g.add_edge(u, v);
}

int main()
{
    list_list();
    cout << endl << endl;
    vec_vec();
    cout << endl << endl;

    return 0;
}

void list_list()
{
    typedef undirected_graph<City, Road, vertex_list, edge_list> Graph;

    Graph g;
    cout << demangle(typeid(Graph).name()) << endl;
    add_verts(g);
}

void vec_vec()
{
    typedef undirected_graph<City, Road, vertex_vector, edge_vector> Graph;

    Graph g;
    cout << demangle(typeid(Graph).name()) << endl;
    add_verts(g);
    add_edges(g);
}

