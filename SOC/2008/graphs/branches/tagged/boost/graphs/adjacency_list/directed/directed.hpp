
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_UNDIRECTED_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_UNDIRECTED_HPP

#include <list>

#include <boost/graphs/utility/ordered.hpp>
#include <boost/graphs/adjacency_list/vertex.hpp>
#include <boost/graphs/adjacency_list/edge.hpp>
#include <boost/graphs/adjacency_list/storage_traits.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

// Implementation of directed adjacency lists.

// Forward declarations
template <typename VP, typename EP, typename V, typename E, template <typename> class VES> struct directed_vertex;
template <typename VP, typename EP, typename V, typename E> struct directed_edge;

// Unfortunately, I have to hack out a little tag dispatch here... This will
// go away with the concepts.
struct directed_tag { };

/**
 * This is the metafunction for directed types.
 */
template <
        typename VertexProps,
        typename EdgeProps,
        typename VertexStore,
        typename EdgeStore,
        template <typename> class VertexEdgeStore
    >
struct directed
{
    typedef directed_tag tag;

    typedef vertex_desc<
            typename storage_traits<VertexStore>::descriptor_type
        > vertex_descriptor;

    typedef edge_desc<
            typename storage_traits<EdgeStore>::descriptor_type
        > edge_descriptor;

    typedef directed_vertex<
            VertexProps, EdgeProps, vertex_descriptor, edge_descriptor, VertexEdgeStore
        > vertex_type;

    typedef directed_edge<
            VertexProps, EdgeProps, vertex_descriptor, edge_descriptor
        > edge_type;
};

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#include <boost/graphs/adjacency_list/directed/vertex.hpp>
#include <boost/graphs/adjacency_list/directed/edge.hpp>

#endif
