
#include <iostream>
#include <set>

#include <boost/assert.hpp>
#include <boost/utility.hpp>
#include <boost/graphs/undirected_graph.hpp>

#include "demangle.hpp"

using namespace std;
using namespace boost;

typedef int City;
typedef int Road;

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
void test_add_vertices()
{
    Graph g;
    list<typename Graph::vertex_descriptor> V;
    for(int i = 0; i < 5; ++i) {
        V.push_back(g.add_vertex(i));
    }
}

template <typename Graph>
void test_add_remove_vertices()
{
    Graph g;
    list<typename Graph::vertex_descriptor> V;
    for(int i = 0; i < 5; ++i) {
        V.push_back(g.add_vertex(i));
    }
    BOOST_ASSERT(g.num_vertices() == 5);
    while(!V.empty()) {
        g.remove_vertex(V.front());
        V.pop_front();
    }
    BOOST_ASSERT(g.num_vertices() == 0);
}

template <typename Graph>
void test_make_simple_triangle()
{
    Graph g;
    vector<typename Graph::vertex_descriptor> V;
    for(int i = 0; i < 3; ++i) {
        V.push_back(g.add_vertex(i));
    }
    g.add_edge(V[0], V[1]);
    g.add_edge(V[1], V[2]);
    g.add_edge(V[2], V[0]);
    BOOST_ASSERT(g.num_vertices() == 3);
    BOOST_ASSERT(g.num_edges() == 3);
}


int main()
{
    vec_vec();
    list_list();
    set_set();

    return 0;
}

void vec_vec()
{
    typedef undirected_graph<City, Road, vertex_vector, edge_vector> Graph;
    test_add_vertices<Graph>();
    test_make_simple_triangle<Graph>();
}

void list_list()
{
    typedef undirected_graph<City, Road, vertex_list, edge_list> Graph;

    Graph g;
    test_add_remove_vertices<Graph>();
    test_make_simple_triangle<Graph>();
}


void set_set()
{
    typedef undirected_graph<City, Road, vertex_set<>, edge_set<> > Graph;
    test_add_remove_vertices<Graph>();
    test_make_simple_triangle<Graph>();
}

