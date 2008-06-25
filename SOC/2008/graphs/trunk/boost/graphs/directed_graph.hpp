
#ifndef DIRECTED_GRAPH_HPP
#define DIRECTED_GRAPH_HPP

#include <boost/static_assert.hpp>

// Notes on directed graphs... Unlike directed graphs, which are required
// to globally store edge properties, the vertices directed graphs act as
// the stores for nested properties. Edge properties are stored with the
// out edges of each vertex.

// To differentiate from the BGL, which separates the basic concepts of half
// and fully directed graphs, this type implements a fully directed (or
// bidirectional) graph. If you really want a half directed or semidirected
// graph, you can roll your own. This is analagous to the list/slist data
// structures in the std library.

// Note that directed graphs maintain an independent edge count since the actual
// count would basically require a search.

// Apparently directed graphs can be challenging - especially when you're
// talking about in and out edges. Let's start with the basics - out edges. Each
// out edge of a vertex references a target vertex descriptor and the properties
// of that edge. However, there is an additional in edge that needs to be
// accounted for (in another vertex no less). We could augment each out edge
// with an iterator to its corresponding in edge.
//
// Each in edge is similarly defined. Minimally, this could contain only the
// source vertex and a reference to the properties that define that edge. In
// order to provide quick access to it "other half", it needs a reference to
// the out edge (which could be implemented as an iterator of some kind).

#include "none.hpp"

#include "directed_vertex.hpp"
#include "vertex_vector.hpp"
#include "vertex_list.hpp"
#include "vertex_set.hpp"
#include "vertex_map.hpp"

#include "directed_edge.hpp"
#include "edge_vector.hpp"
#include "edge_list.hpp"
#include "edge_set.hpp"

