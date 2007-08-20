// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_NEW_GRAPH_CONCEPTS_HXX
#define BOOST_GRAPH_NEW_GRAPH_CONCEPTS_HXX

#include <boost/property_map.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/numeric_values.hpp>

#include <boost/concept/detail/concept_def.hpp>
namespace boost
{
    namespace concepts
    {
        BOOST_concept(VertexIndexGraph,(Graph))
        {
            BOOST_CONCEPT_USAGE(VertexIndexGraph)
            {
                // TODO: This relaxes the constraints so that the only
                // thing it actually requires is access to the property map
                // and indices.
                typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
                typedef typename property_map<Graph, vertex_index_t>::type Map;
                typedef unsigned Index; // This should be Graph::vertex_index_type
                Map m = get(vertex_index, g);
                Index x = get(vertex_index, g, Vertex());
                ignore_unused_variable_warning(x);

                // This is relaxed
                renumber_vertex_indices(g);

                const_constraints(g);
            }
            void const_constraints(const Graph& g)
            {
                typedef typename property_map<Graph, vertex_index_t>::const_type Map;
                Map m = get(vertex_index, g);
                ignore_unused_variable_warning(m);
            }
        private:
            Graph g;
        };

        BOOST_concept(EdgeIndexGraph,(Graph))
        {
            BOOST_CONCEPT_USAGE(EdgeIndexGraph)
            {
                // TODO: This relaxes the constraints so that the only
                // thing it actually requires is access to the property map
                // and indices.
                typedef typename graph_traits<Graph>::edge_descriptor Edge;
                typedef typename property_map<Graph, edge_index_t>::type Map;
                typedef unsigned Index; // This should be Graph::vertex_index_type
                Map m = get(edge_index, g);
                Index x = get(edge_index, g, Edge());
                ignore_unused_variable_warning(x);

                // This is relaxed
                renumber_edge_indices(g);

                const_constraints(g);
            }
            void const_constraints(const Graph& g)
            {
                typedef typename property_map<Graph, edge_index_t>::const_type Map;
                Map m = get(edge_index, g);
                ignore_unused_variable_warning(m);
            }
        private:
            Graph g;
        };

        BOOST_concept(NumericValue,(Numeric))
        {
            BOOST_CONCEPT_USAGE(NumericValue)
            {
                function_requires< DefaultConstructible<Numeric> >();
                function_requires< CopyConstructible<Numeric> >();
                numeric_values<Numeric>::zero();
                numeric_values<Numeric>::infinity();
            }
        };

        BOOST_concept(DegreeMeasure,(Measure)(Graph))
        {
            BOOST_CONCEPT_USAGE(DegreeMeasure)
            {
                typedef typename Measure::degree_type Degree;
                typedef typename Measure::vertex_type Vertex;

                Degree d = m(Vertex(), g);
                ignore_unused_variable_warning(d);
            }
        private:
            Measure m;
            Graph g;
        };

        BOOST_concept(DistanceMeasure,(Measure)(Graph))
        {
            BOOST_CONCEPT_USAGE(DistanceMeasure)
            {
                typedef typename Measure::distance_type Distance;
                typedef typename Measure::result_type Result;
                Result r = m(Distance(), g);
                ignore_unused_variable_warning(r);
            }
        private:
            Measure m;
            Graph g;
        };

        BOOST_concept(CycleVisitor,(Visitor)(Path)(Graph))
        {
            BOOST_CONCEPT_USAGE(CycleVisitor)
            {
                vis.cycle(p, g);
            }
        private:
            Visitor vis;
            Graph g;
            Path p;
        };

        BOOST_concept(CliqueVisitor,(Visitor)(Clique)(Graph))
        {
            BOOST_CONCEPT_USAGE(CliqueVisitor)
            {
                vis.clique(k, g);
            }
        private:
            Visitor vis;
            Graph g;
            Clique k;
        };
}

    using boost::concepts::VertexIndexGraphConcept;
    using boost::concepts::EdgeIndexGraphConcept;
    using boost::concepts::NumericValueConcept;
    using boost::concepts::DistanceMeasureConcept;
    using boost::concepts::DegreeMeasureConcept;
    using boost::concepts::CycleVisitorConcept;
    using boost::concepts::CliqueVisitorConcept;
}
#include <boost/concept/detail/concept_undef.hpp>

#endif


