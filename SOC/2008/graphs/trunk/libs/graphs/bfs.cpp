
#include <iostream>
#include <fstream>
#include <queue>

#include <boost/graphs/undirected_graph.hpp>
#include <boost/graphs/directed_graph.hpp>
#include <boost/graphs/algorithms/breadth_first.hpp>

#include "typestr.hpp"
#include "read.hpp"

using namespace std;

struct edge_printer : bfs_visitor
{
    template <typename Graph, typename Edge>
    void tree_edge(Graph const& g, Edge e)
    {
        typedef typename Graph::vertex_descriptor Vertex;
        Vertex u = e.source();
        Vertex v = e.target();
        cout << "(" << g[u] << g[v] << ") ";
    }
};

template <typename Graph, typename Walk>
void iterate(Graph& g, Walk& walk)
{
    typedef typename Walk::vertex_descriptor Vertex;

    typedef algorithm_iterator<Walk> Iterator;
    typedef pair<Iterator, Iterator> Range;

    Range rng(walk.begin(), walk.end());
    for( ; rng.first != rng.second; ++rng.first) {
        Vertex u = rng.first->source();
        Vertex v = rng.first->target();
        cout << "(" << g[u] << g[v] << ") ";
    }
    cout << endl;
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
        iterate(g, walk);
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

    {
        ColorProp colors(g, white_color);
        ColorMap color(colors);

        cout << "--- algo visit ---" << endl;
        breadth_first_visit(g, *g.begin_vertices(), edge_printer());
        cout << endl;

        cout << "--- algo multi visit ---" << endl;
        breadth_first_visit(g, g.begin_vertices(), next(g.begin_vertices(), 2), edge_printer());
        cout << endl;

        cout << "--- algo bfs ---" << endl;
        breadth_first_search(g, edge_printer());
        cout << endl;
    }

}

int main(int, char* argv[])
{
    {
        typedef undirected_graph<string, string, vertex_set<>, edge_set<>> Graph;
        Graph g;
        ifstream f(argv[1]);
        read(f, g);

        cout << "=== undirected ===" << endl;
        edge_walk(g);
        cout << endl;
    }

    {
        typedef directed_graph<string, string, vertex_set<>, edge_set<>> Graph;
        Graph g;
        ifstream f(argv[1]);
        read(f, g);

        cout << "=== directed ===" << endl;
        edge_walk(g);
        cout << endl;
    }


    return 0;
}