#include "out_iterator.hpp"
#include "in_iterator.hpp"

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
    typedef VertexStore vertex_store_selector;
    typedef EdgeStore edge_store_selector;

    // In order to create the vertex store, we have to create the vertex type.
    // In order to create the vertex type we need to create the edge store
    // types. There are two types of edge store types - one that stores outgoing
    // edges (a pair of vertex and edge property) and another that stores the
    // descriptors of incoming vertices.

    // We have to generate the vertex descriptor before we can do anything else.
    typedef typename VertexStore::descriptor_type vertex_descriptor;

    // Use the vertex descriptor and edge properties to generate the out and in
    // edge stores for the graph. Get the out edge descriptor type from the out
    // store.
    typedef typename EdgeStore::template out_store<vertex_descriptor, edge_properties>::type out_edge_store;
    typedef typename EdgeStore::template in_store<vertex_descriptor>::type in_edge_store;

    // We can now generate the edge descriptor.
    typedef directed_edge<typename out_edge_store::iterator, typename in_edge_store::iterator> edge_descriptor;

    // Generate the vertex type over the vertex properties, and in/out stores.
    // We can also pull size
    // NOTE: Omitting the in-edge store allows us to create half-directed
    // graphs.
    typedef directed_vertex<vertex_properties, out_edge_store, in_edge_store> vertex_type;
    typedef typename vertex_type::out_size_type out_edges_size_type;
    typedef typename vertex_type::in_size_type in_edges_size_type;
    typedef typename vertex_type::incident_size_type incident_edges_size_type;

    // Finally, use the vertex type to generate the vertex store. This is then
    // used to generate types iteration and size functions.
    typedef typename VertexStore::template store<vertex_type>::type vertex_store;
    typedef typename vertex_store::size_type vertices_size_type;
    typedef typename vertex_store::vertex_iterator vertex_iterator;
    typedef typename vertex_store::vertex_range vertex_range;

    // Additional iterator types. I'm cheating here and using the edge
    // descriptor to piggyback type information to these iterators. It kinda
    // works.
    typedef basic_out_iterator<edge_descriptor> out_edge_iterator;
    typedef std::pair<out_edge_iterator, out_edge_iterator> out_edge_range;
    typedef basic_in_iterator<edge_descriptor> in_edge_iterator;
    typedef std::pair<in_edge_iterator, in_edge_iterator> in_edge_range;

    // This just makes sense.
    typedef std::size_t edges_size_type;

    // FIXME: This is a bit hacky, but without constrained members, we need a key
    // type to enable mapped vertices.
    typedef typename VertexStore::key_type vertex_key;


    // Constructors
    directed_graph();

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
    edge_descriptor add_edge(vertex_descriptor, vertex_descriptor);
    edge_descriptor add_edge(vertex_properties const&, vertex_properties const&);
    edge_descriptor add_edge(vertex_key const&, vertex_key const&);

    edge_descriptor add_edge(vertex_descriptor, vertex_descriptor, edge_properties const&);
    edge_descriptor add_edge(vertex_properties const&, vertex_properties const&, edge_properties const&);
    edge_descriptor add_edge(vertex_key const&, vertex_key const&, edge_properties const&);
    //@}

    /** @name Test Edge
     * Determine if the edge, given by two vertices exists. This function a few
     * convenience overloads that depend on the type of vertex store.
     */
    //@{
    std::pair<edge_descriptor, bool> edge(vertex_descriptor, vertex_descriptor);
    std::pair<edge_descriptor, bool> edge(vertex_properties const&, vertex_properties const&);
    std::pair<edge_descriptor, bool> edge(vertex_key const&, vertex_key const&);
    //@}

    /** @name Remove Edge(s)
     * Remove one or more edges from the graph. The function taking a single
     * edge descriptor removes exactly that edge. The fucntion(s) taking
     * a single vertex descriptor remove all edges incident to (both in and
     * out edges) that vertex. The function(s) taking two vertices remove all
     * edges connecting the two vertices. The so-called half-remove functions
     * remove only the outgoing and incoming edges from the given vertex.
     */
    //@{
    void remove_edge(edge_descriptor e);

    void remove_edges(vertex_descriptor);
    void remove_edges(vertex_properties const&);
    void remove_edges(vertex_key const&);

    void remove_out_edges(vertex_descriptor);
    void remove_out_edges(vertex_properties const&);
    void remove_out_edges(vertex_key const&);

    void remove_in_edges(vertex_descriptor);
    void remove_in_edges(vertex_properties const&);
    void remove_in_edges(vertex_key const&);

    void remove_edges(vertex_descriptor, vertex_descriptor);
    void remove_edges(vertex_properties const&, vertex_properties const&);
    void remove_edges(vertex_key const&, vertex_key const&);
    //@}

    /** @name Size and Degree
     * Return the in/out or cumulative degree of the given vertex.
     */
    //@{
    vertices_size_type num_vertices() const;
    edges_size_type num_edges() const;
    out_edges_size_type out_degree(vertex_descriptor v) const;
    in_edges_size_type in_degree(vertex_descriptor v) const;
    incident_edges_size_type degree(vertex_descriptor v) const;
    //@}

    /** @name Out Edge Iterator */
    //@{
    out_edge_iterator begin_out_edges(vertex_descriptor) const;
    out_edge_iterator end_out_edges(vertex_descriptor) const;
    out_edge_range out_edges(vertex_descriptor) const;
    //@}

    /** @name In Edge Iterator */
    //@{
    in_edge_iterator begin_in_edges(vertex_descriptor) const;
    in_edge_iterator end_in_edges(vertex_descriptor) const;
    in_edge_range in_edges(vertex_descriptor) const;
    //@}

    /** @name Property Accessors
     * Access the properties of the given vertex or edge.
     */
    //@{
    vertex_properties& operator[](vertex_descriptor);
    edge_properties& operator[](edge_descriptor);
    //@}

private:
    vertex_store        _verts;
    edges_size_type     _edges;
};

#define BOOST_GRAPH_DG_PARAMS \
    typename VP, typename EP, typename VS, typename ES

template <BOOST_GRAPH_DG_PARAMS>
directed_graph<VP,EP,VS,ES>::directed_graph()
    : _verts()
    , _edges(0)
{ }

