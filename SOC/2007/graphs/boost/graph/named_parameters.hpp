// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_NAMED_PARAMETERS_HPP
#define BOOST_GRAPH_NAMED_PARAMETERS_HPP

#ifndef BOOST_GRAPH_REQUIRED_PARAMA_ARITY
#  define BOOST_GRAPH_REQUIRED_PARAM_ARITY 10
#endif

#if defined(BOOST_PARAMETER_MAX_ARITY) && \
    (BOOST_PARAMETER_MAX_ARIT < BOOST_GRAPH_REQUIRED_PARAM_ARITY)
#  warning "BOOST_PARAMETER_MAX_ARITY is too small for Boost.Graph"
#else
#  define BOOST_PARAMETER_MAX_ARITY BOOST_GRAPH_REQUIRED_PARAM_ARITY
#endif

#include <boost/parameter.hpp>

namespace boost
{
    BOOST_PARAMETER_NAME(graph)

    // data and data sets
    BOOST_PARAMETER_NAME(distribution)
    BOOST_PARAMETER_NAME(in_distribution)
    BOOST_PARAMETER_NAME(out_distribution)
    BOOST_PARAMETER_NAME(histogram)
    BOOST_PARAMETER_NAME(in_histogram)
    BOOST_PARAMETER_NAME(out_histogram)
    BOOST_PARAMETER_NAME(components)
    BOOST_PARAMETER_NAME(number)

    // various map-type parameters
    BOOST_PARAMETER_NAME(distance_map)
    BOOST_PARAMETER_NAME(component_map)
    BOOST_PARAMETER_NAME(color_map)
    BOOST_PARAMETER_NAME(vertex_index_map)

    // miscellaneous
    BOOST_PARAMETER_NAME(combine)
    BOOST_PARAMETER_NAME(distance_numbers)
    BOOST_PARAMETER_NAME(output_numbers)
}

#endif
