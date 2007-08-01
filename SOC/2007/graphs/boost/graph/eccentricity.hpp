// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_ECCENTRICITY_HPP
#define BOOST_GRAPH_ECCENTRICITY_HPP

#include <boost/graph/detail/geodesic.hpp>

namespace boost
{
    template <typename Graph,
              typename DistanceMap,
              typename Combinator>
    inline typename property_traits<DistanceMap>::value_type
    vertex_eccentricity(const Graph& g,
                        DistanceMap dist,
                        Combinator combine)
    {
        typedef typename property_traits<DistanceMap>::value_type Distance;
        return detail::combine_distances(g, dist, combine,
                                         numeric_values<Distance>());
    }

    template <typename Graph, typename DistanceMap>
    inline typename property_traits<DistanceMap>::value_type
    vertex_eccentricity(const Graph& g, DistanceMap dist)
    {
        typedef typename property_traits<DistanceMap>::value_type Distance;
        return vertex_eccentricity(g, dist, detail::maximize<Distance>());
    }

    template <typename Graph, typename DistanceMatrix, typename EccentricityMap>
    inline void
    eccentricity(const Graph& g, const DistanceMatrix& dist, EccentricityMap ecc)
    {
        typedef typename property_matrix_traits<DistanceMatrix>::value_type Distance;
        typedef typename exterior_vertex_property<Graph, Distance>::map_type DistanceMap;
        typename graph_traits<Graph>::vertex_iterator i, end;
        for(tie(i, end) = vertices(g); i != end; ++i) {
            ecc[*i] = vertex_eccentricity(g, DistanceMap(dist[*i]));
        }
    }
}

#endif
