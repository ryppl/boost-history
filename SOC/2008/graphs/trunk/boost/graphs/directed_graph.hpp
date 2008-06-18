
#ifndef DIRECTED_GRAPH_HPP
#define DIRECTED_GRAPH_HPP

// Notes on directed graphs... Unlike undirected graphs, which are required
// to globally store edge properties, the vertices directed graphs act as
// the stores for nested properties. Edge properties are stored with the
// out edges of each vertex.

// To differentiate from the BGL, which separates the basic concepts of half
// and fully directed graphs, this type implements a fully directed (or
// bidirectional) graph. If you really want a half directed or semidirected
// graph, you can roll your own. This is analagous to the list/slist data
// structures in the std library.

#include "none.hpp"

#include "descriptor.hpp"

#include "directed_vertex.hpp"
#include "vertex_iterator.hpp"
#include "vertex_vector.hpp"

#include "directed_edge.hpp"
#include "edge_vector.hpp"

#include "adjacency_iterator.hpp"

template <
    typename VertexProps,
    typename EdgeProps,
    typename VertexStore,
    typename EdgeStore>
class directed_graph
{
    typedef directed_graph<VertexProps, EdgeProps, VertexStore, EdgeStore> this_type;
public:
    typedef VertexProps vertex_properties;
    typedef EdgeProps edge_properties;
    
    // In order to create the vertex store, we have to create the vertex type.
    // In order to create the vertex type we need to create the edge store
    // types. There are two types of edge store types - one that stores outgoing
    // edges (a pair of vertex and edge property) and another that stores the
    // descriptors of incoming vertices.
    
    // We have to generate the vertex descriptor before we can do anything else.
    typedef typename VertexStore::descriptor_type vertex_descriptor;
    
    // Use the vertex descriptor and edge properties to generate the out edge
    // store for the graph.
    typedef typename EdgeStore::template out_store<vertex_descriptor, edge_properties>::type out_edge_store;
    
    // Like above, but generate the in edge store.
    typedef typename EdgeStore::template in_store<vertex_descriptor, edge_properties>::type in_edge_store;
    
    // Generate the vertex type over the vertex properties, and in/out stores.
    // NOTE: Omitting the in-edge store allows us to create half-directed
    // graphs.
    typedef directed_vertex<vertex_properties, out_edge_store, in_edge_store> vertex_type;
    
    // Finally, use the vertex type to generate the vertex store. This is then
    // used to generate types iteration and size functions.
    typedef typename VertexStore::template store<vertex_type>::type vertex_store;
    typedef typename vertex_store::size_type vertices_size_type;
    typedef typename vertex_store::vertex_iterator vertex_iterator;
    typedef typename vertex_store::vertex_range vertex_range;
    // FIXME: This is a bit hacky, but without constrained members, we need a key
    // type to enable mapped vertices.
    typedef typename VertexStore::key_type vertex_key;

    // Constructors
    directed_graph();

    /** @name Vertex Set
     * These functions operate (mostly) on the vertices of the graph. These
     * functions include the ability to add, disconnect, and remove vertices.
     */
    //@{
    vertex_descriptor add_vertex();
    vertex_descriptor add_vertex(vertex_properties const&);
    vertex_descriptor add_vertex(vertex_key const&, vertex_properties const&);
    //@}

private:
    vertex_store _verts;
};

#define BOOST_GRAPH_DG_PARAMS \
    typename VP, typename EP, typename VS, typename ES

template <BOOST_GRAPH_DG_PARAMS>
directed_graph<VP,EP,VS,ES>::directed_graph()
    : _verts()
{ }

/**
 * Add a vertex to the graph with no or default graph properties. Return a
 * descriptor to the vertex being returned.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::vertex_descriptor
directed_graph<VP,EP,VS,ES>::add_vertex()
{
    return _verts.add();
}

/**
 * Add a vertex to the graph with the given properties and return a descriptor
 * for that vertex. If the graph has labeled, unique vertices, and the given 
 * properties describe a vertex already in the graph, then a new vertex is not 
 * added and the descriptor for the existing vertex is returned.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::vertex_descriptor
directed_graph<VP,EP,VS,ES>::add_vertex(vertex_properties const& vp)
{
    return _verts.add(vp);
}

/**
 * Add a vertex to the graph that is identified by the given key and with the
 * given properties. If the key identifies a vertex already in the graph, do
 * not add a new vertex and return a descriptor to the existing one.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::vertex_descriptor
directed_graph<VP,EP,VS,ES>::add_vertex(vertex_key const& k, vertex_properties const& vp)
{
    return _verts.add(k, vp);
}

#endif
