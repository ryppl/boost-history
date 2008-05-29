
#include <iostream>
#include <string>
#include <vector>

#include <typeinfo>
#include <cxxabi.h>

#include <boost/graphs/adjacency_list.hpp>
#include "print_vertices.hpp"

using namespace std;
using namespace boost::graphs;
using namespace boost::graphs::adj_list;

string demangle(string const& name)
{
    return string(abi::__cxa_demangle(name.c_str(), 0, 0, 0));
}

static void test_graph();
static void test_multigraph();

static void vector_simple_graph();
static void list_simple_graph();
static void set_simple_graph();
static void map_simple_graph();

int
main(int argc, char *argv[])
{
    test_graph();
    test_multigraph();

    cout << "*** vector simple graph ***" << endl;
    vector_simple_graph();

    cout << "*** list simple graph ***" << endl;
    list_simple_graph();

    cout << "*** set simple graph ***" << endl;
    set_simple_graph();

    cout << "*** map simple graph ***" << endl;
    map_simple_graph();

    return 0;
}

void
test_graph()
{
    graph<> g;
}

void
test_multigraph()
{
    multigraph<> g;
}

void
vector_simple_graph()
{
    static string V[] = {
        "Houston", "Dallas", "Fort Worth", "Austin", "San Antonio", "El Paso",
        "Austin", "Dallas"
    };
    int N = sizeof(V) / sizeof(string);

    typedef adjacency_list<
            undirected,
            string,
            none,
            vertex_vector,
            edge_vector
        > Graph;
    Graph g;

    for(int i = 0; i < N; ++i) {
        g.add_vertex(V[i]);
    }

    print_vertices(g);
}

void
list_simple_graph()
{
    static string V[] = {
        "Houston", "Dallas", "Fort Worth", "Austin", "San Antonio", "El Paso",
        "Austin", "Dallas"
    };
    int N = sizeof(V) / sizeof(string);

    typedef adjacency_list<
            undirected,
            string,
            none,
            vertex_list
        > Graph;
    Graph g;

    for(int i = 0; i < N; ++i) {
        g.add_vertex(V[i]);
    }

    print_vertices(g);
}

void
set_simple_graph()
{
    static string V[] = {
        "Houston", "Dallas", "Fort Worth", "Austin", "San Antonio", "El Paso",
        "Austin", "Dallas"
    };
    int N = sizeof(V) / sizeof(string);

    typedef adjacency_list<
            undirected,
            string,
            none,
            vertex_set
        > Graph;
    Graph g;

    for(int i = 0; i < N; ++i) {
       g.add_vertex(V[i]);
    }

    print_vertices(g);
}

void
map_simple_graph()
{
    static string V[] = {
        "Houston", "Dallas", "Fort Worth", "Austin", "San Antonio", "El Paso",
        "Austin", "Dallas"
    };
    int N = sizeof(V) / sizeof(string);

    typedef adjacency_list<
            undirected,
            int,
            none,
            string_to_vertex_map,
            edge_list,
            vertex_edge_list,
            no_loops_policy
        > Graph;
    Graph g;

    for(int i = 0; i < N; ++i) {
        g.add_vertex(V[i], i);
    }

    Graph::vertex_descriptor houston = g.find_vertex("Houston");
    Graph::vertex_descriptor dallas = g.find_vertex("Dallas");
    Graph::vertex_descriptor ftworth = g.find_vertex("Fort Worth");

    std::cout << "Houston: " << houston.get() << endl;
    std::cout << "Dallas: " << dallas.get() << endl;
    std::cout << "Fort Worth: " << ftworth.get() << endl;

    Graph::edge_descriptor hd = g.add_edge(houston, dallas);
    Graph::edge_descriptor fw = g.add_edge(ftworth, houston);

    cout << "edges: " << g.num_edges() << endl;
    g.remove_edge(hd);
    g.remove_edge(houston, ftworth);
    cout << "edges: " << g.num_edges() << endl;

    // print_vertices(g);

}

