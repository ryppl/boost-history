// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <map>
#include <tr1/unordered_map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/generators/complete_graph.hpp>
#include <boost/graph/generators/cycle_graph.hpp>
#include <boost/graph/generators/star_graph.hpp>
#include <boost/graph/generators/wheel_graph.hpp>
#include <boost/graph/generators/prism_graph.hpp>
#include <boost/graph/generators/web_graph.hpp>
#include <boost/graph/generators/path_graph.hpp>
#include <boost/graph/graphviz.hpp>

using namespace std;
using namespace boost;

static const int N = 3;
static const int K = 2;

template <class Graph>
void test_path()
{
    Graph g;
    make_path_graph(g, N);
    write_graphviz(cout, g);
}

template <class Graph, class Direction>
void test_path(Direction dir)
{
    Graph g;
    make_path_graph(g, N, dir);
    write_graphviz(cout, g);
}



template <class Graph>
void test_cycle()
{
    Graph g;
    make_cycle_graph(g, N);
    write_graphviz(cout, g);
}

template <class Graph, class Cycle>
void test_cycle(Cycle cycle)
{
    Graph g;
    make_cycle_graph(g, N, cycle);
    write_graphviz(cout, g);
}


template <class Graph>
void test_star()
{
    Graph g;
    make_star_graph(g, N);
    write_graphviz(cout, g);
}

template <class Graph, class Spoke>
void test_star(Spoke spokes)
{
    Graph g;
    make_star_graph(g, N, spokes);
    write_graphviz(cout, g);
}



template <class Graph>
void test_wheel()
{
    Graph g;
    make_wheel_graph(g, N);
    write_graphviz(cout, g);
}

template <class Graph, class Cycle, class Spoke>
void test_wheel(Cycle cycle, Spoke spokes)
{
    Graph g;
    make_wheel_graph(g, N, cycle, spokes);
    write_graphviz(cout, g);
}



template <class Graph>
void test_prism()
{
    Graph g;
    make_prism_graph(g, N, K);
    write_graphviz(cout, g);
}

template <class Graph, class Cycle, class Spoke>
void test_prism(Cycle cycle, Spoke spokes)
{
    Graph g;
    make_prism_graph(g, N, K, cycle, spokes);
    write_graphviz(cout, g);
}


template <class Graph>
void test_web()
{
    Graph g;
    make_web_graph(g, N, K);
    write_graphviz(cout, g);
}

template <class Graph, class Cycle, class Spoke>
void test_web(Cycle cycle, Spoke spokes)
{
    Graph g;
    make_web_graph(g, N, K, cycle, spokes);
    write_graphviz(cout, g);
}


template <class Graph>
void test_complete()
{
    Graph g;
    make_complete_graph(g, N);
    write_graphviz(cout, g);
}

int
main(int argc, char *argv[])
{
    typedef undirected_graph<> Graph;
    typedef directed_graph<> DiGraph;

    with_clockwise_cycle clock;
    with_bidirected_spokes spokes;

    /*
    test_complete<Graph>();
    test_path<Graph>();
    test_cycle<Graph>();
    test_star<Graph>();
    test_wheel<Graph>();
    test_prism<Graph>();
    */
    // test_web<Graph>();

    test_prism<DiGraph>(clock, spokes);

    /*
    test_path<DiGraph>(with_forward_edges());
    test_path<DiGraph>(with_reverse_edges());
    test_path<DiGraph>(with_bidirected_edges());
    */

    /*
    test<DiGraph>(clockwise, outward);
    test<DiGraph>(counterclockwise, outward);
    test<DiGraph>(bothwise, outward);
    test<DiGraph>(clockwise, inward);
    test<DiGraph>(counterclockwise, inward);
    test<DiGraph>(bothwise, inward);
    test<DiGraph>(clockwise, iospokes);
    test<DiGraph>(counterclockwise, iospokes);
    test<DiGraph>(bothwise, iospokes);
    */
}
