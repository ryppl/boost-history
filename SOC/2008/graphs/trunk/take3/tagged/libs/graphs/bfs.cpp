
#include <iostream>
#include <string>
#include <vector>

#include <boost/utility.hpp>

#include <boost/graphs/adjacency_list.hpp>
#include <boost/graphs/breadth_first_search.hpp>

// FIXME: this doesn't compile under g++4.4. Something about the changing
// the meaning of a type (i'm guessing template to type name).
// #include <boost/date_time.hpp>

#include "demangle.hpp"

using namespace std;
using namespace boost::graphs;
using namespace boost::graphs::adj_list;
// using namespace boost::posix_time;

struct VertexProps
{
    string name;
    int age;
};

struct EdgeProps
{
    string relation;
    double weight;
};

// Notes on timing...
// Obviously (and somewhat unfortunately), hashed property containers will
// never perform as efficiently as a vector - that's just the way of things.
// However, there's a serious tradeoff between flexibility (which is what
// you're getting with lists, sets, maps) and speed (which you get with vectors
// at the cost of half-mutability).

static const size_t N = 1000;

void test_1();
void test_2();
void test_3();

int main()
{
    test_1();
    cout << endl << endl;
    test_2();
    cout << endl << endl;
    test_3();

    return 0;
}

void test_1()
{
    typedef undirected_graph<int, double, vertex_set> Graph;
    typedef exterior_vertex_property<Graph, color>::container_type ColorContainer;
    typedef exterior_vertex_property<Graph, color>::map_type ColorMap;

    Graph g;

    // Add a couple thousand vertices
    for(size_t i = 0; i < N; ++i) {
        g.add_vertex(i);
    }

    // Create a color map for the vertices.
    ColorContainer colors(g.vertices());
    ColorMap cm(colors);

    // ptime start = microsec_clock::local_time();
    for(Graph::vertex_iterator i = g.begin_vertices(); i != g.end_vertices(); ++i) {
        colors[*i] = black_color;
    }
    // ptime stop = microsec_clock::local_time();
    // cout << "time: " << stop - start << endl;

    Graph::vertex_descriptor v = *g.begin_vertices();
    put(cm, v, color_traits<color>::red());
    cout << get(cm, v) << endl;

    interior_vertex_property<Graph>::map_type im(g);
    put(im, v, 12);
    cout << "test: " << get(im, v) << endl;
}

void test_2()
{
    typedef undirected_graph<int, int, vertex_vector> Graph;
    typedef exterior_vertex_property<Graph, color>::container_type ColorContainer;

    Graph g;

    // Add a couple thousand vertices
    for(size_t i = 0; i < N; ++i) {
        g.add_vertex(i);
    }

    // Create a color map for the vertices.
    // ptime start = microsec_clock::local_time();
    ColorContainer colors(g.vertices());
    for(Graph::vertex_iterator i = g.begin_vertices(); i != g.end_vertices(); ++i) {
        colors[*i] = black_color;
    }
    // ptime stop = microsec_clock::local_time();
    // cout << "time: " << stop - start << endl;
}

void test_3()
{
    typedef undirected_graph<VertexProps, EdgeProps, vertex_list> Graph;
    typedef interior_vertex_property<Graph, string VertexProps::*>::map_type NameMap;
    typedef interior_vertex_property<Graph, double EdgeProps::*>::map_type WeightMap;

    Graph g;

    // Add a couple thousand vertices
    for(size_t i = 0; i < N; ++i) {
        g.add_vertex();
    }

    NameMap nm(g, &VertexProps::name);
    Graph::vertex_descriptor v = *g.begin_vertices();

    WeightMap wm(g, &EdgeProps::weight);

    put(nm, v, "Bob");
    cout << "test: " << get(nm, v) << endl;


}
