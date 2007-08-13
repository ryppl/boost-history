// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include "social_network.hpp"
#include "helper.hpp"

#include <iostream>
#include <iomanip>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/closeness_centrality.hpp>

using namespace std;
using namespace boost;

//[normalized_closeness_measure
template <typename Graph,
          typename Distance,
          typename Result,
          typename Divide = std::divides<Result> >
struct normalized_closeness_measure
{
    typedef Distance distance_type;
    typedef Result result_type;

    Result operator ()(Distance d, const Graph& g)
    {
        if(d == numeric_values<Distance>::infinity()) {
            return numeric_values<Result>::zero();
        }
        else {
            return div(Result(num_vertices(g)), Result(d));
        }
    }
    Divide div;
};
//]

int
main(int argc, char *argv[])
{
    Graph g;
    map<string, Vertex> verts;

    // Read in and build the graph
    for(string line; getline(cin, line); ) {
        if(line.empty()) continue;
        size_t index = line.find_first_of(',');
        string first(line, 0, index);
        string second(line, index + 1);

        Vertex u = add_named_vertex(g, first, verts);
        Vertex v = add_named_vertex(g, second, verts);
        add_edge(u, v, g);
    }

    DistanceMatrix distances(num_vertices(g));
    DistanceMatrixMap dm(distances, g);
    WeightMap wm(1);
    floyd_warshall_all_pairs_shortest_paths(g, dm, weight_map(wm));
    //]

    // Compute the degree centrality for graph
    //[compute_normalized_closeness
    ClosenessContainer cents(num_vertices(g));
    ClosenessMap cm(cents, g);
    normalized_closeness_measure<Graph, int, float> m;
    closeness_centrality(g, dm, cm, m);
    //]

    vector<Vertex> sorted(num_vertices(g));
    sort_vertices(sorted, g, cm);

    // Print the degree centrality of each vertex
    vector<Vertex>::iterator i, end = sorted.end();
    for(i = sorted.begin(); i != end; ++i) {
        cout << setw(12) << setiosflags(ios::left)
             << g[*i].name << cm[*i] << "\n";
    }

    return 0;
}
