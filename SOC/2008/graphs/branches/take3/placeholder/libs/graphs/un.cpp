
#include <iostream>
#include <set>

#include <boost/assert.hpp>
#include <boost/utility.hpp>
#include <boost/graphs/undirected_graph.hpp>
#include <boost/graphs/traits.hpp>

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
void test_add_edges()
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

template <typename Graph>
void test_add_remove_edges()
{
    Graph g;
    vector<typename Graph::vertex_descriptor> V;
    list<typename Graph::edge_descriptor> E;
    for(int i = 0; i < 3; ++i) {
        V.push_back(g.add_vertex(i));
    }
    E.push_back(g.add_edge(V[0], V[1]));
    E.push_back(g.add_edge(V[1], V[2]));
    E.push_back(g.add_edge(V[2], V[0]));
    BOOST_ASSERT(g.num_vertices() == 3);
    BOOST_ASSERT(g.num_edges() == 3);

    g.remove_edge(E.front());
    BOOST_ASSERT(g.num_edges() == 2);
    BOOST_ASSERT(g.degree(V[1]) == 1);
    E.pop_front();

    g.remove_edge(E.front());
    BOOST_ASSERT(g.degree(V[1]) == 0);
    E.pop_front();
}

template <typename Graph>
void test_disconnect_vertex()
{
    Graph g;
    vector<typename Graph::vertex_descriptor> V;
    list<typename Graph::edge_descriptor> E;
    for(int i = 0; i < 3; ++i) {
        V.push_back(g.add_vertex(i));
    }
    g.add_edge(V[0], V[1]);
    g.add_edge(V[1], V[2]);
    g.add_edge(V[2], V[0]);
    BOOST_ASSERT(g.num_vertices() == 3);
    BOOST_ASSERT(g.num_edges() == 3);

    g.remove_edges(V[1]);
    BOOST_ASSERT(g.num_vertices() == 3);
    BOOST_ASSERT(g.degree(V[1]) == 0);
    BOOST_ASSERT(g.degree(V[0]) == 1);
    BOOST_ASSERT(g.degree(V[2]) == 1);
}

// This is a little different than above. We remove a vertex from the triangle,
// which implicitly disconnects it.
template <typename Graph>
void test_implicit_disconnect_vertex()
{
    Graph g;
    vector<typename Graph::vertex_descriptor> V;
    list<typename Graph::edge_descriptor> E;
    for(int i = 0; i < 3; ++i) {
        V.push_back(g.add_vertex(i));
    }
    g.add_edge(V[0], V[1]);
    g.add_edge(V[1], V[2]);
    g.add_edge(V[2], V[0]);
    BOOST_ASSERT(g.num_vertices() == 3);
    BOOST_ASSERT(g.num_edges() == 3);

    g.remove_vertex(V[1]);
    BOOST_ASSERT(g.num_vertices() == 2);
    BOOST_ASSERT(g.degree(V[0]) == 1);
    BOOST_ASSERT(g.degree(V[2]) == 1);
}

template <typename Graph>
void test_add_multi_edges()
{
    Graph g;
    typename Graph::vertex_descriptor u = g.add_vertex(1);
    typename Graph::vertex_descriptor v = g.add_vertex(2);
    g.add_edge(u, v);
    g.add_edge(v, u);
    g.add_edge(u, v);
    g.add_edge(v, u);

    BOOST_ASSERT(g.num_vertices() == 2);
    if(allows_parallel_edges(g)) {
        BOOST_ASSERT(g.num_edges() == 1);
    }
    else {
        BOOST_ASSERT(g.num_edges() == 4);
    }
}

template <typename Graph>
void test_remove_multi_edges()
{
    Graph g;
    typename Graph::vertex_descriptor u = g.add_vertex(1);
    typename Graph::vertex_descriptor v = g.add_vertex(2);
    g.add_edge(u, v);
    g.add_edge(v, u);
    g.add_edge(u, v);
    g.add_edge(v, u);

    g.remove_edges(u, v);
    BOOST_ASSERT(g.num_edges() == 0);
}

template <typename Graph>
void test_incidence_iterator()
{
    Graph g;
    typename Graph::vertex_descriptor u = g.add_vertex(1);
    typename Graph::vertex_descriptor v = g.add_vertex(2);
    g.add_edge(u, v, 1);
    g.add_edge(v, u, 2);
    g.add_edge(u, v, 3);
    g.add_edge(v, u, 4);
    cout << "done building" << endl;

    typename Graph::incident_edge_range x = g.incident_edges(v);

    typename Graph::incident_edge_iterator i = x.first;
    for(typename Graph::incident_edge_iterator i = x.first; i != x.second; ++i) {
        cout << *i << endl;
    }
}

template <typename Graph>
void test_adjacency_iterator()
{
    Graph g;
    typename Graph::vertex_descriptor u = g.add_vertex(1);
    typename Graph::vertex_descriptor v = g.add_vertex(2);
    g.add_edge(u, v, 1);
    g.add_edge(v, u, 2);
    g.add_edge(u, v, 3);
    g.add_edge(v, u, 4);

    typename Graph::adjacent_vertex_range x = g.adjacent_vertices(v);
    for( ; x.first != x.second; ++x.first) {
        // cout << "test: " << g[*x.first] << endl;
    }
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
    cout << "---- vec/vec ----" << endl;
    typedef undirected_graph<City, Road, vertex_vector<>, edge_vector<> > Graph;
    // test_add_vertices<Graph>();
    // test_add_edges<Graph>();
    // test_add_multi_edges<Graph>();
    test_incidence_iterator<Graph>();
    // test_adjacency_iterator<Graph>();
}

void list_list()
{
    /*
    cout << "---- list/list ----" << endl;
    typedef undirected_graph<City, Road, vertex_list<>, edge_list<> > Graph;
    test_add_remove_vertices<Graph>();
    test_add_remove_edges<Graph>();
    test_disconnect_vertex<Graph>();
    test_implicit_disconnect_vertex<Graph>();
    test_add_multi_edges<Graph>();
    test_remove_multi_edges<Graph>();
    test_incidence_iterator<Graph>();
    test_adjacency_iterator<Graph>();
    */
}


void set_set()
{
    /*
    cout << "---- set/set ----" << endl;
    typedef undirected_graph<City, Road, vertex_set<>, edge_set<> > Graph;
    test_add_remove_vertices<Graph>();
    test_add_remove_edges<Graph>();
    test_disconnect_vertex<Graph>();
    test_implicit_disconnect_vertex<Graph>();
    test_add_multi_edges<Graph>();
    test_remove_multi_edges<Graph>();
    test_incidence_iterator<Graph>();
    test_adjacency_iterator<Graph>();
    */
}
