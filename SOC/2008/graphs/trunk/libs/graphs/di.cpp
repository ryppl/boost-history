
#include <iostream>
#include <set>

#include <boost/assert.hpp>
#include <boost/graphs/directed_graph.hpp>

#include "typestr.hpp"
#include "out_edge_traits.hpp"

using namespace std;
using namespace boost;

typedef int City;
typedef int Road;

void list_list();
void vec_vec();
void set_set();

namespace dispatch
{
    bool allows_parallel_edges(unique_associative_container_tag)
    { return false; }

    bool allows_parallel_edges(multiple_associative_container_tag)
    { return false; }

    bool allows_parallel_edges(sequence_tag)
    { return true; }
}

template <typename Graph>
bool allows_parallel_edges(Graph const& g)
{
    typedef typename Graph::out_store Store;
    return dispatch::allows_parallel_edges(typename out_edge_traits<Store>::category());
}


template <typename Graph>
void print_types(const Graph&)
{
    /*
    cout << demangle(typeid(typename Graph::property_descriptor).name()) << endl;
    cout << demangle(typeid(typename Graph::vertex_descriptor).name()) << endl;
    cout << demangle(typeid(typename Graph::edge_descriptor).name()) << endl;
    cout << demangle(typeid(typename Graph::property_store).name()) << endl;
    cout << demangle(typeid(typename Graph::vertex_store).name()) << endl;
    */
}

template <typename Graph>
void test_add_vertices()
{
    Graph g;
    cout << "  * adding vertices" << endl;
    list<typename Graph::vertex_descriptor> V;
    for(int i = 0; i < 5; ++i) {
        V.push_back(g.add_vertex(i));
    }
}

