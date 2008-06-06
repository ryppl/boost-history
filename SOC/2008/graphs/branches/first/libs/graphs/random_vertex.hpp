
#ifndef RANDOM_VERTEX_HPP
#define RANDOM_VERTEX_HPP

#include <boost/utility.hpp>

// Probably an inefficient implementation of a random vertex.
// This just picks some vertex in the range of vertices.
template <typename Graph>
typename Graph::vertex_descriptor
random_vertex(Graph const& g)
{
    using std::rand;
    using boost::next;

    int n = rand() % g.num_vertices();
    return *next(g.begin_vertices(), n);
}

#endif

