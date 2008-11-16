
#ifndef BOOST_GRAPHS_ADJMATRIX_UNDIRECTED_GRAPH_HPP
#define BOOST_GRAPHS_ADJMATRIX_UNDIRECTED_GRAPH_HPP

#include <vector>

#include <boost/descriptors.hpp>
#include <boost/graphs/adjacency_matrix/basic_matrix.hpp>
#include <boost/graphs/adjacency_matrix/undirected_edge.hpp>

namespace boost { namespace graphs { namespace adjacency_matrix {

/**
 * We should be able to specialize the hell out of the storage unit contained
 * in the matrix, based on the EdgeLabel. At the minimum, we have to store a
 * boolean value indicating whether or not an edge actually exists. If the
 * EdgeLabel is none, we could store this is as a NxN array of bools, or if you
 * really want compressed space - as a bitmap. If the EdgeLabel is not none,
 * then we have to decide how to implement optional storage of that data.
 * This probably comes down to using Boost.Optional to store the edge labels.
 *
 * @todo The undirected graph's underlying matrix should reduce to a triangular
 * matrix. Otherwise, we're just wasting space.
 *
 * @todo Find a way to better parameterize the underlying store for vertex
 * properties. We can basically require something that provides random access,
 * and dynamic resizing. Look at the storage selectors for adjacency lists for
 * inspiration. For now, this is just a vector. Note that whatever the solution
 * is, there has to be a 1-to-1 correspondence between the descriptor for the
 * vertex's label and its position on the matrix (which is why vectors work
 * well).
 *
 * @todo Specialize away the storage requirement for VertexLabel == none.
 */
template <
    typename VertexLabel = none,
    typename EdgeLabel = none,
    typename EdgeMatrix = basic_matrix<EdgeLabel>>
struct undirected_graph
{
    typedef std::vector<VertexLabel> label_store;

    typedef VertexLabel vertex_label;
    typedef EdgeLabel edge_label;

    typedef EdgeMatrix edge_matrix;
    typedef typename edge_matrix::size_type vertices_size_type;
    typedef typename edge_matrix::size_type edges_size_type;

    // Descriptor types.
    // The vertex descriptor plays double duty indexing both the label store
    // and the matrix.
    // The matrix descriptor hosts a unique location into the matrix. For simple
    // matrices this is essentially unused because you can only store a single
    // edge at each (u,v) element of the matrix. For multimatrices, this will
    // probably represent an iterator into a list of edges.
    typedef descriptor_traits<label_store> vertex_descriptor;
    typedef int matrix_descriptor;
    typedef undirected_edge<vertex_descriptor, matrix_descriptor> edge_descriptor;

    // Construct a graph with n vertices, whose vertex and edge labels are all
    // initialized to the given value.
    undirected_graph(vertices_size_type n,
                     vertex_label const& vl = vertex_label(),
                     edge_label const& el = edge_label())
        : _labels(n, vl), _matrix(n, el)
    { }

    template <typename VertexIter, typename EdgeIter>
    undirected_graph(VertexIter fv, VertexIter lv, EdgeIter fe, EdgeIter le)
        : _labels(fv, le), _matrix(std::distance(fv, lv), fe, le)
    { }

    label_store _labels;
    edge_matrix _matrix;
};

} } }

#endif
