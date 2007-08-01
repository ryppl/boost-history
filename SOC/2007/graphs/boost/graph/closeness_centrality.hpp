// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_CLOSENESS_CENTRALITY_HPP
#define BOOST_GRAPH_CLOSENESS_CENTRALITY_HPP

#include <boost/graph/detail/geodesic.hpp>
#include <boost/graph/exterior_property.hpp>

namespace boost
{
    template <
            typename Graph,
            typename DistanceNumbers,
            typename ResultNumbers,
            typename Reciprocal = detail::reciprocal<typename ResultNumbers::value_type>
        >
    struct closeness_measure
        : public geodesic_measure<Graph, DistanceNumbers, ResultNumbers>
    {
        typedef geodesic_measure< Graph, DistanceNumbers, ResultNumbers> base_type;
        typedef typename base_type::distance_type distance_type;
        typedef typename base_type::result_type result_type;

        result_type operator ()(distance_type d, const Graph&)
        {
            Reciprocal r;
            return
                (d == base_type::infinite_distance()) ?
                    base_type::zero_result() : r(result_type(d));
        }
    };

    template <typename Graph, typename DistanceMap>
    inline closeness_measure<
            Graph,
            typename property_traits<DistanceMap>::value_type,
            float,
            detail::reciprocal<float> >
    measure_closeness(const Graph&, DistanceMap)
    {
        typedef typename property_traits<DistanceMap>::value_type Distance;
        return closeness_measure<Graph, Distance, float, detail::reciprocal<float> >();
    }

    template <typename T, typename Graph, typename DistanceMap>
    inline closeness_measure<
            Graph,
            typename property_traits<DistanceMap>::value_type,
            T,
            detail::reciprocal<T> >
    measure_closeness(const Graph&, DistanceMap)
    {
        typedef typename property_traits<DistanceMap>::value_type Distance;
        return closeness_measure<Graph, Distance, T, detail::reciprocal<T> >();
    }

    template <typename T, typename Graph, typename DistanceMap, typename Reciprocal>
    inline closeness_measure<
            Graph,
            typename property_traits<DistanceMap>::value_type,
            T,
            Reciprocal>
    measure_closeness(const Graph&, DistanceMap)
    {
        typedef typename property_traits<DistanceMap>::value_type Distance;
        return closeness_measure<Graph, Distance, T, Reciprocal>();
    }

    template <typename Graph,
              typename DistanceMap,
              typename Measure,
              typename Combinator>
    inline typename Measure::result_type
    vertex_closeness_centrality(const Graph& g,
                                DistanceMap dist,
                                Measure measure,
                                Combinator combine)
    {
        typedef typename Measure::distance_type Distance;
        typedef typename Measure::distance_values DistanceValues;
        Distance n = detail::combine_distances(g, dist, combine,
                                               DistanceValues());
        return measure(n, num_vertices(g));
    }

    template <typename Graph,
              typename DistanceMap,
              typename Measure>
    inline typename Measure::result_type
    vertex_closeness_centrality(const Graph& g,
                                DistanceMap dist,
                                Measure measure)
    {
        typedef typename Measure::distance_type Distance;
        return vertex_closeness_centrality(g, dist, measure, std::plus<Distance>());
    }

    template <typename Graph, typename DistanceMap>
    inline float
    vertex_closeness_centrality(const Graph& g, DistanceMap dist)
    {
        return vertex_closeness_centrality(g, dist, measure_closeness(g, dist));
    }

    template <typename T, typename Graph, typename DistanceMap>
    inline T
    vertex_closeness_centrality(const Graph& g, DistanceMap dist)
    {
        return vertex_closeness_centrality(g, dist, measure_closeness<T>(g, dist));
    }

    template <typename Graph,
              typename DistanceMatrix,
              typename CentralityMap,
              typename Measure>
    inline void
    closeness_centrality(const Graph& g,
                         const DistanceMatrix& dist,
                         CentralityMap cent,
                         Measure measure)
    {
        typedef typename property_matrix_traits<DistanceMatrix>::value_type Distance;

        typename graph_traits<Graph>::vertex_iterator i, end;
        for(tie(i, end) = vertices(g); i != end; ++i) {
            cent[*i] = vertex_closeness_centrality(g, dist[*i], measure);
        }
    }

    template <typename Graph,
              typename DistanceMatrix,
              typename CentralityMap>
    inline void
    closeness_centrality(const Graph& g,
                         const DistanceMatrix& dist,
                         CentralityMap cent)
    {
        typedef typename property_matrix_traits<DistanceMatrix>::value_type Distance;
        typedef typename exterior_vertex_property<Graph, Distance>::map_type DistanceMap;
        typedef typename property_traits<CentralityMap>::value_type Result;
        closeness_centrality(g, dist, cent,
                             measure_closeness<Result>(g, DistanceMap()));
    }
}

#endif
