// This is just a stab at a concept hierarchy for graph. For right now, the
// trend is to require member functions for the graphs. Obviously, this can
// be easily rewritten to free functions, but I prefer the object paradigm to
// the procedural paradigm.

// Recall that a graph is defined as the mathematical object G = (V, E) with
// V being a set of vertices and E a set of edges. While mathematically
// consistent, the meaning of "set" can vary greatly depending on the specific
// graph application. These concepts are intended to provide several alternative
// definitions of the meanings of the term "set".

// Is this overengineering or is it just an extremely thorough analysis of the
// domain?

/**
 * Descriptors are the cornerstone of the graph library. In order to be usable
 * they must be both regular and hashable.
 *
 * The hashability requirements is only really required if you want to use these
 * in hash tables and sets.
 */
concept Descriptor<typename Desc>
    : Regular<Desc>
    , Hashable<Desc>
{ }

/**
 * The base of all graph concepts, this simply requires type names for vertex
 * and edge descriptors. Note that descriptors are regular types.
 */
concept GraphBase<typename Graph>
    : DefaultConstructible<Graph>
    , CopyConstructible<Graph>
{
    Descriptor vertex_descriptor;
    Descriptor edge_descriptor;
};

/**
 * A VertexListGraph provides iterators over the graph's vertex set.
 *
 * The term "list" in the name does not connote storage requirements, but simply
 * expresses the fact that vertices can be accessed in sequential order.
 *
 * Although it may seem that this is a no-brainer and all graphs should support
 * vertex iteration, this not necessarily the case. A sequence of pairs can
 * be used to model a very lightweight graph type that has no explicit vertex
 * iteration capabilities.
 */
concept VertexListGraph<typename Graph>
    : GraphBase<Graph>
{
    // Should this be mutable forward? Probably not since we don't really like
    // modifying the store structure via iterators.
    ForwardIterator vertex_iterator;
    requires SameType<vertex_iterator::dereference_result, vertex_descriptor>;

    // Just for ease of expression.
    typename vertex_range = std::pair<vertex_iterator, vertex_iterator>;

    // Number of vertices.
    typename vertices_size_type;

    vertex_iterator Graph::begin_vertices() const;
    vertex_iterator Graph::end_vertices() const;
    vertex_range Graph::vertices() const;
    vertices_size_type Graph::num_vertices() const;
};

/**
 * An EdgeListGraph provides iterator over the graph's edge set.
 *
 * The term "list" in the name does not connote storage requirements, but simply
 * expresses the fact that edges can be accessed in sequential order.
 */
concept EdgeListGraph<typename Graph>
    : GraphBase<Graph>
{
    // Again, not mutable because we don't use these to modify edge storage.
    ForwardIterator edge_iterator;
    requires SameType<edge_iterator::dereference_result, edge_descriptor>;

    typename edge_range = std::pair<edge_iterator, edge_iterator>;
    typename edges_size_type;

    edge_iterator Graph::begin_edges() const;
    edge_iterator Graph::end_edges() const;
    edge_range Graph::edges() const;
    edges_size_type num_edges() const;
};

/**
 * A PropertyVertexGraph is a graph whose vertices have properties. The graph
 * must therefore provide a method for mapping between a vertex and its
 * properties.
 */
concept PropertyVertexGraph<typename Graph>
    : GraphBase<Graph>
{
    Semiregular vertex_properties;

    vertex_properties& Graph::properties(vertex_descriptor);
    vertex_properties const& Graph::properties() const;
};

/**
 * This concept requires that vertex properties are sortable (i.e., less than
 * comparable).
 */
concept SortableVertexPropertyGraph<typename Graph>
    : PropertyVertexGraph<Graph>
{
    requires LessThanComparable<vertex_properties>
};

/**
 * This concept requires that vertex properties are hashable (i.e., can be
 * hased and are equality comparable).
 */
concept HashableVertexPropertyGraph<typename Graph>
    : PropertyVertexGraph<Graph>
{
    requires Hashable<vertex_properties>;
};

/**
 * A KeyVertexGraph is a graph whose vertices are mapped to a key. Graphs
 * that model this concept must provide a function that maps from a vertex
 * descriptor to its key.
 */
concept KeyVertexGraph<typename Graph>
    : GraphBase<Graph>
{
    typename vertex_key;

    vertex_key Graph::key(vertex_descriptor) const;
};

