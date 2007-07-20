// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_DISTANCE_HPP
#define BOOST_GRAPH_DISTANCE_HPP

// boost includes
#include <boost/graph/named_parameters.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/properties.hpp>

namespace boost
{
    namespace detail
    {
        template <typename Graph, typename DistanceMap>
        inline typename property_traits<DistanceMap>::value_type
        sum_distances(const Graph& g, DistanceMap dist)
        {
            size_t ret = 0;
            typename graph_traits<Graph>::vertex_iterator i, end;
            for(tie(i, end) = vertices(g); i != end; ++i) {
                ret += dist[*i];
            }
            return ret;
        }
    }

    // These measures operate on the first vertex given. This is to say that
    // closeness(g, v, dist) will return the closeness of the vertex v with
    // respect to all other vertices in the graph.
    //
    // Note that the target vertex in each algorithm is essentially a dummy
    // parameter (for now). If the distance map isn't supplied, then we
    // may shell to default computations.
    //
    // Vertex distance measures:
    // - geodesic_distance
    // - mean_geodesic_distance
    // - closeness
    // - eccentricity
    //
    // Graph distance measures:
    // - radius
    // - diameter
    //
    // Note that two versions of each algorithm exist. One takes a precomputed
    // distance map or matrix while the other computes it on the fly by trying
    // to guess an algorithm to use.

    template <typename Graph, typename DistanceMap>
    inline typename property_traits<DistanceMap>::value_type
    geodesic_distance(const Graph& g,
                      typename graph_traits<Graph>::vertex_descriptor v,
                      DistanceMap dist)
    {
        return dist[v];
    }

    template <typename Graph, typename DistanceMap>
    inline double
    mean_geodesic_distance(const Graph& g,
                           DistanceMap dist)
    {
        return double(detail::sum_distances(g, dist)) / double(num_vertices(g));
    }

    // Arbitrary numeric version uses T as some numeric type.
    // T must be constructible from degree_size_type and
    // DistanceMap::value_type. Note that above T == double.
    template <typename Graph, typename DistanceMap, typename T>
    inline T
    mean_geodesic_distance(const Graph& g,
                           DistanceMap dist,
                           const T& dummy)
    {
        return T(detail::sum_distances(g, dist)) / T(num_vertices(g));
    }


    template <typename Graph, typename DistanceMap>
    inline double
    closeness(const Graph& g,
              DistanceMap dist)
    {
        return double(1) / double(detail::sum_distances(g, dist));
    }

    // Arbitrary numeric version uses T as some numeric type.
    // T must be constructible from integral and DistanceMap::value_type.
    // Note that above T == double.
    template <typename Graph, typename DistanceMap, typename T>
    inline T
    closeness(const Graph& g,
              DistanceMap dist,
              const T& dummy)
    {
        return dummy(1) / double(detail::sum_distances(g, dist));
    }

    // Note that the DistanceMap::value_type must be constructible
    // as 0 - basically indicating an acceptable default value.
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
