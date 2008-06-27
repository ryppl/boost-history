
#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <boost/assert.hpp>

// Make a K4 square
template <typename G>
void make_square(G& g)
{
    typename G::vertex_descriptor v[4];
    for(int i = 0; i < 4; ++i) v[i] = g.add_vertex(i);
    for(int i = 0; i < 3; ++i) g.add_edge(v[i], v[i + 1], i);
    g.add_edge(v[3], v[0], 3);
    g.add_edge(v[0], v[2], 4);
    g.add_edge(v[1], v[3], 5);

    // Check the basic structure...
    BOOST_ASSERT(g.num_vertices() == 4);
    BOOST_ASSERT(g.num_edges() == 6);

    // Make sure that this actually does what I think it does. It should have
    // the following edges:
    // (0,1), (1,2), (2,3), (3,0), (0,2), and (1,3)
    BOOST_ASSERT(g.edge(v[0], v[1]).second);
    BOOST_ASSERT(g.edge(v[1], v[2]).second);
    BOOST_ASSERT(g.edge(v[2], v[3]).second);
    BOOST_ASSERT(g.edge(v[3], v[0]).second);
    BOOST_ASSERT(g.edge(v[0], v[2]).second);
    BOOST_ASSERT(g.edge(v[1], v[3]).second);

    // Just to check again, assert that the degrees are correct.
    BOOST_ASSERT(g.degree(v[0]) == 3);
    BOOST_ASSERT(g.degree(v[1]) == 3);
    BOOST_ASSERT(g.degree(v[2]) == 3);
    BOOST_ASSERT(g.degree(v[3]) == 3);
}

#endif
