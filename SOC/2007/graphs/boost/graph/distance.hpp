// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_DISTANCE_HPP
#define BOOST_GRAPH_DISTANCE_HPP

#include <boost/graph/detail/combine_distances.hpp>

namespace boost
{


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

    template <typename Graph, typename EccentricityMap>
    inline typename property_traits<EccentricityMap>::value_type
    radius(const Graph& g, EccentricityMap ecc)
    {
        typedef typename property_traits<EccentricityMap>::value_type eccentricity;

        eccentricity ret = ecc[*vertices(g).first];
        typename graph_traits<Graph>::vertex_iterator i, end;
        for(tie(i, end) = vertices(g); i != end; ++i) {
            ret = std::min(ret, ecc[*i]);
        }
        return ret;
    }

    template <typename Graph, typename EccentricityMap>
    inline typename property_traits<EccentricityMap>::value_type
    diameter(const Graph& g, EccentricityMap ecc)
    {
        typedef typename property_traits<EccentricityMap>::value_type eccentricity;

        eccentricity ret = ecc[*vertices(g).first];
        typename graph_traits<Graph>::vertex_iterator i, end;
        for(tie(i, end) = vertices(g); i != end; ++i) {
            ret = std::max(ret, ecc[*i]);
        }
        return ret;
    }

    // The following functions are pretty much gimmes once we've computed
    // some of the other properties (like eccentricities, radius, and
    // diameter).

    template <typename Graph, typename EccentricityMap, typename Inserter>
    inline void
    radial_group(const Graph& g,
                 EccentricityMap ecc,
                 Inserter ins,
                 typename property_traits<EccentricityMap>::value_type level)
    {
        typename Graph::vertex_iterator i, end;
        for(tie(i, end) = vertices(g); i != end; ++i) {
            if(ecc[*i] == level) {
                *ins++ = *i;
            }
        }
    }

    template <typename Graph, typename EccentricityMap, typename Inserter>
    inline void
    center(const Graph& g,
           typename property_traits<EccentricityMap>::value_type r,
           EccentricityMap ecc,
           Inserter ins)
    {
        radial_group(g, ecc, ins, r);
    }

    template <typename Graph, typename EccentricityMap, typename Inserter>
    inline void
    center(const Graph& g,
           EccentricityMap ecc,
           Inserter ins)
    {
        radial_group(g, ecc, ins, radius(g, ecc));
    }


    template <typename Graph, typename EccentricityMap, typename Inserter>
    inline void
    periphery(const Graph& g,
              typename property_traits<EccentricityMap>::value_type d,
              EccentricityMap ecc,
              Inserter ins)
    {
        radial_group(g, ecc, ins, d);
    }

    template <typename Graph, typename EccentricityMap, typename Inserter>
    inline void
    periphery(const Graph& g,
              EccentricityMap ecc,
              Inserter ins)
    {
        radial_group(g, ecc, ins, diameter(g, ecc));
    }
}

#endif