/**
 * This concept requires that the vertex keys are sortable (i.e., less than
 * comparable.
 */
concept SortableVertexKeyGraph<typename Graph>
    : VertexKeyGraph<Graph>
{
    requires LessThanComparable<vertex_key>;
};

/**
 * This concept requires that the vertex keys are hashable.
 */
concept HashableVertexKeyGraph<typename Graph>
    : VertexKeyGraph<Graph>
{
    requires Hashable<vertex_key>;
};

// We need a couple of empty concepts to group vertex sets and maps because
// the add/remove concepts are orthoganal to the find functions. These conepts
// are analagous to parts of the STL container concepts. Note that any specific
// graph type is probably only exactly one of these.

concept SequentialVertexGraph<typename Graph>
    : GraphBase<Graph>
{ }

concept AssociativeVertexGraph<typename Graph>
{ }

concept SimpleAssociativeVertexGraph<typename Graph>
    : AssociativeVertexGraph<Graph>
    , VertexPropertyGraph<Graph>
{ };

concept PairAssociativeVertexGraph<typename Graph>
    : AssociativeVertexGraph<Graph>
    , VertexKeyGraph<Graph>
{ }

// All of the following essentially describe requirements on storage types
// that enable the efficient lookup of vertices based on either properties
// or some key type.

/**
 * A UniqueVertexGraph is a property graph whose vertex properties comprise a
 * unique identify for that vertex within the graph.
 *
 * This concept does not define the method by which the uniqueness constraint
 * is enforced. That is done by derived vertices.
 *
 * @todo Should this be called VertexSetGraph?
 */
concept VertexSetGraph<typename Graph>
    : SimpleAssociativeVertexGraph<Graph>
{
    vertex_descriptor Graph::find(vertex_properties);
};

/** V == std::set */
concept SortedVertexSetGraph<typename Graph>
    : VeretxSetGraph<Graph>
    , SortableVertexPropertyGraph<Graph>
{ }

/** V == std::unordered_set */
concept HashedVertexSetGraph<typename Graph>
    : VertexSetGraph<Graph>
    , HashableVertexPropertyGraph<Graph>
{ }

/**
 * A UniqueMappedVertexGraph is a property graph whose vertices are mapped
 * one-to-one to a key type. These types of graphs are often used in favor of
 * simple vertex graphs when the vertex properties have many attributes, but
 * only one attribute that is intended to be a key.
 *
 * This graph concept refines a property graph because it makes little sense
 * not to.
 */
concept VertexMapGraph<typename Graph>
    : PairAssociativeVertexGraph<Graph>
{
    typename vertex_key;

    vertex_descriptor Graph::find(vertex_key);
};

/** V == std::map */
concept SortedVertexMapGraph<typename Graph>
    : VertexMapGraph<Graph>
    , SortableVertexPropertyGraph<Graph>
{ }

/** V = std::unordered_map */
concept HashedVertexMapGraph<typename Graph>
    : VertexMapGraph<Graph>
    , SortableVertexPropertyGraph<Graph>
{ }

/**
 * A MultiSimpleVertexGraph is a property graph and vertex list graph that can
 * stores multiple vertices with the same identity. Note this is also a
 * vertex list graph because the find operation returns a range of vertices.
 *
 * I am not entirely sure of the applications of multisets for vertices. There
 * may be some interesting applications since the equality of vertices denotes
 * an implicit relation between them.
 */
concept VertexMultisetGraph<typename Graph>
    : VertexListGraph<Graph>
    , SimpleAssociativeVertexGraph<Graph>
{
    vertex_range Graph::find(vertex_properties);
};

concept SortedVertexMultisetGraph<typename Graph>
    : VertexMultisetGraph<Graph>
    , SortableVertexPropertyGraph<Graph>
{ }

concept HashedVertexMultisetGraph<typename Graph>
    : VertexMultisetGraph<Graph>
    , HashableVertexPropertyGraph<Graph>
{ }

/**
 * A MultiMappedVertexGraph is a property grapbh and vertex list graph that
 * can map multiple vertices to the same key.
 *
 * As with above, I am not certain of the specific applications of multimaps
 * for vertices.
 */
concept VertexMulitmapGraph<typename Graph>
    : VertexListLGraph<Graph>
    , PairAssociativeVertexGraph<Graph>
{
    typename vertex_key;

    vertex_range Graph::find(vertex_key);
};