template <typename Graph>
void test_add_remove_vertices()
{
    Graph g;
    cout << "  * adding vertices" << endl;
    list<typename Graph::vertex_descriptor> V;
    for(int i = 0; i < 5; ++i) {
        V.push_back(g.add_vertex(i));
    }

    cout << "  * removing vertices" << endl;
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

    cout << "  * adding edges" << endl;
    g.add_edge(V[0], V[1]);
    g.add_edge(V[1], V[2]);
    g.add_edge(V[2], V[0]);
    BOOST_ASSERT(g.num_vertices() == 3);
    BOOST_ASSERT(g.num_edges() == 3);
    BOOST_ASSERT(g.degree(V[0]) == 2);
    BOOST_ASSERT(g.out_degree(V[0]) == 1);
    BOOST_ASSERT(g.in_degree(V[0]) == 1);
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

    cout << "  * adding edges" << endl;
    E.push_back(g.add_edge(V[0], V[1]));
    E.push_back(g.add_edge(V[1], V[2]));
    E.push_back(g.add_edge(V[2], V[0]));
    BOOST_ASSERT(g.num_vertices() == 3);
    BOOST_ASSERT(g.num_edges() == 3);
    BOOST_ASSERT(g.degree(V[0]) == 2);
    BOOST_ASSERT(g.out_degree(V[0]) == 1);
    BOOST_ASSERT(g.in_degree(V[0]) == 1);

    cout << "  * removing edges" << endl;
    g.remove_edge(E.front());
    BOOST_ASSERT(g.degree(V[1]) == 1);
    BOOST_ASSERT(g.out_degree(V[0]) == 0);
    BOOST_ASSERT(g.in_degree(V[0]) == 1);
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

    // Ensure that the graph is set up correctly.
    BOOST_ASSERT(g.num_vertices() == 3);
    BOOST_ASSERT(g.num_edges() == 3);
    BOOST_ASSERT(g.out_degree(V[0]) == 1);
    BOOST_ASSERT(g.out_degree(V[1]) == 1);
    BOOST_ASSERT(g.out_degree(V[2]) == 1);
    BOOST_ASSERT(g.in_degree(V[0]) == 1);
    BOOST_ASSERT(g.in_degree(V[1]) == 1);
    BOOST_ASSERT(g.in_degree(V[2]) == 1);
    BOOST_ASSERT(g.edge(V[0], V[1]));
    BOOST_ASSERT(g.edge(V[2], V[0]));

    // Disconnect the vertex
    g.remove_edges(V[0]);

    // Assert that the graph structure is correct after cutting v0 out of the
    // graph. The only remaining edge should be (v1, v2)
    BOOST_ASSERT(g.num_vertices() == 3);
    BOOST_ASSERT(g.num_edges() == 1);
    BOOST_ASSERT(g.out_degree(V[0]) == 0);
    BOOST_ASSERT(g.out_degree(V[1]) == 1);
    BOOST_ASSERT(g.out_degree(V[2]) == 0);

    BOOST_ASSERT(g.in_degree(V[0]) == 0);
    BOOST_ASSERT(g.in_degree(V[1]) == 0);
    BOOST_ASSERT(g.in_degree(V[2]) == 1);

    // This isn't really part of this test. See the multi-edge removals.
    g.remove_edges(V[1], V[2]);
    BOOST_ASSERT(g.num_edges() == 0);
    BOOST_ASSERT(g.out_degree(V[1]) == 0);
    BOOST_ASSERT(g.in_degree(V[2]) == 0);
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

    cout << "  * adding multiple edge" << endl;
    g.add_edge(u, v);
    g.add_edge(v, u);
    g.add_edge(u, v);
    g.add_edge(v, u);

    BOOST_ASSERT(g.num_vertices() == 2);
    if(allows_parallel_edges(g)) {
        BOOST_ASSERT(g.num_edges() == 4);
    }
    else {
        // Two edges: (u,v) and (v,u)
        BOOST_ASSERT(g.num_edges() == 2);
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

    cout << "  * removing multiple edges" << endl;
    g.remove_edges(u, v);
    BOOST_ASSERT(g.out_degree(u) == 0);
    BOOST_ASSERT(g.in_degree(v) == 0);
    BOOST_ASSERT(g.num_edges() == allows_parallel_edges(g) ? 2 : 1);
    BOOST_ASSERT(g.out_degree(v) == allows_parallel_edges(g) ? 2 : 1);
    BOOST_ASSERT(g.in_degree(u) == allows_parallel_edges(g) ? 2 : 1);
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

    typename Graph::incident_edge_range rng = g.incident_edges(u);
    for( ; rng.first != rng.second; ++rng.first) {
        typename Graph::edge_descriptor e = *rng.first;
        BOOST_ASSERT(e.source() == u);
        BOOST_ASSERT(e.target() == v);
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

    typename Graph::adjacent_vertex_range rng = g.adjacent_vertices(u);
    for( ; rng.first != rng.second; ++rng.first) {
        BOOST_ASSERT(*rng.first == v);
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
    typedef directed_graph<City, Road, vertex_vector<>, edge_vector<> > Graph;
    test_add_vertices<Graph>();
    test_add_edges<Graph>();
    test_add_multi_edges<Graph>();
    /*
    test_incidence_iterator<Graph>();
    test_adjacency_iterator<Graph>();
    */
}

void list_list()
{
    cout << "---- list/list ----" << endl;
    typedef directed_graph<City, Road, vertex_list<>, edge_list<> > Graph;
    test_add_remove_vertices<Graph>();
    test_add_remove_edges<Graph>();
    test_disconnect_vertex<Graph>();
    test_implicit_disconnect_vertex<Graph>();
    test_add_multi_edges<Graph>();
    test_remove_multi_edges<Graph>();
    /*
    test_incidence_iterator<Graph>();
    test_adjacency_iterator<Graph>();
    */
}


void set_set()
{
    cout << "---- set/set ----" << endl;
    typedef directed_graph<City, Road, vertex_set<>, edge_set<> > Graph;
    test_add_remove_vertices<Graph>();
    test_add_remove_edges<Graph>();
    test_disconnect_vertex<Graph>();
    test_implicit_disconnect_vertex<Graph>();
    test_add_multi_edges<Graph>();
    test_remove_multi_edges<Graph>();
    test_incidence_iterator<Graph>();
    test_adjacency_iterator<Graph>();
}

