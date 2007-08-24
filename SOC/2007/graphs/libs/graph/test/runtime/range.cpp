
// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <vector>
#include <boost/utility.hpp>
#include <boost/test/minimal.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/breadth_first_search.hpp>

using namespace std;
using namespace boost;

typedef undirected_graph<> Graph;
typedef Graph::vertex_descriptor Vertex;

static const int N = 5;

void build_n_clique(Graph& g, int n)
{
    // build and tear down an N-clique
    for(int i = 0; i < N; ++i) {
	add_vertex(g);
    }
    Graph::vertex_iterator i, j, end;
    for(tie(i, end) = vertices(g); i != end; ++i) {
	for(j = next(i); j != end; ++j) {
	    add_edge(*i, *j, g);
	}
    }
}

void clear(Graph& g)
{
    // apparently, the only way to safely clear the list of vertices
    // is to use a trailing iterator type of trick. it's not pretty,
    // but it works.
    Graph::vertex_iterator i, j, end;
    tie(i, end) = vertices(g);
    for(j = i; i != end; i = j) {
	++j;
	clear_vertex(*i, g);
	remove_vertex(*i, g);
    }
}

int test_main(int, char *argv[])
{
    Graph g;

    build_n_clique(g, N);
    clear(g);
    build_n_clique(g, N);

    cout << num_vertices(g) << "/" << num_edges(g) << "\n";
    cout << max_vertex_index(g) << "\n";

    // renumber_vertex_indices(g);
    // 
    // WARNING: This will crash without the previous line of code.
    // Why? Deep in the guts of the BFS that gets called below,
    // the algorithm allocates a N-vector of colors that they plan
    // to access via vertex indices. The only problem is that the
    // vertex indices should now be in the range [N, 2*N), well
    // beyond the bounds of the vectors used in the queue.
    //
    // Obviously uncommenting the previous line of code will work
    // just fine, but a better approach might be to change the
    // algorithm to create a vector of size [0, max_vertex_index).
    // Despite the fact that there may gaps in the vector, correct
    // usage of indices will mean that you never actually access
    // non-vertex elements in your exterior property vectors.

    breadth_first_search(
	g, *vertices(g).first,
	visitor(
	    make_bfs_visitor(null_visitor())
	    )
	);
			 
    
    return 0;
}
