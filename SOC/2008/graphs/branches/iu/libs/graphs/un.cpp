
#include <iostream>
#include <set>

#include <boost/utility.hpp>
#include <boost/graphs/undirected_graph.hpp>

#include "demangle.hpp"

using namespace std;
using namespace boost;

typedef int City;
struct Road { };

void list_list();
void vec_vec();
void set_set();

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
    cout << "*** Add Vertices" << endl;
    cout << "*** " << demangle(typeid(Graph).name()) << endl;
    for(int i = 0; i < 5; ++i) {
        g.add_vertex(i);
    }
}

template <typename Graph>
void del_verts(Graph& g)
{
    // Just remove the first two vertices
    typename Graph::vertex_descriptor u = *g.begin_vertices();
    typename Graph::vertex_descriptor v = *next(g.begin_vertices());
    g.remove_vertex(u);
    g.remove_vertex(v);
}

template <typename Graph>
void add_edges(Graph& g)
{
    cout << "*** Add Edges" << endl;
    cout << "*** " << demangle(typeid(Graph).name()) << endl;
    typename Graph::vertex_descriptor u = g.add_vertex(100);
    typename Graph::vertex_descriptor v = g.add_vertex(101);
    typename Graph::vertex_descriptor w = g.add_vertex(102);
    g.add_edge(v, u);
    g.add_edge(w, u);
}

template <typename Graph>
void add_and_del_edges(Graph& g)
{
    cout << "*** Add/Delete Edges" << endl;
    cout << "*** " << demangle(typeid(Graph).name()) << endl;
    typename Graph::vertex_descriptor u = g.add_vertex(100);
    typename Graph::vertex_descriptor v = g.add_vertex(101);
    typename Graph::vertex_descriptor w = g.add_vertex(102);
    typename Graph::edge_descriptor e1 = g.add_edge(v, u);
    typename Graph::edge_descriptor e2 = g.add_edge(w, u);

    g.remove_edge(e1);
    g.remove_edge(e2);
}

template <typename Graph>
void test_multi_edge(Graph& g)
{
    cout << "*** Add/Delete Multi-Edges" << endl;
    cout << "*** " << demangle(typeid(Graph).name()) << endl;
    typename Graph::vertex_descriptor u = g.add_vertex(200);
    typename Graph::vertex_descriptor v = g.add_vertex(201);
    g.add_edge(u, v);
    g.add_edge(v, u);
    g.add_edge(u, v);
    g.add_edge(v, u);
    g.remove_edges(u, v);
}

template <typename Graph>
void print_verts(Graph& g)
{
    typename Graph::vertex_range rng = g.vertices();
    for( ; rng.first != rng.second; ++rng.first) {
        typename Graph::vertex_descriptor v = *rng.first;
        cout << "vert: " << g[v] << " w/degree == " << g.degree(v) << endl;
    }
}

int main()
{
    vec_vec();
    cout << endl << endl;
    list_list();
    cout << endl << endl;
    set_set();
    cout << endl << endl;

    return 0;
}

void vec_vec()
{
    typedef undirected_graph<City, Road, vertex_vector, edge_vector> Graph;

    Graph g;
    add_verts(g);
    add_edges(g);
}

void list_list()
{
    typedef undirected_graph<City, Road, vertex_list, edge_list> Graph;

    Graph g;
    add_verts(g);
    add_and_del_edges(g);
    test_multi_edge(g);
}


void set_set()
{
    typedef undirected_graph<City, Road, vertex_set<>, edge_set<> > Graph;

    Graph g;
    add_verts(g);
    add_and_del_edges(g);
    test_multi_edge(g);
}

