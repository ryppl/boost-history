
#ifndef UNDIRECTED_GRAPH_HPP
#define UNDIRECTED_GRAPH_HPP

#include <boost/assert.hpp>
#include <boost/none.hpp>

#include <boost/graphs/undirected_types.hpp>
#include <boost/graphs/adjacency_iterator.hpp>

template <
    typename VertexProps,
    typename EdgeProps,
    typename VertexStore,
    typename EdgeStore>
class undirected_graph
{
    typedef undirected_types<VertexProps, EdgeProps, VertexStore, EdgeStore> types;
    typedef undirected_graph<VertexProps, EdgeProps, VertexStore, EdgeStore> this_type;
public:
    typedef VertexProps vertex_properties;
    typedef EdgeProps edge_properties;
    typedef VertexStore vertex_store_selector;
    typedef EdgeStore edge_store_selector;

    // Underlying stores
    typedef typename types::property_store property_store;
    typedef typename types::vertex_store vertex_store;
    typedef typename types::vertex_key vertex_key;
    typedef typename types::vertex_type vertex_type;

    // Vertex/Property/Edge descriptors
    typedef typename types::vertex_descriptor vertex_descriptor;
    typedef typename types::edge_descriptor edge_descriptor;
    typedef typename types::property_descriptor property_descriptor;
    typedef typename types::incidence_descriptor incidence_descriptor;

    // Iterators and ranges
    typedef typename types::vertex_iterator vertex_iterator;
    typedef typename types::vertex_range vertex_range;
    typedef typename types::edge_iterator edge_iterator;
    typedef typename types::edge_range edge_range;
    typedef typename types::incident_edge_iterator incident_edge_iterator;
    typedef typename types::incident_edge_range incident_edge_range;
    typedef typename types::adjacent_vertex_iterator adjacent_vertex_iterator;
    typedef typename types::adjacent_vertex_range adjacent_vertex_range;

    // Sizes for vertices, edges, and degree.
    typedef typename types::vertices_size_type vertices_size_type;
    typedef typename types::edges_size_type edges_size_type;
    typedef typename types::incident_edges_size_type incident_edges_size_type;


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
    //@}

    /** @name Vertex Key
     * Return the key for the given vertex. This is only provided for graphs
     * with MappedVertices (can be multimapped).
     */
    //@{
    vertex_key const& key(vertex_descriptor) const;
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

    /** @name Add Edge
     * Add an edge, connecting two vertices, to the graph. There are a number of
     * variations of this function, depending on the type of vertex store and
     * whether or not the edges are labeled. Convenience functions are provided
     * for graphs with UniqueVertices. Graphs with LabeledEdges can be added
     * with edge properties. Convenience functions are equivalent to the
     * expression add_edge(find_vertex(x), find_vertex(y), p) with x and y
     * eithe vertex proeprties or vertex keys and p optional edge properties.
     *
     * ExtendableEdgeSet        add_edge(vertex_descriptor, vertex_descriptor)
     *   && LabeledEdges        add_edge(vertex_descriptor, vertex_descriptor, edge_properties)
     *
     * LabeledUniqueVertices    add_edge(vertex_properties, vertex_properties)
     *   && LabeledEdges        add_edge(vertex_properties, vertex_properties, edge_properties)
     *
     * MappedUniqueVertices     add_edge(vertex_key, vertex_key)
     *   & LabeledEdges         add_edge(vertex_key, vertex_key, edge_properties)
     */
    //@{
    // Unlabeled edge adders.
    edge_descriptor add_edge(vertex_descriptor, vertex_descriptor);
    edge_descriptor add_edge(vertex_properties const&, vertex_properties const&);
    edge_descriptor add_edge(vertex_key const&, vertex_key const&);

