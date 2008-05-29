
#ifndef ADD_VERTICES_HPP
#define ADD_VERTICES_HPP

/**
 * Add numbered vertices to the graph. This works for mapped
 * vertices too - as long as the key type is integers. This
 * will just default the mapped vertex properties.
 *
 * @requires ExtendableGraph<Graph>
 * @requires PropertyGraph<Graph>
 * @requires Convertible<Graph::vertex_properties, int>
 */
template <typename Graph>
void add_vertices(Graph& g, int n)
{
    for(int i = 0; i < n; ++i) {
        g.add_vertex(i);
    }
}

#endif