/**
 * Add a vertex to the graph with no or default graph properties, and return a
 * descriptor to the vertex being returned. This function should not be used
 * with graphs that have LabeledUniqueVertices since each addition will return
 * the same default-propertied vertex.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::vertex_descriptor
directed_graph<VP,EP,VS,ES>::add_vertex()
{
    // BOOST_STATIC_ASSERT(!LabeledUniqueVertices<this_type>);
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

/**
 * Find the vertex with the given properties, returning a descriptor to it.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::vertex_descriptor
directed_graph<VP,EP,VS,ES>::find_vertex(vertex_properties const& vp) const
{
    BOOST_STATIC_ASSERT(is_not_none<vertex_properties>::value);
    return _verts.find(vp);
}

/**
 * Find the vertex with the given properties, returning a descriptor to it.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::vertex_descriptor
directed_graph<VP,EP,VS,ES>::find_vertex(vertex_key const& k) const
{
    return _verts.find(k);
}

/**
 * Remove the vertex from the graph. This will disconnect the vertex from the
 * graph prior to remove.
 */
template <BOOST_GRAPH_DG_PARAMS>
void
directed_graph<VP,EP,VS,ES>::remove_vertex(vertex_descriptor v)
{
    remove_edges(v);
    _verts.remove(v);
}

/**
 * Remove the vertex from the graph identified by the given properties.
 */
template <BOOST_GRAPH_DG_PARAMS>
void
directed_graph<VP,EP,VS,ES>::remove_vertex(vertex_properties const& vp)
{
    BOOST_STATIC_ASSERT(is_not_none<vertex_properties>::value);
    remove_edges(vp);
    _verts.remove(vp);
}

/**
 * Remove the vertex from the graph identified by the given key.
 */
template <BOOST_GRAPH_DG_PARAMS>
void
directed_graph<VP,EP,VS,ES>::remove_vertex(vertex_key const& k)
{
    remove_edges(k);
    _verts.remove(k);
}

/**
 * Return the key associated with the given vertex. This function is only
 * available for graphs with mapped vertices.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::vertex_key const&
directed_graph<VP,EP,VS,ES>::key(vertex_descriptor v) const
{
    return _verts.key(v);
}

/**
 * Add an edge, connecting the two vertices to the graph with default or no
 * properties. The edge is added to both the in and out edge stores of the
 * target and source vertices respectively.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::edge_descriptor
directed_graph<VP,EP,VS,ES>::add_edge(vertex_descriptor u, vertex_descriptor v)
{
    return add_edge(u, v, edge_properties());
}

/**
 * Add an edge, connecting the two vertices to the graph with the given
 * properties. The edge is added to both the in and out edge stores of the
 * target and source vertices respectively.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::edge_descriptor
directed_graph<VP,EP,VS,ES>::add_edge(vertex_descriptor u,
                                      vertex_descriptor v,
                                      edge_properties const& ep)
{
    typedef typename vertex_type::out_iterator out_iterator;
    typedef typename vertex_type::in_iterator in_iterator;

    vertex_type &src = _verts.vertex(u);
    vertex_type &tgt = _verts.vertex(v);

    // Do we add the edge or not?
    std::pair<out_iterator, bool> ins = src.allow(v);
    if(ins.second) {
        // If the returned iterator is past the end, then we have to create and
        // connect the edge.
        if(ins.first == src.end_out()) {
            ++_edges;

            // Insert the edge stubs, getting iterators to each stub.
            out_iterator i = src.connect_target(v, ep);
            in_iterator j = tgt.connect_source(u);
            edge_descriptor e = vertex_type::bind_connection(i, j);
            return e;
        }
        else {
            return edge_descriptor(u, ins.first);
        }
    }
    else {
        // Can't add the edge? This is a flat refusal (as in a loop).
    }

    return edge_descriptor();
}

/**
 * Add an edge to the graph that connects the two vertices identified by the
 * given properties. The edge is either unlabeled or has default properties.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::edge_descriptor
directed_graph<VP,EP,VS,ES>::add_edge(vertex_properties const& u,
                                      vertex_properties const& v)
{
    return add_edge(u, v, edge_properties());
}

/**
 * Add an edge to the graph that connects the two vertices identified by the
 * given properties and has the given edge properties.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::edge_descriptor
directed_graph<VP,EP,VS,ES>::add_edge(vertex_properties const& u,
                                      vertex_properties const& v,
                                      edge_properties const& ep)
{
    return add_edge(find_vertex(u), find_vertex(v), ep);
}

/**
 * Add an edge to the graph that connects the two vertices identified by the
 * given keys. The edge is either unlabeled or has default properties.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::edge_descriptor
directed_graph<VP,EP,VS,ES>::add_edge(vertex_key const& u,
                                      vertex_key const& v)
{
    return add_edge(u, v, edge_properties());
}

/**
 * Add an edge to the graph that connects the two vertices identified by the
 * given keys and has the given edge properties.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::edge_descriptor
directed_graph<VP,EP,VS,ES>::add_edge(vertex_key const& u,
                                      vertex_key const& v,
                                      edge_properties const& ep)
{
    return add_edge(find_vertex(u), find_vertex(v), ep);
}

/**
 * Remove the edge connecting the two vertices. This removes only the given
 * edge. Other edges connecting the two vertices remain.
 */
