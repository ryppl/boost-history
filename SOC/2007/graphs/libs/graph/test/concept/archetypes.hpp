// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_GRAPH_CONCEPT_CHECK_ARCHETYPES_HPP
#define BOOST_GRAPH_CONCEPT_CHECK_ARCHETYPES_HPP

#include "new_archetypes.hpp"

namespace boost
{
    // VertexIndexGraph
    template <typename GraphArchetype>
    struct vertex_index_graph_archetype
        : public property_graph_archetype<GraphArchetype, vertex_index_t, unsigned>
    {
        typedef unsigned vertex_index_type;
        typedef property_graph_archetype<GraphArchetype, vertex_index_t, vertex_index_type> BaseType;
        typedef typename BaseType::vertex_property_type vertex_property_type;
        typedef void edge_property_type;
        typedef typename BaseType::graph_tag graph_tag;
    };

    template <typename G>
    void
    renumber_vertex_indices(vertex_index_graph_archetype<G>&)
    { }

    // EdgeIndexGraph
    template <typename GraphArchetype>
    struct edge_index_graph_archetype
        : public property_graph_archetype<GraphArchetype, edge_index_t, unsigned>
    {
        typedef unsigned edge_index_type;
        typedef property_graph_archetype<GraphArchetype, edge_index_t, edge_index_type> BaseType;
        typedef void vertex_property_type;
        typedef typename BaseType::edge_property_type edge_property_type;
        typedef typename BaseType::graph_tag graph_tag;
    };

    template <typename G>
    void
    renumber_edge_indices(edge_index_graph_archetype<G>&)
    { }

    // DegreeMeasure
    template <typename Graph>
    struct degree_measure_archetype
    {
        typedef typename graph_traits<Graph>::degree_size_type degree_type;
        typedef typename graph_traits<Graph>::vertex_descriptor vertex_type;
        degree_type operator ()(vertex_type, const Graph&)
        { return degree_type(); }
    };

    template <typename Graph, typename Distance, typename Result>
    struct distance_measure_archetype
    {
        typedef Distance distance_type;
        typedef Result result_type;
        result_type operator ()(distance_type, const Graph&)
        { return result_type(); }
    };

    struct cycle_visitor_archetype
    {
        template <typename Path, typename Graph>
        void cycle(const Path&, const Graph&)
        { }
    };

    struct clique_visitor_archetype
    {
        template <typename VertexSet, typename Graph>
        void clique(const VertexSet&, const Graph&)
        { }
    };
}

#endif
