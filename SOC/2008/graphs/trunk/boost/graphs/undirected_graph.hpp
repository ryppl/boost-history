
#ifndef UNDIRECTED_GRAPH_HPP
#define UNDIRECTED_GRAPH_HPP

#include "none.hpp"

#include "descriptor.hpp"

#include "undirected_vertex.hpp"
#include "vertex_iterator.hpp"
#include "vertex_vector.hpp"
#include "vertex_list.hpp"
#include "vertex_set.hpp"
#include "vertex_map.hpp"

#include "undirected_edge.hpp"
#include "edge_vector.hpp"
#include "edge_list.hpp"
#include "edge_set.hpp"

#include "adjacency_iterator.hpp"

template <
    typename VertexProps,
    typename EdgeProps,
    typename VertexStore,
    typename EdgeStore>
class undirected_graph
{
    typedef undirected_graph<VertexProps, EdgeProps, VertexStore, EdgeStore> this_type;
public:
    typedef VertexProps vertex_properties;
    typedef EdgeProps edge_properties;

    // Generate the property store type first. We can do this first because
    // it's basically independant of everything else, but contributes to almost
    // everything in the class by virtue of the property descriptor.
    typedef typename EdgeStore::template property_store<edge_properties>::type property_store;
    typedef typename property_store::size_type edges_size_type;

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

    // Generate the vertex type over the given properties and the incidence
    // store. Then, turn around and use that to generate the vertex store and its
    // related types. Incident edge iterators are abstracted over the iterators
    // of the vertex.
    typedef undirected_vertex<vertex_properties, incidence_store> vertex_type;
    typedef typename vertex_type::size_type incident_edges_size_type;
    typedef incidence_iterator<typename vertex_type::iterator> incident_edge_iterator;
    typedef std::pair<incident_edge_iterator, incident_edge_iterator> incident_edge_range;

    typedef adjacency_iterator<incident_edge_iterator> adjacent_vertex_iterator;
    typedef std::pair<adjacent_vertex_iterator, adjacent_vertex_iterator> adjacent_vertex_range;

    // The vertex store and related properties can also be built on the vertex
    // type.
    typedef typename VertexStore::template store<vertex_type>::type vertex_store;
    typedef typename vertex_store::size_type vertices_size_type;
    typedef typename vertex_store::vertex_iterator vertex_iterator;
    typedef typename vertex_store::vertex_range vertex_range;
    // FIXME: This is a bit hacky, but without constrained members, we need a key
    // type to enable mapped vertices.
    typedef typename VertexStore::key_type vertex_key;


    // Constructors
    undirected_graph();

    /** @name Add Vertex
     * Add a vertex to the graph. Add operations are determined by the concepts
     * modeled by the vertex set.
     *
     * UnlabeledVertices        add_vertex()
     * LabeledVerteces          add_vertex(vertex_properties)
     * LabeledUniqueVertices    add_vertex(vertex_properties)
     * MappedUniqueVertices     add_vertex(vertex_key)
     */
    //@{
    vertex_descriptor add_vertex();
    vertex_descriptor add_vertex(vertex_properties const&);
    vertex_descriptor add_vertex(vertex_key const&, vertex_properties const&);
    //@}

    /** @name Find Vertex
     * Find a vertex in the graph. These methods only exist for graphs that
     * have UniqueVertices. These functions can also be used to find the first
     * vertex of a non-unique vertices.
     *
     * LabeledUniqueVertices    find_vertex(vertex_properties)
     * MappedUniqueVertices     find_vertex(vertex_key)
     */
    //@{
    vertex_descriptor find_vertex(vertex_properties const&) const;
    vertex_descriptor find_vertex(vertex_key const&) const;
    //@{

