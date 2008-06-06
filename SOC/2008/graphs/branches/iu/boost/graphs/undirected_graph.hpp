
#ifndef UNDIRECTED_GRAPH_HPP
#define UNDIRECTED_GRAPH_HPP

#include <boost/bind.hpp>

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

#include "descriptor.hpp"

#include "vertex.hpp"
#include "vertex_iterator.hpp"
#include "vertex_vector.hpp"
#include "vertex_list.hpp"
#include "vertex_set.hpp"

#include "edge.hpp"
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
    typedef undirected_edge<vertex_descriptor, property_descriptor> edge_descriptor;

    // Generate the incidence list. The incidence list for a single vertex
    // contains a pair: the opposite edge and a property descriptor.
    typedef typename EdgeStore::template incidence_store<vertex_descriptor, property_descriptor>::type incidence_store;
    typedef typename incidence_store::size_type incident_edges_size_type;
    typedef typename incidence_store::incidence_iterator incident_edge_iterator;

    // Generate the vertex type over the given properties and the incidence
    // store. Then, turn around and use that to generate the vertex store and its
    // related types.
    typedef vertex<vertex_properties, incidence_store> vertex_type;
    typedef typename VertexStore::template store<vertex_type>::type vertex_store;
    typedef typename vertex_store::size_type vertices_size_type;
    typedef typename vertex_store::vertex_iterator vertex_iterator;
    typedef typename vertex_store::vertex_range vertex_range;

    // FIXME: This is a bit hacky, but without constrained members, we need a key
    // type to enable mapped vertices.
    typedef typename VertexStore::key_type key_type;

    // Constructors
    undirected_graph();

    // Add vertices
    vertex_descriptor add_vertex();
    vertex_descriptor add_vertex(vertex_properties const&);
    vertex_descriptor add_vertex(key_type const&, vertex_properties const&);

    // Remove vertices
    void remove_vertex(vertex_descriptor);

    // Add edges
    edge_descriptor add_edge(vertex_descriptor, vertex_descriptor);
    edge_descriptor add_edge(vertex_descriptor, vertex_descriptor, edge_properties const&);

    void remove_edge(edge_descriptor);
    void remove_edges(vertex_descriptor, vertex_descriptor);

    vertex_range vertices() const;
    vertex_iterator begin_vertices() const;
    vertex_iterator end_vertices() const;
    vertices_size_type num_vertices() const;

    incident_edges_size_type degree(vertex_descriptor) const;

    vertex_properties& operator[](vertex_descriptor);
    edge_properties& operator[](edge_descriptor);

private:
    property_store _props;
    vertex_store _verts;
};

#define BOOST_GRAPH_UG_PARAMS \
    typename VP, typename EP, typename VS, typename ES

template <BOOST_GRAPH_UG_PARAMS>
undirected_graph<VP,EP,VS,ES>::undirected_graph()
    : _props()
    , _verts()
{ }

template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::vertex_descriptor
undirected_graph<VP,EP,VS,ES>::add_vertex()
{
    return _verts.add();
}

template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::vertex_descriptor
undirected_graph<VP,EP,VS,ES>::add_vertex(vertex_properties const& vp)
{
    return _verts.add(vp);
}

/**
 * Remove the vertex from the graph. This will disconnect the vertex from the
 * graph prior to remove.
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::remove_vertex(vertex_descriptor v)
{
    _verts.remove(v);
}

/**
 * Create an edge, connecting the vertices u and v.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::edge_descriptor
undirected_graph<VP,EP,VS,ES>::add_edge(vertex_descriptor u, vertex_descriptor v)
{
    // Start by getting a property descriptor for the edge.
    property_descriptor p = _props.add();
    vertex_type& src = _verts.vertex(u);
    vertex_type& tgt = _verts.vertex(v);
    src.connect(v, p);
    tgt.connect(u, p);

    return edge_descriptor(u, v, p);
}

/**
 * Create an edge with the given properties that connects the vertices u and v.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::edge_descriptor
undirected_graph<VP,EP,VS,ES>::add_edge(vertex_descriptor u,
                                        vertex_descriptor v,
                                        edge_properties const& ep)
{
    // Start by getting a property descriptor for the edge.
    property_descriptor p = _props.add(ep);
    vertex_type& src = _verts.vertex(u);
    vertex_type& tgt = _verts.vertex(v);
    src.connect(v, p);
    tgt.connect(u, p);

    return edge_descriptor(u, v, p);
}

/**
 * Remove only the given edge from graph. This disconnects both vertices in
 * the edge and removes the property from the graph.
 *
 * @requires HasRemove<edge_store>
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::remove_edge(edge_descriptor e)
{
    // Cache some useful info for the erasure.
    property_descriptor p = e.property();
    vertex_descriptor u = e.edge().first();
    vertex_descriptor v = e.edge().second();
    vertex_type& src = _verts.vertex(u);
    vertex_type& tgt = _verts.vertex(v);

    // Disconnect the incidence ends and then remove the property from
    // the global property store.
    src.disconnect(v, p);
    tgt.disconnect(u, p);
    _props.remove(p);
}

/**
 * This removes all distinct edges connecting the vertices u and v.
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::remove_edges(vertex_descriptor u,
                                            vertex_descriptor v)
{
    using boost::bind;
    using boost::ref;

    // The implementation of this function is... not pretty because of the
    // number of efficient ways to do this for both lists, sets and maps.
    // Remember that we have to remove the same basic edge structures from
    // both source and target, but only need to remove the global properties
    // once.

    vertex_type& src = _verts.vertex(u);
    vertex_type& tgt = _verts.vertex(v);

    // Disconnect v from the src, removing global properties,. Then disconnect
    // u from tgt, but don't actually do anything with the properties (they're
    // already gone!).
    src.disconnect(v, bind(&property_store::remove, ref(_props), _1));
    tgt.disconnect(u, bind(noop<property_descriptor>(), _1));
}

/**
 * Return an iterator range over the vertices of the graph.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::vertex_range
undirected_graph<VP,EP,VS,ES>::vertices() const
{
    return _verts.vertices();
}

/**
 * Return an iterator to the first iterator in the graph.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::vertex_iterator
undirected_graph<VP,EP,VS,ES>::begin_vertices() const
{
    return _verts.begin_vertices();
}

/**
 * Return an iterator past the end of the vertices in the graph.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::vertex_iterator
undirected_graph<VP,EP,VS,ES>::end_vertices() const
{
    return _verts.end_vertices();
}

/**
 * Return the number of iterators in this graph.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::vertices_size_type
undirected_graph<VP,EP,VS,ES>::num_vertices() const
{
    return _verts.size();
}

/**
 * Return the degree (number of incdent edges) of the given vertex.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::incident_edges_size_type
undirected_graph<VP,EP,VS,ES>::degree(vertex_descriptor v) const
{
    return _verts.vertex(v).degree();
}

/**
 * Return the properties for the given vertex.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::vertex_properties&
undirected_graph<VP,EP,VS,ES>::operator[](vertex_descriptor v)
{
    return _verts.properties(v);
}

/**
 * Return the properties for the given edge.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::edge_properties&
undirected_graph<VP,EP,VS,ES>::operator[](edge_descriptor e)
{
    return _props.properties(e.prop);
}

#undef BOOST_GRAPH_UG_PARAMS

#endif

