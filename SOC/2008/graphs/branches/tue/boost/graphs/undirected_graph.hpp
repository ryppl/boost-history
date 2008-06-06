
#ifndef UNDIRECTED_GRAPH_HPP
#define UNDIRECTED_GRAPH_HPP

#include "none.hpp"

// Various issues...
// * Undirected graphs have a global property store.
// * Global property stores are lists (for node-based edge stores) and
//   vectors (for vector edge stores).
// * Edge descriptors are a triple: an unordered pair consisting of
//   vertex descriptors, and an a property descriptor.
// * The property descriptor provide access to the properties of the
//   given edge. This would best be described as the edge property
//   accessors.
// * If there are no properties, then the property store doesn't really
//   need to exist. We can just pretend to allocate them and return
//   an integer "pseudo descriptor".

#include "vertex.hpp"
#include "vertex_vector.hpp"
#include "vertex_list.hpp"

#include "descriptor.hpp"
#include "edge_vector.hpp"
#include "edge_list.hpp"
#include "edge_set.hpp"

template <
    typename VertexProps,
    typename EdgeProps,
    typename VertexStore,
    typename EdgeStore>
class undirected_graph
{
public:
    typedef VertexProps vertex_properties;
    typedef EdgeProps edge_properties;

    // Generate the property store type first. We can do this first because
    // it's basically independant of everything else, but contributes to almost
    // everything in the class by virtue of the property descriptor.
    typedef typename EdgeStore::template property_store<edge_properties>::type property_store;

    // Generate a bunch of descriptors. The vertex descriptor is fairly
    // straightforward since, like the property store, its independant of almost
    // everything. The property descriptor depends entirely upon the property
    // store and the edge descriptor is actually fairly complicated.
    typedef typename VertexStore::descriptor_type vertex_descriptor;
    typedef typename property_store::property_descriptor property_descriptor;
    typedef undirected_edge_descriptor<vertex_descriptor, property_descriptor> edge_descriptor;

    // Generate the incidence list. The incidence list for a single vertex
    // contains a pair: the opposite edge and a property descriptor.
    typedef typename EdgeStore::template incidence_store<vertex_descriptor, property_descriptor>::type incidence_store;
    typedef typename incidence_store::size_type incidence_size_type;

    // Generate the vertex type over the given properties and the incidence
    // store. Then, turn around and use that to generate the vertex store and its
    // related types.
    typedef vertex<vertex_properties, incidence_store> vertex_type;
    typedef typename VertexStore::template store<vertex_type>::type vertex_store;
    typedef typename vertex_store::size_type vertices_size_type;

    // FIXME: This is a bit hacky, but without constrained members, we need a key
    // type to enable mapped vertices.
    typedef typename VertexStore::key_type key_type;

    // Constructors
    undirected_graph();

    // Add vertices
    vertex_descriptor add_vertex();
    vertex_descriptor add_vertex(vertex_properties const&);
    vertex_descriptor add_vertex(key_type const&, vertex_properties const&);

    // Add edges
    edge_descriptor add_edge(vertex_descriptor, vertex_descriptor);
    edge_descriptor add_edge(vertex_descriptor, vertex_descriptor, edge_properties const&);

    vertices_size_type num_vertices() const;

    incidence_size_type degree() const;

private:
    property_store _props;
    vertex_store _verts;
};

#define BOOST_GRAPHS_UG_PARAMS \
    typename VP, typename EP, typename VS, typename ES

template <BOOST_GRAPHS_UG_PARAMS>
undirected_graph<VP,EP,VS,ES>::undirected_graph()
    : _props()
    , _verts()
{ }

template <BOOST_GRAPHS_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::vertex_descriptor
undirected_graph<VP,EP,VS,ES>::add_vertex()
{
    return _verts.add();
}

template <BOOST_GRAPHS_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::vertex_descriptor
undirected_graph<VP,EP,VS,ES>::add_vertex(vertex_properties const& vp)
{
    return _verts.add(vp);
}

template <BOOST_GRAPHS_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::edge_descriptor
undirected_graph<VP,EP,VS,ES>::add_edge(vertex_descriptor u, vertex_descriptor v)
{
    // Start by getting a property descriptor for the edge.
    property_descriptor p = _props.add();
    vertex_type& vert = _verts.vertex(u);
    vert.connect(v, p);

    return edge_descriptor();
}

template <BOOST_GRAPHS_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::vertices_size_type
undirected_graph<VP,EP,VS,ES>::num_vertices() const
{ 
    return _verts.size(); 
}

template <BOOST_GRAPHS_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::incidence_size_type
undirected_graph<VP,EP,VS,ES>::degree(vertex_descriptor v) const
{
}

#undef BOOST_GRAPHS_UG_PARAMS

#endif

