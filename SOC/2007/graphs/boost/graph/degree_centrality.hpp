// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_DEGREE_CENTRALITY_HXX
#define BOOST_GRAPH_DEGREE_CENTRALITY_HXX

#include <boost/graph/new_graph_concepts.hpp>

namespace boost
{
    template <typename Graph>
    struct degree_centrality_measure
    {
        BOOST_CLASS_REQUIRE(Graph, boost, IncidenceGraphConcept);

        typedef typename graph_traits<Graph>::degree_size_type degree_type;
        typedef typename graph_traits<Graph>::vertex_descriptor vertex_type;
    };

    template <typename Graph>
    struct influence_measure
        : public degree_centrality_measure<Graph>
    {
        BOOST_CLASS_REQUIRE(Graph, boost, IncidenceGraphConcept);

        typedef degree_centrality_measure<Graph> base_type;
        typedef typename base_type::degree_type degree_type;
        typedef typename base_type::vertex_type vertex_type;

        inline degree_type operator ()(vertex_type v, const Graph& g)
        { return out_degree(v, g); }
    };

    template <typename Graph>
    inline influence_measure<Graph>
    measure_influence(const Graph&)
    { return influence_measure<Graph>(); }


    template <typename Graph>
    struct prestige_measure
        : public degree_centrality_measure<Graph>
    {
        BOOST_CLASS_REQUIRE(Graph, boost, BidirectionalGraphConcept);

        typedef degree_centrality_measure<Graph> base_type;
        typedef typename base_type::degree_type degree_type;
        typedef typename base_type::vertex_type vertex_type;

        inline degree_type operator ()(vertex_type v, const Graph& g)
        { return in_degree(v, g); }
    };

    template <typename Graph>
    inline prestige_measure<Graph>
    measure_prestige(const Graph&)
    { return prestige_measure<Graph>(); }


    template <typename Graph, typename Vertex, typename Measure>
    inline typename Measure::degree_type
    vertex_degree_centrality(const Graph& g,
                             Vertex v,
                             Measure measure)
    {
        function_requires< DegreeMeasureConcept<Measure, Graph> >();
        return measure(v, g);
    }

    template <typename Graph, typename Vertex>
    inline typename graph_traits<Graph>::degree_size_type
    vertex_degree_centrality(const Graph& g,
                             Vertex v)
    {
        return vertex_degree_centrality(g, v, measure_influence(g));
    }


    template <typename Graph, typename CentralityMap, typename Measure>
    inline void
    degree_centrality(const Graph& g,
                      CentralityMap cent,
                      Measure measure)
    {
        function_requires< VertexListGraphConcept<Graph> >();
        typename graph_traits<Graph>::vertex_iterator i, end;
        for(tie(i, end) = vertices(g); i != end; ++i) {
            put(cent, *i, vertex_degree_centrality(g, *i, measure));
        }
    }

    template <typename Graph, typename CentralityMap>
    inline void degree_centrality(const Graph& g,
                                  CentralityMap cent)
    {
        degree_centrality(g, cent, measure_influence(g));
    }
}

#endif


