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
#include <boost/graph/graphviz.hpp>

using namespace std;
using namespace boost;

static const int N = 10;

template <class Graph, class Cycle, class Spoke>
void test_complete(Cycle cycle, Spoke spoke)
{
    Graph g;
    make_complete_graph(g, N);
    write_graphviz(cout, g);
}

template <class Graph, class Cycle, class Spoke>
void test_cycle(Cycle cycle, Spoke spoke)
{
    Graph g;
    make_cycle_graph(g, N, cycle);
    write_graphviz(cout, g);
}

template <class Graph, class Cycle, class Spoke>
void test_star(Cycle cycle, Spoke spoke)
{
    Graph g;
    make_star_graph(g, N, spoke);
    write_graphviz(cout, g);
}

template <class Graph, class Cycle, class Spoke>
void test_wheel(Cycle cycle, Spoke spoke)
{
    Graph g;
    make_wheel_graph(g, N, cycle, spoke);
    write_graphviz(cout, g);
}

template <class Graph, class Cycle, class Spoke>
void test(Cycle cycle, Spoke spoke)
{
    test_complete<Graph>(cycle, spoke);
    test_cycle<Graph>(cycle, spoke);
    test_star<Graph>(cycle, spoke);
    test_wheel<Graph>(cycle, spoke);
}

template <class Graph>
void test()
{
    with_clockwise_cycle cycle;
    with_outward_spokes spoke;
    test<Graph>(cycle, spoke);
}


int
main(int argc, char *argv[])
{
    typedef undirected_graph<> Graph;
    typedef directed_graph<> DiGraph;

    with_clockwise_cycle clockwise;
    with_counterclockwise_cycle counterclockwise;
    with_bidirected_cycle bothwise;

    with_outward_spokes outward;
    with_inward_spokes inward;
    with_bidirected_spokes iospokes;

    test<Graph>();
    test<DiGraph>(clockwise, outward);
    test<DiGraph>(counterclockwise, outward);
    test<DiGraph>(bothwise, outward);
    test<DiGraph>(clockwise, inward);
    test<DiGraph>(counterclockwise, inward);
    test<DiGraph>(bothwise, inward);
    test<DiGraph>(clockwise, iospokes);
    test<DiGraph>(counterclockwise, iospokes);
    test<DiGraph>(bothwise, iospokes);
}
