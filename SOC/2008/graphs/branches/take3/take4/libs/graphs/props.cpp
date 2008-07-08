
#include <iostream>
#include <string>
#include <list>

#include <boost/graphs/directed_graph.hpp>
#include <boost/graphs/undirected_graph.hpp>
#include <boost/graphs/properties.hpp>

#include "demangle.hpp"

using namespace std;
using namespace boost;

void un_vec_vec();
void un_list_list();

template <typename Graph>
void make_square(Graph& g)
{
    typename Graph::vertex_descriptor v[4];
    v[0] = g.add_vertex(0);
    v[1] = g.add_vertex(0);
    v[2] = g.add_vertex(0);
    v[3] = g.add_vertex(0);
    g.add_edge(v[0], v[1]);
    g.add_edge(v[1], v[2]);
    g.add_edge(v[2], v[3]);
    g.add_edge(v[3], v[0]);
    g.add_edge(v[0], v[2]);
    g.add_edge(v[1], v[3]);
}

template <typename Graph>
void test_ext_vertex_props(Graph& g)
{
    typedef exterior_vertex_property<Graph, double> VertexWeight;
    typedef exterior_edge_property<Graph, double> EdgeWeight;

    VertexWeight vp(g, 5.0);
    EdgeWeight ep(g, 1.0);

    cout << "vertex using: " << demangle<typename VertexWeight::base_type>() << endl;
    cout << "edge using: " << demangle<typename EdgeWeight::base_type>() << endl;

    cout << ep[*g.begin_edges()] << endl;
    cout << ep.data.size() << endl;
}

int main()
{
    un_vec_vec();
    un_list_list();

    return 0;
}

void un_vec_vec()
{
    typedef undirected_graph<int, int, vertex_vector<>, edge_vector<> > Graph;
    Graph g;
    make_square(g);
    test_ext_vertex_props(g);
}

void un_list_list()
{
    typedef undirected_graph<int, int, vertex_list<>, edge_list<> > Graph;
    Graph g;
    make_square(g);
    // test_ext_vertex_props(g);
}
