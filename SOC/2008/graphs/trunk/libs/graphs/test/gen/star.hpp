
#ifndef GEN_STAR_HPP
#define GEN_STAR_HPP

#include <vector>

#include "../traits.hpp"

// Generate an (n-1)-spoked star. Vertex 0 is the center. This assumes that
//
template <typename Graph>
typename Graph::vertex_descriptor
make_star(Graph& g, int n)
{
    std::vector<typename Graph::vertex_descriptor> v(n);
    for(int i = 0; i < n; ++i) {
        v[i] = detail::make_vertex(g, i);
    }
    for(int i = 1; i < n; ++i) {
        add_edge(g, v[0], v[i], i - 1);
    }
    return v[0];
}

#endif