template <BOOST_GRAPH_DG_PARAMS>
void
directed_graph<VP,EP,VS,ES>::remove_edge(edge_descriptor e)
{
    // Removing the edge involves the removal of both parts from the two
    // connected vertices.
    vertex_type& src = _verts.vertex(e.source());
    vertex_type& tgt = _verts.vertex(e.target());
    src.disconnect_target(e);
    tgt.disconnect_source(e);
}

/**
 * Remove all edges incident to this vertex. This will remove all edges in
 * which the given vertex appears as either a source or target, effectively
 * disconnecting the vertex from the graph. This is equivalent to calling
 * remove_out_edges(v) and removing_in_edges(v).
 */
template <BOOST_GRAPH_DG_PARAMS>
void
directed_graph<VP,EP,VS,ES>::remove_edges(vertex_descriptor v)
{
    remove_out_edges(v);
    remove_in_edges(v);
}

/**
 * Remove all edges incident to the vertex identified by the given properties.
 * This is equivalent to remove_edges(find_vertex(vp));
 */
template <BOOST_GRAPH_DG_PARAMS>
void
directed_graph<VP,EP,VS,ES>::remove_edges(vertex_properties const& vp)
{
    BOOST_STATIC_ASSERT(is_not_none<vertex_properties>::value);
    remove_edges(find_vertex(vp));
}

/**
 * Remove all edges incident to the vertex identified by the given key. This is
 * equivalent to remove_edges(find_vertex(k)).
 */
template <BOOST_GRAPH_DG_PARAMS>
void
directed_graph<VP,EP,VS,ES>::remove_edges(vertex_key const& k)
{
    remove_edges(find_vertex(k));
}

/**
 * Remove all out edges of the given vertex.
 */
template <BOOST_GRAPH_DG_PARAMS>
void
directed_graph<VP,EP,VS,ES>::remove_out_edges(vertex_descriptor v)
{
    // Basically, just iterate over the out edges of v and remove all of the
    // incoming parts of each out edge. Don't forget to drop the edge count.
    out_edge_range out = out_edges(v);
    for( ; out.first != out.second; ++out.first) {
        edge_descriptor e = *out.first;
        vertex_type& tgt = _verts.vertex(e.target());
        tgt.disconnect_source(e);
        --_edges;
    }

    // Clear the out edges.
    _verts.vertex(v).clear_out();
}

/**
 * Remove all out edges of the vertex identified by the given properties. This
 * is equivalent to remove_out_edges(find_vertex(vp));
 */
template <BOOST_GRAPH_DG_PARAMS>
void
directed_graph<VP,EP,VS,ES>::remove_out_edges(vertex_properties const& vp)
{
    BOOST_STATIC_ASSERT(is_not_none<vertex_properties>::value);
    remove_out_edges(find_vertex(vp));
}

/**
 * Remove all out edges incident to the vertex identified by the given key. This
 * is equivalent to remove_out_edges(find_vertex(k)).
 */
template <BOOST_GRAPH_DG_PARAMS>
void
directed_graph<VP,EP,VS,ES>::remove_out_edges(vertex_key const& k)
{
    remove_out_edges(find_vertex(k));
}

