
#ifndef READ_HPP
#define READ_HPP

#include <iostream>
#include <sstream>

/**
 * Populate the graph based on an incidence list. Here, we're assuming that the
 * given graph is simple and has properties for both vertices and edges.
 */
template <typename Graph>
void read(std::istream& is, Graph& g)
{
    typedef typename Graph::vertex_descriptor Vertex;
    typedef typename Graph::vertex_properties VertexLabel;
    typedef typename Graph::edge_properties EdgeLabel;

    for(string line; std::getline(is, line); ) {
        if(line.empty() || line[0] == '#') continue;

        // Force these to be default constructed.
        VertexLabel ul = VertexLabel();
        VertexLabel vl = VertexLabel();
        EdgeLabel el = EdgeLabel();
        std::stringstream ss(line);
        ss >> ul >> vl >> el;

        Vertex u = g.add_vertex(ul);
        Vertex v = g.add_vertex(vl);
        g.add_edge(u, v, el);
    }
}

#endif
