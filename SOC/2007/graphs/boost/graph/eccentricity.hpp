// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_ECCENTRICITY_HPP
#define BOOST_GRAPH_ECCENTRICITY_HPP

#include <boost/graph/detail/combine_distances.hpp>

namespace boost
{
    template <
        typename Graph,
        typename DistanceMap,
        typename Combinator>
    inline typename property_traits<DistanceMap>::value_type
    eccentricity(const Graph& g,
                 dist,
                 Combinator combine,
                 typename property_traits<DistanceMap>::value_type zero = T(),
                 typename property_traits<DistanceMap>::value_type inf = std::numeric_limits<T>::max())
    {
        return detail::combine_distances(g, dist, combine, zero, inf);
    }

    template <typename Graph, typename DistanceMap>
    inline T
    eccentricity(const Graph& g, DistanceMap dist)
    {
        return eccentricity<T>(g, dist, std::plus<T>());
    }

    template <typename Graph, typename DistanceMap>
    inline double
    mean_geodesic(const Graph& g, DistanceMap dist)
    {
        return mean_geodesic<double>(g, dist);
    }


    template <typename Graph, typename DistanceMap>
    inline typename property_traits<DistanceMap>::value_type
    eccentricity(const Graph& g,
                 DistanceMap dist)
    {
        typename property_traits<DistanceMap>::value_type ret(0);
        typename graph_traits<Graph>::vertex_iterator i, end;
        for(tie(i, end) = vertices(g); i != end; ++i) {
            ret = std::max(ret, dist[*i]);
        }
        return ret;
    }

    // The computation of eccentricities, radius and diameter are all
    // closely related. Basically, these computations can be run at
    // the same time - compute eccentricities of all vertices, and
    // the radius and diameter of the graph.

    template <typename Graph, typename DistanceMatrix, typename EccentricityMap>
    void
    eccentricities(const Graph& g, DistanceMatrix& dist, EccentricityMap ecc)
    {
        typename graph_traits<Graph>::vertex_iterator i, j, end;
        for(tie(i, end) = vertices(g); i != end; ++i) {
            // compute the max eccentricity "in-place"
            typename property_traits<EccentricityMap>::value_type& ei = ecc[*i];
            for(j = vertices(g).first; j != end; ++j) {
                ei = std::max(ei, dist[*i][*j]);
            }
        }
    }
