
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>

#include <boost/random.hpp>

#include <boost/graphs/adjacency_list/undirected_graph.hpp>
#include <boost/graphs/adjacency_list/directed_graph.hpp>
#include <boost/graphs/algorithms/search.hpp>
#include <boost/graphs/algorithms/breadth_first.hpp>
#include <boost/graphs/algorithms/depth_first.hpp>

#include "typestr.hpp"

using namespace std;
using namespace boost;

typedef mt19937 Generator;
typedef bernoulli_distribution<> Distribution;

const int N = 10;
const double P = 0.25;

Generator rng;

template <typename Graph, typename RootMap, typename TreeMap, typename TreeOrderMap>
struct search_recorder : default_search_visitor
{
    typedef typename Graph::vertex_descriptor Vertex;
    typedef typename Graph::edge_descriptor Edge;
    typedef directional_edge<Edge> SearchEdge;

    search_recorder(Graph const& g, RootMap root, TreeMap tree, TreeOrderMap tord,
                    int& tsize)
        : root(root), tree(tree), tree_order(tord)
        , tree_size(tsize)
    { }

    void start_vertex(Graph const& g, Vertex v)
    { root(v) = true; }

    void examine_vertex(Graph const& g, Vertex v)
    { }

    void discover_vertex(Graph const& g, Vertex v)
    { }

    void tree_edge(Graph const& g, SearchEdge e)
    {
        tree(e) = true;
        tree_order(e) = ++tree_size;
    }

    RootMap root;
    TreeMap tree;
    TreeOrderMap tree_order;
    int& tree_size;
};

template <typename Graph>
struct graph_traits
{ static const bool directed = false; };

template <typename VP, typename EP, typename VS, typename ES>
struct graph_traits<undirected_graph<VP,EP,VS,ES>>
{ static const bool directed = false; };

template <typename VP, typename EP, typename VS, typename ES>
struct graph_traits<directed_graph<VP,EP,VS,ES>>
{ static const bool directed = true; };

// Create an Erdos-Renyi style random graphs with n vertices and p probability
// of edge connection.
template <typename Graph>
void random_graph(Graph& g, int n, double p)
{
    typedef typename Graph::vertex_descriptor Vertex;
    typedef typename Graph::vertex_iterator VertexIterator;
    typedef typename Graph::vertex_range VertexRange;
    typedef typename Graph::edge_descriptor Edge;

    // Start by adding all the required vertices.
    for(int i = 0; i < N; ++i) {
        g.add_vertex(i);
    }

    // Add edges with the given probability.
    Distribution coin(p);
    VertexRange verts = g.vertices();
    for(VertexIterator i = verts.first; i != verts.second; ++i) {
        for(VertexIterator j = verts.first; j != verts.second; ++j) {
            if(i == j) continue;
            if(coin(rng)) {
                g.add_edge(*i, *j);
            }
        }
    }
}

// Actually
template <typename Graph, typename Recorder>
void draw_algorithm(Graph const& g, Recorder rec, string const& module, string const& alg)
{
    typedef typename Graph::vertex_descriptor Vertex;
    typedef typename Graph::vertex_range VertexRange;
    typedef typename Graph::edge_descriptor Edge;
    typedef typename Graph::edge_range EdgeRange;

    // Open the given file for writing.
    string name = module + "_" + alg + ".dot";
    ofstream os(name.c_str());

    string decl = graph_traits<Graph>::directed ? "digraph" : "graph";
    string sym = graph_traits<Graph>::directed ? "->" : "--";

    os << decl << " {" << endl;

    VertexRange vr = g.vertices();
    for( ; vr.first != vr.second; ++vr.first) {
        Vertex v = *vr.first;
        os << "  " << g[v];
        if(rec.root(v)) {
            os << " [color=blue]";
        }
        os << ";" << endl;
    }

    EdgeRange er = g.edges();
    for( ; er.first != er.second; ++er.first) {
        Edge e = *er.first;
        Vertex u = e.first();
        Vertex v = e.second();
        os << "  " << g[u] << sym << g[v];
        if(rec.tree(e)) {
           os << " [color=red,label=\"" << rec.tree_order(e) << "\"]";
        }
        os << ";" << endl;
    }
    os << "}" << endl;
}

template <typename Graph>
void test(string const& module)
{
    typedef typename Graph::vertex_descriptor Vertex;
    typedef exterior_vertex_label<Graph, color> ColorProp;
    typedef exterior_vertex_label<Graph, bool> RootProp;
    typedef exterior_edge_label<Graph, bool> TreeProp;
    typedef exterior_edge_label<Graph, int> TreeOrderProp;
    typedef exterior_property_map<ColorProp> ColorMap;
    typedef exterior_property_map<RootProp> RootMap;
    typedef exterior_property_map<TreeProp> TreeMap;
    typedef exterior_property_map<TreeOrderProp> TreeOrderMap;
    typedef search_recorder<Graph, RootMap, TreeMap, TreeOrderMap> Recorder;

    Graph g;
    cout << "--- " << typestr(g) << " ---" << endl;

    // Generate a random graph
    random_graph(g, N, P);

    // Run a BFS (determined by the use of a queue).
    {
        ColorProp colors(g, white_color);
        RootProp root(g, false);
        TreeProp tree(g, false);
        TreeOrderProp tord(g, 0);

        ColorMap cm(colors);
        queue<Vertex> q;
        int tsize = 0;
        Recorder rec(g, RootMap(root), TreeMap(tree), TreeOrderMap(tord), tsize);
        breadth_first_search(g, rec);
        draw_algorithm(g, rec, module, string("bfs"));
    }

    // Run a DFS (determined by the use of a stack).
    {
        ColorProp colors(g, white_color);
        RootProp root(g, false);
        TreeProp tree(g, false);
        TreeOrderProp tord(g, 0);

        ColorMap cm(colors);
        stack<Vertex> q;
        int tsize = 0;
        Recorder rec(g, RootMap(root), TreeMap(tree), TreeOrderMap(tord), tsize);
        depth_first_search(g, rec);
        draw_algorithm(g, rec, module, string("dfs"));
    }
}

int main()
{
    typedef undirected_graph<int, int, vertex_set<>, edge_set<>> Graph;
    typedef directed_graph<int, int, vertex_set<>, edge_set<>> Digraph;

    rng.seed(time(0));

    test<Graph>("graph");
    test<Digraph>("digraph");

    return 0;
}

