
#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP


concept Graph<typename G>
{
    Descriptor vertex_descriptor;
    Descriptor edge_descriptor;
};

concept HasUnlabeledVertices<typename G>
{ };

concept HasLabeledVertices<typename G> : Graph<G>
{
    ObjectType vertex_label;

    vertex_label& operator[](G&, vertex_descriptor v);
    vertex_label const& operator[](G const&, vertex_descriptor v);

    vertex_descriptor G::vertex(vertex_label const& l) const;
};

concept HasMappedVertices<typename G> : Graph<G>
{
    ObjectType vertex_label;
    ObjectType vertex_key;

    vertex_label& operator[](G&, vertex_descriptor v);
    vertex_label const& operator[](G const&, vertex_descriptor v);

    vertex_descriptor G::vertex(vertex_key const& k) const;
    vertex_key const& G::key(vertex_descriptor v) const;
};

concept HasUniqueLabeledVertices<typename G> : HasLabeledVertices<G>
{ };

concept HasUniqueMappedVertices<typename G> : HasMappedVertices<G>
{ };

concept HasSortedLabels<typename G> : HasLabeledVertices<G>
{
    StrictWeakOrder<auto, vertex_label> vertex_compare;

    vertex_compare G::vertex_compare_function() const;
};

concept HasSortedKeys<typename G> : HasMappedVertices<G>
{
    StrictWeakOrder<auto, vertex_key> vertex_compare;

    vertex_compare G::vertex_compare_function() const;
};

concept HasHashedLabels<typename G> : HasLabeledVertices<G>
{
    HashFunction<auto, vertex_label> vertex_hash;

    vertex_hash G::vertex_hash_function() const;
};

concept HasHashedKeys<typename G> : HasMappedVertices<G>
{
    HashFunction<auto, vertex_key> vertex_hash;

    vertex_hash G::vertex_hash_function() const;
};

concept HasUnlabeledEdges<typename G>
{ };

concept HasLabeledEdges : Graph<G>
{
    typename edge_label;
    edge_label& operator[](G&, edge_descriptor);
    edge_label const& operator[](G const&, edge_descriptor);
};

concept HasUniqueEdges<typename G> { };

concept HasLoopEdges<typename G> { };

concept HasUndirectedEdges<typename G> : Graph<G>
{
    vertex_descriptor first(edge_descriptor e);
    vertex_descriptor second(edge_descriptor e);
    edge_descriptor make_endpoints(vertex_descriptor u, vertex_descriptor v);

    axiom EndpointEquivalence(vertex_descriptor v, vertex_descriptor u)
    {
        make_edge(u, v) == make_edge(v, u);
    }
};

concept HasDirectedEdges<typename G> : Graph<G>
{
    vertex_descriptor source(edge_descriptor e);
    vertex_descriptor target(edge_descriptor e);
    edge_descriptor make_endpoints(vertex_descriptor u, vertex_descriptor v);

    axiom EndpointInequivalence(vertex_descriptor v, vertex_descriptor u)
    {
        if(u != v) {
            make_edge(u, v) != make_edge(v, u);
        }
    }
};


concept HasFindEdge<typename G> : Graph<G>
{
    edge_descriptor G::edge(vertex_descriptor u, vertex_descriptor v);
};

concept HasFindEdge<typename G> : Graph<G>
{
    requires !HasUniqueEdges<G>;
    ??? edges(vertex_descriptor u, vertex_descriptor v);
};

concept HasAddEdge<typename G> : Graph<G> { };

concept HasAddUnlabeledEdge<typename G> : HasAddEdge<G>
{
    requires HasUnlabeledEdges<G>;
    edge_descriptor add_edge(vertex_descriptor u, vertex_descriptor v)
}


concept HasAddLabeledEdge<typename G> : HasLabeledEdges<G>, HasAddEdge<G>
{
    edge_descriptor G::add_edge(vertex_descriptor u, vertex_descriptor v, edge_label const& l)
}

concept HasAddUniqueUnlabeledEdge : HasUniqueEdges<G>, HasAddUnlabeledEdge<G>
{
    std::pair<edge_descriptor, bool> G::insert_edge(vertex_descriptor u, vertex_descriptor v);
};

concept HasAddUniqueLabeledEdge<typename G> : HasUniqueEdges<G>, HasAddLabeledEdge<G>
{
    std::pair<edge_descriptor, bool> G::insert_edge(vertex_descriptor u,
                                                    vertex_descriptor v,
                                                    edge_label const& l);
};

concept HasRemoveEdge<typename G> : HasAddEdge<G>
{
    void G::remove_edge(edge_descriptor e);
    void G::remove_edges(vertex_descriptor u, vertex_descriptor v);
    void G::remove_edges(vertex_descriptor v);
};

