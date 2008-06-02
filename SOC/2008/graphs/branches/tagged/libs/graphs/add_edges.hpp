
#ifndef ADD_EDGES_HPP
#define ADD_EDGES_HPP

#include "random_vertex.hpp"

template <typename Graph>
void
add_edges(Graph& g, int n)
{
    typedef typename Graph::vertex_descriptor Vertex;

    for(int i = 0; i < n; ++i) {
        Vertex u = random_vertex(g);
        Vertex v = random_vertex(g);
        g.add_edge(u, v);
    }
}

#endif

