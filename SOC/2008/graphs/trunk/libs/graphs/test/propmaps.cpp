
#include <iostream>
#include <iterator>

#include <boost/lexical_cast.hpp>
#include <boost/graphs/adjacency_list/undirected_graph.hpp>
#include <boost/graphs/adjacency_list/directed_graph.hpp>
#include <boost/graphs/properties.hpp>

#include "typestr.hpp"

using namespace std;
using namespace boost;

template <typename Graph>
void test_props()
{
    typedef typename Graph::vertex_descriptor Vertex;
    typedef typename Graph::vertex_properties VertexProps;
    typedef typename Graph::edge_descriptor Edge;
    typedef typename Graph::edge_properties EdgeProps;

    Graph g;
    cout << "--- " << typestr(g) << " ---" << endl;

    // Create a fully connected graph.
    const int N = 5;
    for(int i = 0; i < N; ++i) {
        Vertex u = g.add_vertex(i);
    }
    int x = 0;
    typename Graph::vertex_range rng = g.vertices();
    for( ; rng.first != rng.second; ++rng.first) {
        typename Graph::vertex_iterator i = g.begin_vertices();
        for( ; i != rng.first; ++i) {
            Edge e = g.add_edge(*i, *rng.first, x++);
        }
    }
    cout << g.num_vertices() << " x " << g.num_edges() << endl;

    typedef exterior_vertex_property<Graph, double> VertexWeightProp;
    typedef exterior_property_map<VertexWeightProp> VertexWeightMap;
    VertexWeightProp vertex_weights(g, 6.28);
    VertexWeightMap vw(vertex_weights);

    typedef exterior_edge_property<Graph, double> EdgeWeightProp;
    typedef exterior_property_map<EdgeWeightProp> EdgeWeightMap;
    EdgeWeightProp edge_weights(g, 3.14);
    EdgeWeightMap ew(edge_weights);

    // Set and print the values of an exterior vertex property.
    typename Graph::vertex_range vr = g.vertices();
    for( ; vr.first != vr.second; ++vr.first) {
        vw(*vr.first) = distance(g.begin_vertices(), vr.first) * 2;
        cout << "vertex weight: " << vw(*vr.first) << endl;
    }

    // Set and print the values of an exterior edge property.
    typename Graph::edge_range er = g.edges();
    for( ; er.first != er.second; ++er.first) {
        ew(*er.first) = distance(g.begin_edges(), er.first) * 10;
        cout << "edge weight: " << ew(*er.first) << endl;
    }

    // If bundled, constructed over the entire bundle.

    {
        typedef interior_property_map<Graph, Vertex> PropMap;
        typedef interior_property_map<Graph, Vertex, string VertexProps::*> NameMap;
        PropMap props(g);
        NameMap names(g, &VertexProps::name);
        for(vr.first = g.begin_vertices(); vr.first != vr.second; ++vr.first) {
            Vertex v = *vr.first;
            names(v) = "City " + lexical_cast<string>(props(v).id);
            cout << names(v) << endl;
        }
    }

    {
        typedef interior_property_map<Graph, Edge> PropMap;
        typedef interior_property_map<Graph, Edge, string EdgeProps::*> NameMap;
        PropMap props(g);
        NameMap names(g, &EdgeProps::name);
        for(er.first = g.begin_edges(); er.first != er.second; ++er.first) {
            Edge e = *er.first;
            names(e) = "Road " + lexical_cast<string>(props(e).id);
            cout << names(e) << endl;
        }
    }

    {
        // Generic stuff?
        exterior_vertex_property<Graph, double> these_weights(g, 6.28);

        optional_vertex_map<Graph, double> my_weights(g, 3.14);
        optional_vertex_map<Graph, double> your_weights(these_weights);

        for(vr.first = g.begin_vertices(); vr.first != vr.second; ++vr.first) {
            Vertex v = *vr.first;
            cout << my_weights(v) << ", " << your_weights(v) << endl;
        }
    }
}


struct Object
{
    Object(int id)
        : id(id), name()
    { }

    int id;
    string name;

    inline bool operator<(Object const& x) const
    { return id < x.id; }
};

typedef Object City;
typedef Object Road;

int main()
{
    {
        typedef undirected_graph<City, Road, vertex_vector<>, edge_vector<>> G1;
        typedef undirected_graph<City, Road, vertex_list<>, edge_list<>> G2;
        typedef undirected_graph<City, Road, vertex_set<>, edge_set<>> G3;

        test_props<G1>();
        test_props<G2>();
        test_props<G3>();
    }

    {
        typedef directed_graph<City, Road, vertex_vector<>, edge_vector<>> G1;
        typedef directed_graph<City, Road, vertex_list<>, edge_list<>> G2;
        typedef directed_graph<City, Road, vertex_set<>, edge_set<>> G3;

        test_props<G1>();
        test_props<G2>();
        test_props<G3>();
    }

    return 0;
}