    // Labeled edge adders.
    edge_descriptor add_edge(vertex_descriptor, vertex_descriptor, edge_properties const&);
    edge_descriptor add_edge(vertex_properties const&, vertex_properties const&, edge_properties const&);
    edge_descriptor add_edge(vertex_key const&, vertex_key const&, edge_properties const&);
    //@}

    /** @name Test Edge
     * Determine if the edge, given by two vertices exists. This function a few
     * convenience overloads that depend on the type of vertex store.
     */
    //@{
    std::pair<edge_descriptor, bool> edge(vertex_descriptor, vertex_descriptor) const;
    std::pair<edge_descriptor, bool> edge(vertex_properties const&, vertex_properties const&) const;
    std::pair<edge_descriptor, bool> edge(vertex_key const&, vertex_key const&) const;
    //@}

    /** @name Remove Edge(s)
     * Remove one or more edges from the graph. The function taking a single
     * edge descriptor removes exactly that edge. The fucntion(s) taking
     * a single vertex descriptor remove all edges incident to that vertex. The
     * function(s) taking two vertices remove all edges connecting the two
     * vertices.
     */
    //@{
    void remove_edge(edge_descriptor);

    void remove_edges(vertex_descriptor);
    void remove_edges(vertex_properties const&);
    void remove_edges(vertex_key const&);

    void remove_edges(vertex_descriptor, vertex_descriptor);
    void remove_edges(vertex_properties const&, vertex_properties const&);
    void remove_edges(vertex_key const&, vertex_key const&);
    //@}

    /** @name Size and Degree */
    //@{
    vertices_size_type num_vertices() const;
    edges_size_type num_edges() const;
    incident_edges_size_type degree(vertex_descriptor) const;
    //@}

    /** @name Iterators */
    //@{
    vertex_range vertices() const;
    vertex_iterator begin_vertices() const;
    vertex_iterator end_vertices() const;

    edge_range edges() const;
    edge_iterator begin_edges() const;
    edge_iterator end_edges() const;

    incident_edge_iterator begin_incident_edges(vertex_descriptor) const;
    incident_edge_iterator end_incident_edges(vertex_descriptor) const;
    incident_edge_range incident_edges(vertex_descriptor) const;

    adjacent_vertex_iterator begin_adjacent_vertices(vertex_descriptor) const;
    adjacent_vertex_iterator end_adjacent_vertices(vertex_descriptor) const;
    adjacent_vertex_range adjacent_vertices(vertex_descriptor) const;
    //@}

