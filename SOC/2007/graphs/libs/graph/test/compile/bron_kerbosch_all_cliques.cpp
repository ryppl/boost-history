// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include "archetypes.hpp"
#include <boost/graph/bron_kerbosch_all_cliques.hpp>

#include <iostream>

using namespace std;
using namespace boost;

int
main(int argc, char *argv[])
{
    typedef graph_archetype<
            undirected_tag,
            allow_parallel_edge_tag
        > Graph;
    {
        typedef incidence_graph_archetype<Graph> IncidenceGraph;
        typedef vertex_list_graph_archetype<IncidenceGraph> VertexListGraph;
        typedef adjacency_matrix_archetype<VertexListGraph> AdjacencyMatrix;
        typedef vertex_index_graph_archetype<AdjacencyMatrix> VertexIndexGraph;
        typedef clique_visitor_archetype CliqueVisitor;

        VertexIndexGraph& g = static_object<VertexIndexGraph>::get();
        CliqueVisitor v;

        bron_kerbosch_all_cliques(g, v);
        bron_kerbosch_clique_number(g);
    }

    return 0;
}