/**
 * Remove all in edges of the given vertex.
 */
template <BOOST_GRAPH_DG_PARAMS>
void
directed_graph<VP,EP,VS,ES>::remove_in_edges(vertex_descriptor v)
{
    // we have to disconnect the source of the opposite edge. Don't forget to
    // drop the edge count.
    in_edge_range in = in_edges(v);
    for( ; in.first != in.second; ++in.first) {
        edge_descriptor e = *in.first;
        vertex_type& src = _verts.vertex(e.source());
        src.disconnect_target(e);
        --_edges;
    }

    // Clear out the vertices lists.
    _verts.vertex(v).clear_in();
}

/**
 * Remove all in edges of the vertex identified by the given properties. This
 * is equivalent to remove_in_edges(find_vertex(vp));
 */
template <BOOST_GRAPH_DG_PARAMS>
void
directed_graph<VP,EP,VS,ES>::remove_in_edges(vertex_properties const& vp)
{
    BOOST_STATIC_ASSERT(is_not_none<vertex_properties>::value);
    remove_in_edges(find_vertex(vp));
}

/**
 * Remove all out edges incident to the vertex identified by the given key. This
 * is equivalent to remove_in_edges(find_vertex(k)).
 */
template <BOOST_GRAPH_DG_PARAMS>
void
directed_graph<VP,EP,VS,ES>::remove_in_edges(vertex_key const& k)
{
    remove_in_edges(find_vertex(k));
}

/**
 * Remove all edges connecting the vertex u to v. This function only removes
 * the edges with u as a source and v as a target.
 *
 * To remove all edges interconnecting u and v, first call remove_edges(u, v)
 * followed by remove_edges(v, u).
 */
template <BOOST_GRAPH_DG_PARAMS>
void
directed_graph<VP,EP,VS,ES>::remove_edges(vertex_descriptor u, vertex_descriptor v)
{
    vertex_type& src = _verts.vertex(u);
    vertex_type& tgt = _verts.vertex(v);

    // Iterate over the out edges of the source, removing them.
    out_edge_range rng = out_edges(u);
    for(out_edge_iterator i = rng.first ; i != rng.second; ) {
        // If the target vertex is one that we want to remove, remove it from
        // the out list of this vertex. Also, brute-force remove it from the
        // in list of other vertex.
        edge_descriptor e = *i;
        if(v == e.target()) {
            tgt.disconnect_source(e.in_edge());
            i = out_edge_iterator(u, src.disconnect_target(e.out_edge()));
            --_edges;
       }
       else {
            ++i;
       }
    }
}

template <BOOST_GRAPH_DG_PARAMS>
void
directed_graph<VP,EP,VS,ES>::remove_edges(vertex_properties const& u,
                                          vertex_properties const& v)
{
    remove_vertices(find_vertex(u), find_vertex(v));
}

template <BOOST_GRAPH_DG_PARAMS>
void
directed_graph<VP,EP,VS,ES>::remove_edges(vertex_key const& u,
                                          vertex_key const& v)
{
    remove_vertices(find_vertex(u), find_vertex(v));
}


/**
 * Test to see if the given edge exists. Return a pair containing the edge
 * descriptor (if it exists), and a boolean value indicating whether it actually
 * exists or not.
 *
 * @todo Consider making descriptors "self-invalidating".
 */
template <BOOST_GRAPH_DG_PARAMS>
std::pair<typename directed_graph<VP,EP,VS,ES>::edge_descriptor, bool>
directed_graph<VP,EP,VS,ES>::edge(vertex_descriptor u, vertex_descriptor v)
{
    vertex_type& src = _verts.vertex(u);
    typename vertex_type::out_iterator i = src.find_out(v);
    return i != src.end_out() ?
        std::make_pair(edge_descriptor(u, i), true) :
        std::make_pair(edge_descriptor(), false);
}

/**
 * Test to see if at least one edge connects the two vertices identified by
 * the given properties. Return a pair containing a descriptor to the first such
 * edge (if it exists), and a boolean value indicating whether it actually
 * exists or not. This is equivalent to edge(find_vertex(u), find_vertex(v)).
 */
