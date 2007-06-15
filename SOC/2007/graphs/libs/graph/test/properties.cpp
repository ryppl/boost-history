
// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/graph/undirected_graph.hpp>

using namespace std;
using namespace boost;

struct Foo
{
    int foo;
};

typedef undirected_graph<Foo> Graph;
typedef Graph::vertex_descriptor Vertex;

typedef property_map<Graph, vertex_index_t>::type IndexMap;
typedef property_map<Graph, int Foo::*>::type FooMap;

int main()
{
    Graph g;

    Vertex u = add_vertex(g);
    Vertex v = add_vertex(g);

    IndexMap indices = get(vertex_index, g);
    FooMap foos = get(&Foo::foo, g);

    g[u].foo = 3;
    g[v].foo = 5;

    cout << indices[u] << " " << foos[u] << "\n";
    cout << indices[v] << " " << foos[v] << "\n";

    cout << "test: " << get(&Foo::foo, g, v) << "\n";
    put(&Foo::foo, g, v, 12);
    cout << "test: " << g[v].foo << "\n";

    return 0;
}
