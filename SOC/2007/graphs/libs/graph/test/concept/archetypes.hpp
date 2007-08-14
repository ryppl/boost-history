// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_GRAPH_CONCEPT_CHECK_ARCHETYPES_HPP
#define BOOST_GRAPH_CONCEPT_CHECK_ARCHETYPES_HPP

#include <utility>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_archetypes.hpp>

namespace boost
{
    struct descriptor_archetype
        : public
            default_constructible_archetype<
                copy_constructible_archetype<
                    sgi_assignable_archetype<
                        equality_comparable_archetype<> > > >
    {
        descriptor_archetype() { }
        descriptor_archetype(detail::dummy_constructor) { }
    };

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
}

#endif
