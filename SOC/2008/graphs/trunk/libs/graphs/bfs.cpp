
#include <iostream>
#include <queue>

#include <boost/graphs/undirected_graph.hpp>
#include <boost/graphs/algorithms/breadth_first.hpp>

#include "typestr.hpp"
#include "read.hpp"

using namespace std;

template <typename Graph, typename Walk>
void iterate(Graph& g, Walk& walk)
{
    typedef typename Walk::vertex_descriptor Vertex;

    typedef typename Walk::iterator Iterator;
    typedef typename Iterator::value_type Edge;
    typedef pair<Iterator, Iterator> Range;

    Range rng(walk.begin(), walk.end());
    for( ; rng.first != rng.second; ++rng.first) {
        Vertex u = rng.first->source();
        Vertex v = rng.first->target();
        cout << "edge: " << g[u] << g[v] << endl;
    }
}

template <typename Graph>
void edge_walk(Graph& g)
{
    typedef typename Graph::vertex_descriptor Vertex;
    typedef typename Graph::edge_descriptor Edge;
    typedef exterior_vertex_property<Graph, color> ColorProp;
    typedef exterior_property_map<ColorProp> ColorMap;
    typedef queue<Vertex> Queue;

    {
        typedef breadth_first_edge_walk<Graph> Walk;
        cout << "--- default walk ----" << endl;
        Walk walk(g, *g.begin_vertices());
        iterate(g, walk );
    }

    {
        typedef breadth_first_edge_walk<Graph, ColorMap, Queue> BreadthFirstWalk;
        cout << "--- custom walk ---" << endl;
        ColorProp color_prop(g, white_color);
        ColorMap color(color_prop);
        BreadthFirstWalk walk(g, *g.begin_vertices(), color);
        iterate(g, walk);
    }


    {
        typedef breadth_first_edge_traversal<Graph> Traversal;
        cout << "--- default traversal ---" << endl;
        Traversal trav(g);
        iterate(g, trav);
    }

    {
        typedef breadth_first_edge_traversal<Graph, ColorMap, Queue> BreadthFirstTraversal;
        cout << "--- custom traversal ---" << endl;
        ColorProp color_prop(g, white_color);
        ColorMap color(color_prop);
        BreadthFirstTraversal trav(g, color);
        iterate(g, trav);
    }

}

int main()
{
    typedef undirected_graph<string, string, vertex_set<>, edge_set<>> Graph;

    Graph g;
    read(cin, g);

    edge_walk(g);

    return 0;
}
