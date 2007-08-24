// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include "archetypes.hpp"
#include <boost/graph/tiernan_all_cycles.hpp>

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
        typedef vertex_index_graph_archetype<VertexListGraph> VertexIndexGraph;
        typedef cycle_visitor_archetype CycleVisitor;

        VertexIndexGraph& g = static_object<VertexIndexGraph>::get();
        CycleVisitor v;

        tiernan_all_cycles(g, v);
        tiernan_girth(g);
        tiernan_circumference(g);
        tiernan_girth_and_circumference(g);
    }

    return 0;
}
