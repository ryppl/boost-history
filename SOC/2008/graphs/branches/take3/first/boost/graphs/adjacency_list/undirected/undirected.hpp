
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_UNDIRECTED_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_UNDIRECTED_HPP

#include <list>

#include <boost/graphs/utility/unordered_pair.hpp>
#include <boost/graphs/adjacency_list/vertex.hpp>
#include <boost/graphs/adjacency_list/edge.hpp>
#include <boost/graphs/adjacency_list/storage_traits.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

// Implementation of undirected adjacency lists.

// Forward declarations
template <typename VP, typename EP, typename V, typename E, template <typename> class VES> struct undirected_vertex;
template <typename VP, typename EP, typename V, typename E> struct undirected_edge;

// Unfortunately, I have to hack out a little tag dispatch here... This will
// go away with the concepts.
struct undirected_tag { };

/**
 * The undirected template is essentially responsible for generating the types
 * of verties and edges. Note that the definition of these types also depends,
 * to some degree, on the underlying storage mechanisms. Specifically, these
 * stores contribute information about the specific types of descriptors
 * employed by the adjacency list type.
 *
 * @todo Unfortunately, we are actually instantiating these types over dummy
 * stores in order to access their storage traits - basically deciding whether
 * the descriptor will be an integer or a void ptr. There doesn't really seem
 * to be any other way to do this since we're actually passing the stores as
 * template template parameters - which have no type. Perhaps the new language
 * will offer better solutions.
 */
template <
        typename VertexProps,
        typename EdgeProps,
        typename VertexStore,
        typename EdgeStore,
        template <typename> class VertexEdgeStore
    >
struct undirected
{
    typedef undirected_tag tag;

    typedef vertex_desc<
            typename storage_traits<VertexStore>::descriptor_type
        > vertex_descriptor;

    typedef edge_desc<
            typename storage_traits<EdgeStore>::descriptor_type
        > edge_descriptor;

    typedef undirected_vertex<
            VertexProps, EdgeProps, vertex_descriptor, edge_descriptor, VertexEdgeStore
        > vertex_type;

    typedef undirected_edge<
            VertexProps, EdgeProps, vertex_descriptor, edge_descriptor
        > edge_type;
};

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#include <boost/graphs/adjacency_list/undirected/vertex.hpp>
#include <boost/graphs/adjacency_list/undirected/edge.hpp>

#endif
