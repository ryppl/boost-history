
#ifndef PRINT_VERTICES_HPP
#define PRINT_VERTICES_HPP

template <typename Graph>
void print_vertices(Graph const& g)
{
    typename Graph::vertex_iterator
            i = g.begin_vertices(),
            end = g.end_vertices();
    for( ; i != end; ++i) {
        typename Graph::vertex_descriptor v = *i;
        std::cout << g.properties(v) << std::endl;
    }
}

#endif