template <BOOST_GRAPH_DG_PARAMS>
std::pair<typename directed_graph<VP,EP,VS,ES>::edge_descriptor, bool>
directed_graph<VP,EP,VS,ES>::edge(vertex_properties const& u,
                                  vertex_properties const& v)
{
    return edge(find_vertex(u), find_vertex(v));
}
/**
 * Test to see if at least one edge connects the two vertices identified by
 * the given key. Return a pair containing a descriptor to the first such
 * edge (if it exists), and a boolean value indicating whether it actually
 * exists or not. This is equivalent to edge(find_vertex(u), find_vertex(v)).
 */
template <BOOST_GRAPH_DG_PARAMS>
std::pair<typename directed_graph<VP,EP,VS,ES>::edge_descriptor, bool>
directed_graph<VP,EP,VS,ES>::edge(vertex_key const& u,
                                  vertex_key const& v)
{
    return edge(find_vertex(u), find_vertex(v));
}

/**
 * Return the number of vertices in the graph.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::vertices_size_type
directed_graph<VP,EP,VS,ES>::num_vertices() const
{
    return _verts.size();
}

/**
 * Return the number of edges in the graph.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::edges_size_type
directed_graph<VP,EP,VS,ES>::num_edges() const
{
    return _edges;
}


/**
 * Return the out degree of the given vertex.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::out_edges_size_type
directed_graph<VP,EP,VS,ES>::out_degree(vertex_descriptor v) const
{
    return _verts.vertex(v).out_degree();
}

/**
 * Return the in degree of the given vertex.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::in_edges_size_type
directed_graph<VP,EP,VS,ES>::in_degree(vertex_descriptor v) const
{
    return _verts.vertex(v).in_degree();
}

/**
 * Return the degree of the given vertex. This is simply the sum of the in
 * and out degree of the vertex.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::incident_edges_size_type
directed_graph<VP,EP,VS,ES>::degree(vertex_descriptor v) const
{
    return _verts.vertex(v).degree();
}

/**
 * Return an iterator to the first out edge of the given vertex.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::out_edge_iterator
directed_graph<VP,EP,VS,ES>::begin_out_edges(vertex_descriptor v) const
{
    return out_edge_iterator(v, _verts.vertex(v).begin_out());
}

/**
 * Return an iterator past the end of then out edges of the given vertex.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::out_edge_iterator
directed_graph<VP,EP,VS,ES>::end_out_edges(vertex_descriptor v) const
{
    return out_edge_iterator(v, _verts.vertex(v).end_out());
}

/**
 * Return awn iterator range over the out edges of the given vertex.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::out_edge_range
directed_graph<VP,EP,VS,ES>::out_edges(vertex_descriptor v) const
{
    return std::make_pair(begin_out_edges(v), end_out_edges(v));
}

/**
 * Return an iterator to the first in edge of the given vertex.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::in_edge_iterator
directed_graph<VP,EP,VS,ES>::begin_in_edges(vertex_descriptor v) const
{
    return in_edge_iterator(v, _verts.vertex(v).begin_in());
}

/**
 * Return an iterator past the end of then in edges of the given vertex.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::in_edge_iterator
directed_graph<VP,EP,VS,ES>::end_in_edges(vertex_descriptor v) const
{
    return in_edge_iterator(v, _verts.vertex(v).end_in());
}

/**
 * Return an iterator range over the in edges of the given vertex.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::in_edge_range
directed_graph<VP,EP,VS,ES>::in_edges(vertex_descriptor v) const
{
    return std::make_pair(begin_in_edges(v), end_in_edges(v));
}

/**
 * Return the properties for the given vertex.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::vertex_properties&
directed_graph<VP,EP,VS,ES>::operator[](vertex_descriptor v)
{
    return _verts.properties(v);
}

/**
 * Return the properties for the given edge.
 */
template <BOOST_GRAPH_DG_PARAMS>
typename directed_graph<VP,EP,VS,ES>::edge_properties&
directed_graph<VP,EP,VS,ES>::operator[](edge_descriptor e)
{
    return e.properties();
}

#undef BOOST_GRAPH_DG_PARAMS

#endif
