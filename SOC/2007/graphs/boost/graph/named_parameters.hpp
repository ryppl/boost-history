// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_NAMED_PARAMETERS_HPP
#define BOOST_GRAPH_NAMED_PARAMETERS_HPP

// boost includes
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
    BOOST_PARAMETER_NAME(is_connected)

    // various map-type parameters
    BOOST_PARAMETER_NAME(distance_map)
    BOOST_PARAMETER_NAME(component_map)
    BOOST_PARAMETER_NAME(color_map)
    BOOST_PARAMETER_NAME(vertex_index_map)

    struct not_given {};
}

#endif