concept HasAddVertex<typename G> { };

cocnept HasAddUnlabeledVertex<typename G> : HasAddVertex<G>
{
    requires HasUnlabeledVertices<G>;
    vertex_descriptor G::add_vertex();
};

concept HasAddLabeledVertex<typename G> : HasLabeledVertices<G>, HasAddVertex<G>
{
    vertex_descriptor G::add_vertex(vertex_label const& l);
};

concept HasAddMappedVertex<typename G> : HasMappedVertices<G>, HasAddVertex<G>
{
    vertex_descriptor G::add_vertex(vertex_key const& k, vertex_label const& l);
};


concept HasAddUniqueLabeledVertex<typename G> : HasAddLabeledVertex<G>
{
    std::pair<vertex_descriptor, bool> G::insert_vertex(vertex_label const& l);
};

concept HasAddUniqueMappedVertex<typename G> : HasAddMappedVertex<G>
{
    std::pair<vertex_descriptor, bool> G::insert_vertex(vertex_key const& k,
                                                        vertex_label const& l);
};

concept HasRemoveVertex<typename G> : HasAddVertex<G>
{
    void G::remove_vertex(vertex_descriptor v);
};

concept VertexListGraph<typename G> : Graph<G>
{
    ForwardIterator vertex_iterator;
    ForwardRange vertex_range;
    UnsignedIntegralLike vertices_size_type;

    vertex_iterator begin_vertices() const;
    vertex_iterator end_vertices() const;
    vertex_range vertices() const;

    vertices_size_type num_vertices() const;
};

concept EdgeListGraph : <typename G> : Graph<G>
{
    ForwardIterator edge_iterator;
    ForwardRange edge_range;
    UnsignedIntegralLike edges_size_type;

    edge_iterator begin_edges() const;
    edge_iterator end_edges() const;
    edge_range edges() const;

    edges_size_type num_edges() const;
};

concept UndirectedGraph<typename G> : Graph<G>
{
    requires HasUndirectedEdges<G>;

    ForwardIterator incident_edge_iterator;
    ForwardRange incident_edge_range;
    UnsignedIntegralLike incident_edges_size_type;

    incident_edge_iterator G::begin_incident_edges(vertex_descriptor v) const;
    incident_edge_iterator G::end_incident_edges(vertex_descriptor v) const;
    incident_edge_range G::incident_edges(vertex_descriptor v);

    incident_edges_size_type degree(vertex_descriptor v);
};

concept OutDirectedGraph<typename G> : Graph<G>
{
    requires HasDirectedEdges<G>;

    ForwardIterator out_edge_iterator;
    ForwardRange out_edge_range;
    UnsignedIntegralLike out_edges_size_type;

    out_edge_iterator G::begin_out_edges(vertex_descriptor v) const;
    out_edge_iterator G::end_out_edges(vertex_descriptor v) const;
    out_edge_range G::out_edges(vertex_descriptor v);

    out_edges_size_type out_degree(vertex_descriptor v);
};

concept InDirectedGraph<typename G> : Graph<G>
{
    requires HasDirectedEdges<G>;

    ForwardIterator in_edge_iterator;
    ForwardRange in_edge_range;
    UnsignedIntegralLike in_edges_size_type;

    in_edge_iterator G::begin_in_edges(vertex_descriptor v) const;
    in_edge_iterator G::end_in_edges(vertex_descriptor v) const;
    in_edge_range G::in_edges(vertex_descriptor v);

    in_edges_size_type in_degree(vertex_descriptor v);
};

concept DirectedGraph<typename G> : OutDirectedGraph<G>, InDirectedGraph<G>
{
    out_edges_size_type degree(vertex_descriptor v);
};

concept IncidenceGraph<typename G>
{
    typename incident_edge_iterator;
    typename incident_edge_range;
    typename incident_edge_size;

    incident_edge_iterator begin_incident_edges(G const& g, vertex_descriptor v);
    incident_edge_interator end_incident_edges(G const& g, vertex_descriptor v);
    incident_edge_rnage incident_edges(G const& g, vertex_descriptor v);
};

concept AdjacencyGraph<typename G> : IncidenceGraph<G>
{
    typename adjacent_vertex_iterator;
    typename adjacent_vertex_range;
    typename adjacent_vertex_size;

    adjacent_vertex_iterator begin_adjacent_vertices(G const& g, vertex_descriptor v);
    adjacent_vertex_interator end_adjacent_vertices(G const& g, vertex_descriptor v);
    adjacent_vertex_rnage adjacent_vertices(G const& g, vertex_descriptor v);
};

concept AdjacencyMatrix<typename G> : HasFindEdge<G>
{ };

concept IncidenceMatrix<typename G>
{ };


#endif