    /** @name Disconnect Vertex
     * Disconnect a vertex from the graph by removing all of its incident edges.
     * These functions only exist for graphs with ReducibleEdgeSets. Functions
     * that take properties or keys are provided for convenience, but have
     * additional dependencies and cost. These additonal functions are
     * equivalent to disconnect_vertex(find_vertex(x)) where x is either a
     * vertex_properties or vertex_key.
     *
     * ReducibleEdgeSet         disconnect_vertex(vertex_descriptor)
     * LabeledUniqueVertices    disconnect_vertex(vertex_properties)
     * MappedUniqueVertices     disconnect_vertex(vertex_key)
     */
    //@{
    void disconnect_vertex(vertex_descriptor);
    void disconnect_vertex(vertex_properties const&);
    void disconnect_vertex(vertex_key const&);
    //@}

    /** @name Remove Vertex
     * Remove a vertex from the graph. These functions only exist for graphs
     * with ReducibleVertexSets. Functions that take properties or keys are
     * provided for convenience, but have additional requirements and cost.
     * These additional functions are equivalent to remove_vertex(find_vertex(x))
     * where x is either a vertex_properties or vertex_key.
     *
     * ReducibleVertexSet       remove_vertex(vertex_descriptor)
     * LabeledUniqueVertices    remove_vertex(vertex_properties)
     * MappedUniqueVertices     remove_vertex(vertex_key)
     */
    //@{
    void remove_vertex(vertex_descriptor);
    void remove_vertex(vertex_properties const&);
    void remove_vertex(vertex_key const&);
    //@}

    /** @name Vertex Key
     * Return the key for the given vertex. This is only provided for graphs
     * with MappedVertices (can be multimapped).
     */
    //@{
    vertex_key const& key(vertex_descriptor) const;
    //@{

    /** @name Edge Set
     * These functions operate on the edges of the graph. This functions
     * include the ability to add and remove edges.
     */
    //@{
    edge_descriptor add_edge(vertex_descriptor, vertex_descriptor);
    edge_descriptor add_edge(vertex_descriptor, vertex_descriptor, edge_properties const&);
    void remove_edge(edge_descriptor);
    void remove_edges(vertex_descriptor, vertex_descriptor);
    //@}

    /** @name Vertex Iteration
     * These functions allow iteration over the vertex set.
     */
    //@{
    vertex_range vertices() const;
    vertex_iterator begin_vertices() const;
    vertex_iterator end_vertices() const;
    vertices_size_type num_vertices() const;
    //@}

    /** @name Edge Iteration
     * These function allow iteration over the edge set.
     */
    //@{
    // edge_range edges() const;
    // edge_iterator begin_edges() const;
    // edge_iterator end_edges() const;
    edges_size_type num_edges() const;
    //@}

    /** @name Incident Edge Iteration
     * These functions allow iteration over the incident edges of a vertex.
     */
    //@{
    incident_edge_iterator begin_incident_edges(vertex_descriptor) const;
    incident_edge_iterator end_incident_edges(vertex_descriptor) const;
    incident_edge_range incident_edges(vertex_descriptor) const;

    adjacent_vertex_iterator begin_adjacent_vertices(vertex_descriptor) const;
    adjacent_vertex_iterator end_adjacent_vertices(vertex_descriptor) const;
    adjacent_vertex_range adjacent_vertices(vertex_descriptor) const;

    incident_edges_size_type degree(vertex_descriptor) const;
    //@{

    /** @name Property Accessors
     * Access the properties of the given vertex or edge.
     */
    //@{
    vertex_properties& operator[](vertex_descriptor);
    edge_properties& operator[](edge_descriptor);
    //@{

private:
    property_store _props;
    vertex_store _verts;

    std::size_t _edges;
};

#define BOOST_GRAPH_UG_PARAMS \
    typename VP, typename EP, typename VS, typename ES

template <BOOST_GRAPH_UG_PARAMS>
undirected_graph<VP,EP,VS,ES>::undirected_graph()
    : _props()
    , _verts()
{ }