    /** @name Property Accessors
     * Access the properties of the given vertex or edge.
     */
    //@{
    vertex_properties& operator[](vertex_descriptor);
    edge_properties& operator[](edge_descriptor);
    //@}

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
 * Remove the vertex from the graph. This will disconnect the vertex from the
 * graph prior to remove.
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::remove_vertex(vertex_descriptor v)
{
    remove_edges(v);
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
    remove_edges(vp);
    _verts.remove(vp);
}

/**
 * Remove the vertex from the graph identified by the given key.
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::remove_vertex(vertex_key const& k)
{
    remove_edges(k);
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
    // Get vertices for the descriptors
    reorder(u, v);
    vertex_type& src = _verts.vertex(u);
    vertex_type& tgt = _verts.vertex(v);

    // Insert algorithm: Try to stub out the edge locally first. If it succeeds,
    // then add the global property, and finally bind the incidence and property
    // descitpros into their respective "slots". Note that this is actually
    // faster than the
    insertion_result<incidence_descriptor> first = src.connect(v);
    if(first.succeeded()) {
        // Stub the incident edge on the other vertex.
        insertion_result<incidence_descriptor> second = tgt.connect(u);
        BOOST_ASSERT(second.succeeded());

        // Add the property and bind everything together.
        property_descriptor p = _props.add(ep);
        src.bind(first.value, p);
        tgt.bind(second.value, p);
        _props.bind(p, make_pair(u, v));
        return edge_descriptor(u, v, p);
    }
    else if(first.retained()) {
        property_descriptor p = src.edge_properties(first.value);
        return edge_descriptor(u, v, p);
    }
    else {
        return edge_descriptor();
    }
}

/**
 * Add an edge to the graph that connects the two vertices identified by the
 * given properties. The edge is either unlabeled or has default properties.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::edge_descriptor
undirected_graph<VP,EP,VS,ES>::add_edge(vertex_properties const& u,
                                        vertex_properties const& v)
{
    return add_edge(u, v, edge_properties());
}

/**
 * Add an edge to the graph that connects the two vertices identified by the
 * given properties and has the given edge properties.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::edge_descriptor
undirected_graph<VP,EP,VS,ES>::add_edge(vertex_properties const& u,
                                        vertex_properties const& v,
                                        edge_properties const& ep)
{
    return add_edge(find_vertex(u), find_vertex(v), ep);
}

/**
 * Add an edge to the graph that connects the two vertices identified by the
 * given keys. The edge is either unlabeled or has default properties.
 */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::edge_descriptor
undirected_graph<VP,EP,VS,ES>::add_edge(vertex_key const& u,
                                        vertex_key const& v)
{
    return add_edge(u, v, edge_properties());
}

/**
 * Determine if any edge exists connecting the vertices u and v. Return a
 * pair containing the edge descriptor (if it exists) and a bool determing
 * whether or not the edge did exist.
 */
template <BOOST_GRAPH_UG_PARAMS>
std::pair<typename undirected_graph<VP,EP,VS,ES>::edge_descriptor, bool>
undirected_graph<VP,EP,VS,ES>::edge(vertex_descriptor u, vertex_descriptor v) const
{
    reorder(u, v);
    vertex_type const& src = _verts.vertex(u);
    typename vertex_type::iterator i = src.find(v);
    return i != src.end() ?
        std::make_pair(edge_descriptor(u, v, i->second), true) :
        std::make_pair(edge_descriptor(), false);
}

/**
 * Determine if any edge exists connecting the vertices identified by the given
 * properties. Return a pair containing the edge descriptor (if it exists) and
 * a bool determining whether or not the edge did exist. This is equivalent to
 * edge(find_vertex(u), find_vertex(v)).
 */
template <BOOST_GRAPH_UG_PARAMS>
std::pair<typename undirected_graph<VP,EP,VS,ES>::edge_descriptor, bool>
undirected_graph<VP,EP,VS,ES>::edge(vertex_properties const& u,
                                    vertex_properties const& v) const
{
    return edge(find_vertex(u), find_vertex(v));
}

/**
 * Determine if any edge exists connecting the vertices identified by the given
 * keys. Return a pair containing the edge descriptor (if it exists) and a bool
 * determining whether or not the edge did exist. This is equivalent to the
 * expression edge(find_vertex(u), find_vertex(v)).
 */
template <BOOST_GRAPH_UG_PARAMS>
std::pair<typename undirected_graph<VP,EP,VS,ES>::edge_descriptor, bool>
undirected_graph<VP,EP,VS,ES>::edge(vertex_key const& u,
                                    vertex_key const& v) const
{
    return edge(find_vertex(u), find_vertex(v));
}

/**
 * Remove only the given edge from graph. This disconnects both vertices in
 * the edge and removes the property from the graph.
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::remove_edge(edge_descriptor e)
{
    /*
    // Grab descriptors out of the edge.
    property_descriptor p = e.properties();
    vertex_descriptor u = e.first();
    vertex_descriptor v = e.second();

    // And translate to real data structres.
    vertex_type& src = _verts.vertex(u);
    vertex_type& tgt = _verts.vertex(v);

    // Disconnect the incidence ends and then remove the property from the
    // global property store.
    tgt.disconnect(u, p);
    src.disconnect(v, p);
    _props.remove(p);
    */
}

