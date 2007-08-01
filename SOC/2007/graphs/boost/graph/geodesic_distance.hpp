// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_GEODESIC_DISTANCE_HPP
#define BOOST_GRAPH_GEODESIC_DISTANCE_HPP

#include <boost/graph/detail/geodesic.hpp>
#include <boost/graph/exterior_property.hpp>

namespace boost
{

    template <typename Graph, typename DistanceType, typename ResultType>
    struct mean_geodesic_measure
        : public geodesic_measure<Graph, DistanceType, ResultType>
    {
        typedef geodesic_measure<Graph, DistanceType, ResultType> base_type;
        typedef typename base_type::distance_type distance_type;
        typedef typename base_type::result_type result_type;

        result_type operator ()(distance_type d, const Graph& g)
        {
            typedef result_type T;
            return
                (d == base_type::infinite_distance()) ?
                    base_type::infinite_result() :
                    T(d) / T(num_vertices(g));
        }
    };

    template <typename Graph, typename DistanceMap>
    inline mean_geodesic_measure<Graph, typename property_traits<DistanceMap>::value_type, float>
    measure_mean_geodesic(const Graph&, DistanceMap)
    {
        return mean_geodesic_measure<Graph, typename property_traits<DistanceMap>::value_type, float>();
    }

    template <typename T, typename Graph, typename DistanceMap>
    inline mean_geodesic_measure<Graph, typename property_traits<DistanceMap>::value_type, T>
    measure_mean_geodesic(const Graph&, DistanceMap)
    {
        return mean_geodesic_measure<Graph, typename property_traits<DistanceMap>::value_type, T>();
    }


    // This is a little different because it's expected that the result type
    // should (must?) be the same as the distance type. There's a type of
    // transitivity in this thinking... If the average of distances has type
    // X then the average of x's should also be type X.
    template <typename Graph, typename DistanceType>
    struct mean_graph_distance_measure
        : public geodesic_measure<Graph, DistanceType, DistanceType>
    {
        typedef geodesic_measure<Graph, DistanceType, DistanceType> base_type;
        typedef typename base_type::distance_type distance_type;
        typedef typename base_type::result_type result_type;
        typedef typename base_type::size_type size_type;

        inline result_type operator ()(distance_type d, const Graph& g)
        {
            typename graph_traits<Graph>::directed_category cat;
            return this->average(d, num_vertices(g), cat);
        }

    private:
        inline result_type
        average(distance_type d, size_type n, undirected_tag)
        {
            if(d == base_type::infinite_distance()) {
                return base_type::infinite_result();
            }
            else {
                result_type f = (result_type(n) + 1) / result_type(2);
                return d / f;
            }
        }

        inline result_type
        average(distance_type d, size_type n, directed_tag)
        {
            if(d == base_type::infinite_distance()) {
                return base_type::infinite_result();
            }
            else {
                return result_type(d) / result_type(n);
            }
        }
    };

    template <typename Graph, typename DistanceMap>
    inline mean_graph_distance_measure<Graph, typename property_traits<DistanceMap>::value_type>
    measure_graph_mean_geodesic(const Graph& g, DistanceMap dist)
    {
        typedef typename property_traits<DistanceMap>::value_type T;
        return mean_graph_distance_measure<Graph, T>();
    }

    template <typename Graph,
              typename DistanceMap,
              typename Measure,
              typename Combinator>
    inline typename Measure::result_type
    vertex_mean_geodesic(const Graph& g,
                         DistanceMap dist,
                         Measure measure,
                         Combinator combine)
    {
        typedef typename Measure::distance_type Distance;
        typedef typename Measure::distance_values DistanceValues;
        Distance n = detail::combine_distances(g, dist, combine,
                                               DistanceValues());
        return measure(n, g);
    }

    template <typename Graph,
              typename DistanceMap,
              typename Measure>
    inline typename Measure::result_type
    vertex_mean_geodesic(const Graph& g,
                         DistanceMap dist,
                         Measure measure)
    {
        typedef typename Measure::distance_type Distance;
        return vertex_mean_geodesic(g, dist, measure, std::plus<Distance>());
    }

    template <typename Graph, typename DistanceMap>
    inline float
    vertex_mean_geodesic(const Graph& g, DistanceMap dist)
    {
        return vertex_mean_geodesic(g, dist, measure_mean_geodesic(g, dist));
    }

    template <typename T, typename Graph, typename DistanceMap>
    inline T
    vertex_mean_geodesic(const Graph& g, DistanceMap dist)
    {
        return vertex_mean_geodesic(g, dist, measure_mean_geodesic<T>(g, dist));
    }


    template <typename Graph,
              typename DistanceMatrix,
              typename GeodesicMap,
              typename Measure>
    inline void
    mean_geodesic(const Graph& g,
                  const DistanceMatrix& dist,
                  GeodesicMap geo,
                  Measure measure)
    {
        typename graph_traits<Graph>::vertex_iterator i, end;
        for(tie(i, end) = vertices(g); i != end; ++i) {
            geo[*i] = vertex_mean_geodesic(g, dist[*i], measure);
        }
    }

    template <typename Graph,
              typename DistanceMatrix,
              typename GeodesicMap>
    inline void
    mean_geodesic(const Graph& g,
                  const DistanceMatrix& dist,
                  GeodesicMap geo)
    {
        typedef typename property_matrix_traits<DistanceMatrix>::value_type Distance;
        typedef typename exterior_vertex_property<Graph, Distance>::map_type DistanceMap;
        typedef typename property_traits<GeodesicMap>::value_type Result;
        mean_geodesic(g, dist, geo,
                      measure_mean_geodesic<Result>(g, DistanceMap()));
    }


    template <typename Graph, typename GeodesicMap, typename Measure>
    inline typename Measure::result_type
    graph_mean_geodesic(const Graph& g, GeodesicMap geo, Measure measure)
    {
        typedef typename Measure::result_type T;
        T sum = detail::combine_distances(g, geo, std::plus<T>(), numeric_values<T>());
        return measure(sum, g);
    }

    template <typename Graph, typename GeodesicMap>
    inline typename property_traits<GeodesicMap>::value_type
    graph_mean_geodesic(const Graph& g, GeodesicMap geo)
    {
        return graph_mean_geodesic(g, geo, measure_graph_mean_geodesic(g, geo));
    }
}

#endif