/**
 * Add a vertex to the graph with no or default properties, and return a
 * descriptor to the vertex. Although supported for all adjacency lists, this
 * function should not be used with graphs that have LabeledUniqueVertices
 * as it will always return the same default-propertied vertex iterator.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::vertex_descriptor
undirected_graph<VP,EP,VS,ES>::add_vertex()
{
    // BOOST_STATIC_ASSERT(!LabeledUniqueVertices<this_type>);
    return _verts.add();
}

/**
 * Add a vertex to the graph with the given properties, and return a descriptor
 * to the added vertes. If the graph has LabeledUniqe vertices, and a vertex
 * with the given properties already exists in the graph, then a new vertex
 * is not added and returned descriptor refers to the existing vertex.
 *
 * @requires LabeledVertices<Graph>
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::vertex_descriptor
undirected_graph<VP,EP,VS,ES>::add_vertex(vertex_properties const& vp)
{
    return _verts.add(vp);
}

/**
 * Add a new vertex with the given properties to the graph and map it to the
 * given key.
 *
 * @requires MappedUniqueVertices<Graph>
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::vertex_descriptor
undirected_graph<VP,EP,VS,ES>::add_vertex(vertex_key const& k, vertex_properties const& vp)
{
    return _verts.add(k, vp);
}

/**
 * Find the vertex with the given properties, returning a descriptor to it.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::vertex_descriptor
undirected_graph<VP,EP,VS,ES>::find_vertex(vertex_properties const& vp) const
{
    BOOST_STATIC_ASSERT(is_not_none<vertex_properties>::value);
    return _verts.find(vp);
}

/**
 * Find the vertex with the given properties, returning a descriptor to it.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::vertex_descriptor
undirected_graph<VP,EP,VS,ES>::find_vertex(vertex_key const& k) const
{
    return _verts.find(k);
}

/**
 * Disconnect the vertex from the graph. This removes all edges incident to
 * the vertex, but will not remove the vertex itself.
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::disconnect_vertex(vertex_descriptor v)
{
    // Disconnecting a vertex is not quite so simple as clearing the incidence
    // set since we have to remove all the edge properties and remove the
    // opposite edges from other vertices.
    // TODO: Can we specialize this at all?

    // Start by disconnecting all of the incident edges from adjacent vertices.
    incident_edge_range rng = incident_edges(v);
    for( ; rng.first != rng.second; ++rng.first) {
        edge_descriptor e = *rng.first;
        vertex_type& opp = _verts.vertex(e.opposite(v));
        opp.disconnect(v, e.properties());

        // Remove all the properties too. Does this make sense here?
        _props.remove(e.properties());
    }

    // Clear the incident edge set of the vertex. We don't do this in the
    // previous loop because we'll probably end up invalidating our own
    // iterators.
    _verts.vertex(v).disconnect();
}

/**
 * Disconnect the vertex having the given properties from the graph.
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::disconnect_vertex(vertex_properties const& vp)
{
    BOOST_STATIC_ASSERT(is_not_none<vertex_properties>::value);
    disconnect_vertex(find_vertex(vp));
}

/**
 * Disconnect the vertex having the given key from the graph.
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::disconnect_vertex(vertex_key const& k)
{
    disconnect_vertex(find_vertex(k));
}

/**
 * Remove the vertex from the graph. This will disconnect the vertex from the
 * graph prior to remove.
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::remove_vertex(vertex_descriptor v)
{
    disconnect_vertex(v);
    _verts.remove(v);
}

/**
 * Remove the vertex from the graph identified by the given properties.
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::remove_vertex(vertex_properties const& vp)
{
    BOOST_STATIC_ASSERT(is_not_none<vertex_properties>::value);
    disconnect_vertex(vp);
    _verts.remove(vp);
}

/**
 * Remove the vertex from the graph identified by the given key.
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::remove_vertex(vertex_key const& k)
{
    disconnect_vertex(k);
    _verts.remove(k);
}

/**
 * Return the key associated with the given vertex. This function is only
 * available for graphs with mapped vertices.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::vertex_key const&
undirected_graph<VP,EP,VS,ES>::key(vertex_descriptor v) const
{
    return _verts.key(v);
}


/**
 * Create an edge, connecting the vertices u and v.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::edge_descriptor
undirected_graph<VP,EP,VS,ES>::add_edge(vertex_descriptor u,
                                        vertex_descriptor v)
{
    return add_edge(u, v, edge_properties());
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
    typedef typename incidence_store::const_iterator inc_iterator;

    // To add the edge or not... We need to consult the virtual edge set
    // to determine whether or not this edge already exists. For multigraph
    // stores, this should always return false. The protocol is: ask the source
    // if it can be connected to the target. If so, connect them. If they're
    // connected, return the existing edge. If they can't be connected, return
    // a null descriptor.
    vertex_type& src = _verts.vertex(u);
    vertex_type& tgt = _verts.vertex(v);

    std::pair<inc_iterator, bool> ins = src.allow(v);
    if(ins.second) {
        // If the returned iterator is past the end, then we need to add this
        // edge. Otherwise, we can simply return an edge over the existing
        // iterator.
        if(ins.first == src.end()) {
            property_descriptor p = _props.add(ep);
            src.connect(v, p);
            tgt.connect(u, p);
            return edge_descriptor(u, v, p);
        }
        else {
            return edge_descriptor(u, v, ins.first->second);
        }
    }
    else {
        // Can't add the edge?
    }

    // This is a null iterator
    return edge_descriptor();
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
    // Grab descriptors out of the edge.
    property_descriptor p = e.properties();
    vertex_descriptor u = e.first();
    vertex_descriptor v = e.second();

    // And translate to real data structres.
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
    vertex_type& src = _verts.vertex(u);
    vertex_type& tgt = _verts.vertex(v);

    // The implementation of this function is... not pretty because of the
    // number of efficient ways to do this for both lists, sets and maps.
    // Remember that we have to remove the same basic edge structures from
    // both source and target, but only need to remove the global properties
    // once.
    //
    // Disconnect v from the src, removing global properties. Then disconnect
    // u from tgt, but don't actually do anything with the properties (they're
    // already gone!).
    src.disconnect(v, property_eraser<property_store>(_props));
    tgt.disconnect(u, noop_eraser());
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
 * Return the number of edges in this graph.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::edges_size_type
undirected_graph<VP,EP,VS,ES>::num_edges() const
{
    return _props.size();
}

/**
 * Return an iterator to the first incident edge of the given vertex.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::incident_edge_iterator
undirected_graph<VP,EP,VS,ES>::begin_incident_edges(vertex_descriptor v) const
{
    return incident_edge_iterator(v, _verts.vertex(v).begin());
}

/**
 * Return an iterator past the end of the incident edges of the given vertex.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::incident_edge_iterator
undirected_graph<VP,EP,VS,ES>::end_incident_edges(vertex_descriptor v) const
{
    return incident_edge_iterator(v, _verts.vertex(v).end());
}

/**
 * Return an iterator range over the incident edges of the given vertex.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::incident_edge_range
undirected_graph<VP,EP,VS,ES>::incident_edges(vertex_descriptor v) const
{
    return make_pair(begin_incident_edges(v), end_incident_edges(v));
}

/**
 * Return an iterator to the first adjacent vertex of the the given vertex.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::adjacent_vertex_iterator
undirected_graph<VP,EP,VS,ES>::begin_adjacent_vertices(vertex_descriptor v) const
{
    return begin_incident_edges(v);
}

/**
 * Return an iterator past the end of the adjacent vertices of the given vertex.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::adjacent_vertex_iterator
undirected_graph<VP,EP,VS,ES>::end_adjacent_vertices(vertex_descriptor v) const
{
    return end_incident_edges(v);
}

/**
 * Return an iterator range over the adjacent vertices of the given vertex.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::adjacent_vertex_range
undirected_graph<VP,EP,VS,ES>::adjacent_vertices(vertex_descriptor v) const
{
    return std::make_pair(begin_adjacent_vertices(v), end_adjacent_vertices(v));
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
    return _props.properties(e.properties());
}

#undef BOOST_GRAPH_UG_PARAMS

#include "traits.hpp"

#endif

