
#ifndef SQUARE_HPP
#define SQUARE_HPP

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
}

#endif