concept SortedVertexMultisetGraph<typename Graph>
    : VertexMultisetGraph<Graph>
    , SortableVertexPropertyGraph<Graph>
{ }

concept HashedVertexMultisetGraph<typename Graph>
    : VertexMultisetGraph<Graph>
    , HashableVertexPropertyGraph<Graph>
{ }


// The add vertex functionality... These concepts embody graph types that allow
// the programmatic expansion of the graph.
//
// TODO: This is pretty nasty. I'd like a single add_vertex() function that
// takes no parameters, and is available for all graph types. Unfortunately,
// it doesn't make sense for PropertyGraphs or map-like graphs. Besides, you
// can't add an element to a set or map without specific properties although
// you can force them to default values.

/**
 * A base concept for all extendible graphs. This concept does not actually
 * require anything of the graph type and should never be explicitly modeled
 * by any specific graph type. Instead, types should model one of the following
 * concepts.
 */
concept ExtendableGraph<typename Graph>
{
};

/**
 * For sequential graphs with that has no properties. This essentially provides
 * functionality for adding another vertex to the graph.
 */
concept ExtendableSequentialVertexGraph<typename Graph>
    : ExtendableGraph<Graph>
    , SequentialVertexGraph<Graph>
{
    vertex_descriptor Graph::add_vertex();
};

/**
 * For sequential graphs with properties. Note that this is not a refinement
 * of ExtendableSequentialVertexGraph because the vertex properties are not
 * guaranteed to be default constructible (although they almost always are).
 *
 * @todo Consider refinement from the ExtendableSequentialVertexGraph. Again,
 * this would only be supported if
 */
concept ExtendableSequentialVertexPropertyGraph<typename Graph>
    : ExtendableGraph<Graph>
    , SequentialVertexGraph<Graph>
    , VertexPropertyGraph<Graph>
{
    vertex_descriptor Graph::add_vertex(vertex_properties);
};

/**
 * For set-like graphs, provides the ability to add a vertex that is identified
 * by the given properties.
 *
 * Although this supports the same requirements as the previous concept, it has
 * different semantics (and complexity).
 *
 * @todo Consider merging this with the previous function.
 */
concept ExtendableSimpleAssociativeVertexGraph<typename Graph>
    : ExtendableGraph<Graph>
    , SimpleAssociativeVertexGraph<Graph>
{
    vertex_descriptor Graph::add_vertex(vertex_properties);
};

/**
 * For map-like graphs, provides the ability to add a vertex that is identified
 * by the given vertex key.
 */
concept ExtendablePairAssociativeVertexGraph<typename Graph>
    : ExtendableGraph<Graph>
    , PairAssociativeVertexGraph<Graph>
{
    vertex_descriptor Graph::add_vertex(vertex_key)
};

// Remove vertex functionality. Unlike the add

concept ReducibleVertexGraph<typename Graph>
    : GraphBase<Graph>
{
    void Graph::remove_vertex(vertex_descriptor);
};

/**
 * For set-like vertex sets, provides the ability to remove all vertices with
 * the given properties. To remove specific vertices use the remove_vertex()
 * function that takes a descriptor.
 *
 * @todo We could provide a default implementation as remove_vertex(find(p))
 * except that find() returns a range of iterators for multisets and multimaps.
 * We can create more functions like remove_vertices() that takes a range.
 */
concept ReducibleSimpleAssociativeVertexGraph<typename Graph>
    : ReducibleVertexGraph<Graph>
    , SimpleAssociativeVertexGraph<Graph>
{
    void Graph::remove_vertex(vertex_properties);
};

/**
 * For map-like vertex sets, provides the ability to remove all vertices with
 * the given key. To remove specific vertices use the remove_vertex() function
 * that takes a given descriptor.
 */
concept ReduciblePairAssociativeVertexGraph<typename Graph>
    : ReducibleVertexGraph<Graph>
    , PairAssociativeVertexGraph<Graph>
{
    void Graph::remove_vertex(vertex_key);
};

// I guess we could ultimately end up with somethig like this.

concept Graph<typename G>
    : AssociativeVertexGraph<G>
    , EdgeSetGraph<G>
{
};

concept Multigraph<G>
    : AssociativeVertexGraph<G>
    , EdgeMultisetGraph<G>
{ }