/**
 * Remove all edges incident to the given vertex, effectively disconnecting
 * it from the graph.
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::remove_edges(vertex_descriptor v)
{
    /*
    // Disconnecting a vertex is not quite so simple as clearing the incidence
    // set since we have to remove all the edge properties and remove the
    // opposite edges from other vertices.
    // TODO: Can we specialize this at all?

    vertex_type& src = _verts.vertex(v);

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
    src.clear();
    */
}

/**
 * Disconnect the vertex having the given properties from the graph.
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::remove_edges(vertex_properties const& vp)
{
    BOOST_STATIC_ASSERT(is_not_none<vertex_properties>::value);
    remove_edges(find_vertex(vp));
}

/**
 * Disconnect the vertex having the given key from the graph.
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::remove_edges(vertex_key const& k)
{
    remove_edges(find_vertex(k));
}

/**
 * Remove all edges connecting the vertices u and v.
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::remove_edges(vertex_descriptor u,
                                            vertex_descriptor v)
{
    /*
    // Canonicalize the ordering of vertices first and the get the vertices.
    reorder(u, v);
    vertex_type& src = _verts.vertex(u);
    vertex_type& tgt = _verts.vertex(v);

    // Iterate over the incident edges of the source vertex.
    typename vertex_type::iterator i = src.begin(), end = src.end();
    for( ; i != end; ) {
        vertex_descriptor o = i->first;
        property_descriptor p = i->second;
        // If this is the opposite end of the edge, then we need to remove this
        // from a) the incidence store of the opposite vertex and b) the global
        // edge property.
        if(i->first == v) {
            tgt.disconnect(u, p);
            _props.remove(p);
            i = src.disconnect(i);
        }
        else {
            ++i;
        }
    }
    */
}

/**
 * Remove all edges connecting the vertices identified by the given properties.
 * This is equivalent to remove_edges(find_vertex(u), find_vertex(v)).
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::remove_edges(vertex_properties const& u,
                                            vertex_properties const& v)
{
    return remove_edges(find_vertex(u), find_vertex(v));
}

/**
 * Remove all edges connecting the vertices identified by the given keys. This
 * is equivalent to remove_edges(find_vertex(u), find_vertex(v)).
 */
template <BOOST_GRAPH_UG_PARAMS>
void
undirected_graph<VP,EP,VS,ES>::remove_edges(vertex_key const& u,
                                            vertex_key const& v)
{
    return remove_edges(find_vertex(u), find_vertex(v));
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

/** Return an iterator to the first edge. */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::edge_iterator
undirected_graph<VP,EP,VS,ES>::begin_edges() const
{ return edge_iterator(_props, _props.begin()); }

/** Return an iterator past the end of the edges. */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::edge_iterator
undirected_graph<VP,EP,VS,ES>::end_edges() const
{ return edge_iterator(_props, _props.end()); }

/** Return an iterator range over the edges in the graph. */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::edge_range
undirected_graph<VP,EP,VS,ES>::edges() const
{ return std::make_pair(begin_edges(), end_edges()); }

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

/** Return an iterator to the first incident edge of the given vertex. */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::incident_edge_iterator
undirected_graph<VP,EP,VS,ES>::begin_incident_edges(vertex_descriptor v) const
{ return incident_edge_iterator(v, _verts.vertex(v).begin()); }

/** Return an iterator past the end of the incident edges of the given vertex. */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::incident_edge_iterator
undirected_graph<VP,EP,VS,ES>::end_incident_edges(vertex_descriptor v) const
{ return incident_edge_iterator(v, _verts.vertex(v).end()); }

/** Return an iterator range over the incident edges of the given vertex. */
template <BOOST_GRAPH_UG_PARAMS>
typename undirected_graph<VP,EP,VS,ES>::incident_edge_range
undirected_graph<VP,EP,VS,ES>::incident_edges(vertex_descriptor v) const
{ return make_pair(begin_incident_edges(v), end_incident_edges(v)); }

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

#endif

