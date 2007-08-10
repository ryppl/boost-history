// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_NEW_GRAPH_CONCEPTS_HXX
#define BOOST_GRAPH_NEW_GRAPH_CONCEPTS_HXX

#include <boost/graph/graph_concepts.hpp>

#include <boost/concept/detail/concept_def.hpp>
namespace boost
{
    // The DegreeMeasure is its own concept because it does, in a way
    // specialize the semantics of an AdaptableBinaryFunction. Specifically,
    // it is templated over a single type - a graph and derives its result
    // and argument types from that graph.

    namespace concepts
    {
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
    }

    using boost::concepts::DegreeMeasureConcept;
}
#include <boost/concept/detail/concept_undef.hpp>

#endif


